// ECE/CS 5544 S23 Assignment 3: dominators.cpp
// Group: Shambhavi Kuthe, Rohit Mehta

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Transforms/Utils/SSAUpdater.h>

#include "dataflow.h"
#include "landingpad.h"

using namespace llvm;
using namespace std;

//namespace
//{
//    class LandingPad : public FunctionPass
//    {
//    public:
//        static char ID;
LandingPad::LandingPad() : LoopPass(ID) {}

    void LandingPad::getAnalysisUsage(AnalysisUsage &AU) const
    {
        AU.addRequired<LoopInfoWrapperPass>();
    }

    BasicBlock *LandingPad::removePhiDep(BasicBlock *new_test, BasicBlock *header)
    {
        BasicBlock *back_edge_block = NULL;
        for (Instruction &I : *header)
        {
            if (PHINode *phi = dyn_cast<PHINode>(&I))
            {
                back_edge_block = phi->getIncomingBlock(1);
                Value *incoming_val = phi->getIncomingValue(0);
                for (Instruction &new_test_I : *new_test)
                {
                    for (auto op = new_test_I.op_begin(); op!= new_test_I.op_end(); ++op)
                    {
                        if (op->get() == &I)
                        {
                            op->set(incoming_val);
                        }
                    }
                }
            }
        }
        return back_edge_block;
    }

    void LandingPad::updatePhiNotInLoop (Loop &loop, std::vector<Instruction *> &prev_phi_users, PHINode *from, PHINode *to)
    {
        std::vector<BasicBlock *> &loop_blocks = loop.getBlocksVector();

        for (Instruction *I : prev_phi_users)
        {
            BasicBlock *users_block = I->getParent();

            if ((std::find(loop_blocks.begin(), loop_blocks.end(), users_block) == loop_blocks.end()))
            {
                if (I != to)
                {
                    I->replaceUsesOfWith(from, to);
                }
            }
        }
    }

    void LandingPad::unifyPhiAtExit (BasicBlock *new_test, BasicBlock *unified_exit, BasicBlock *loop_exit, BasicBlock *header, BasicBlock *last_body, Loop *L)
    {
        std::vector<PHINode *> inserted_phi;
        std::vector<Instruction *> prev_phi_users;

        for (Instruction &I : *header)
        {
            if (PHINode *phi = dyn_cast<PHINode>(&I))
            {
                for (User *U : phi->users())
                {
                    if (Instruction *user_inst = dyn_cast<Instruction>(U))
                    {
                        prev_phi_users.push_back(user_inst);
                    }
                }

                PHINode *exit_phi = PHINode::Create(I.getType(), 0);
                exit_phi->addIncoming(phi, last_body);
                exit_phi->addIncoming(phi->getIncomingValue(0), new_test);
                loop_exit->getInstList().push_front(exit_phi);
                inserted_phi.push_back(phi);

                updatePhiNotInLoop(*L, prev_phi_users, phi, exit_phi);
            }
        }
    }

    bool LandingPad::runOnLoop(Loop *L, LPPassManager &LPM)
    {
        outs() << "Landing Pad Transformation\n";

        BasicBlock *prev_preheader = L->getLoopPreheader();
        BasicBlock *header = L->getHeader();

        LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

        // Dont process loops with no preheader
        if (prev_preheader != NULL)
        {
			//new preheader generated after splitting basic block at after terminator
            BasicBlock *new_preheader = prev_preheader->splitBasicBlock(prev_preheader->getTerminator(), "new_preheader");
			
            // add the new preheader into the parent loop, in case of nested loops
            if (Loop *parent = L->getParentLoop())
            {
                parent->addBasicBlockToLoop(new_preheader, LI);
            }
            // latch: BB whose successor includes the header of the loop. Can be conditional or unconditional trafer back to the loop header.
			//delete terminator instr from latch of loop
            BasicBlock *loop_latch = L->getLoopLatch();
            loop_latch->getTerminator()->eraseFromParent();
            // 

            std::map<Value *, Value *> replace_map;
            for (BasicBlock::iterator it = header->getFirstInsertionPt(); it != header->end(); ++it)
            {
				//clone instruction and add to replace map
                Instruction *clone = it->clone();
                replace_map[&*it] = clone;

				//cast clone to branch instr
				// add edge from clone to header
				//add cloned inst to instr list of the latch instr
                if (BranchInst *terminator = dyn_cast<BranchInst>(clone))
                {
                    terminator->setOperand(2, header);
                }
                loop_latch->getInstList().push_back(clone);
            }

            // replace instrs
            for (Instruction &I : *loop_latch)
            {
                for (auto op = I.op_begin(), end = I.op_end(); op!= end; ++op)
                {
                    auto to_replace = replace_map.find(*op);
                    if (to_replace != replace_map.end())
                    {
                        I.setOperand(op - I.op_begin(), to_replace->second);
                    }
                }
            }

            // Move all instructions in previous header to new split block
            prev_preheader->getTerminator()->eraseFromParent();
            prev_preheader->getInstList().splice(prev_preheader->end(), header->getInstList(), header->getFirstNonPHI()->getIterator(), header->end());

            // previous header is now the new test block
            BasicBlock *new_test = prev_preheader;

            // The loop latch is the last body block
            for (Instruction &I : *header)
            {
                if (PHINode *phi = dyn_cast<PHINode>(&I))
                {
                    Value *incoming_val = phi->getIncomingValue(0);
                    for (Instruction &new_test_I : *new_test)
                    {
                        for (auto op = new_test_I.op_begin(); op!= new_test_I.op_end(); ++op)
                        {
                            if (op->get() == &I)
                            {
                                op->set(incoming_val);
                            }
                        }
                    }
                }
            }

            // 
            BasicBlock *body_block = cast<BasicBlock>(cast<BranchInst>(new_test->getTerminator())->getOperand(2));
            cast<BranchInst>(new_test->getTerminator())->setOperand(2, new_preheader);

            // Add terminator for current header
            BranchInst::Create(body_block, header);

            // Unified loop exit created by splitting current loop exit
            BasicBlock *loop_exit = dyn_cast<BasicBlock>(new_test->getTerminator()->getOperand(1));

            BasicBlock *unified_exit = loop_exit->splitBasicBlock(loop_exit->begin(), ".unified_exit");

            if (Loop *parent = L->getParentLoop())
            {
                parent->addBasicBlockToLoop(unified_exit, LI);
            }

            // unifyPhiAtExit(new_test, loop_exit, header, loop_latch, L);
            // 
            std::vector<PHINode *> inserted_phi;
            std::vector<Instruction *> prev_phi_users;

            for (Instruction &I : *header)
            {
                if (PHINode *phi = dyn_cast<PHINode>(&I))
                {
                    for (User *U : phi->users())
                    {
                        if (Instruction *user_inst = dyn_cast<Instruction>(U))
                        {
                            prev_phi_users.push_back(user_inst);
                        }
                    }

                    PHINode *exit_phi = PHINode::Create(I.getType(), 0);
                    exit_phi->addIncoming(phi, loop_latch);
                    exit_phi->addIncoming(phi->getIncomingValue(0), new_test);
                    loop_exit->getInstList().push_front(exit_phi);
                    inserted_phi.push_back(phi);

                    // updatePhiNotInLoop(*L, prev_phi_users, phi, exit_phi);
                    // Update all the users of PHI instruction not in loop
                    std::vector<BasicBlock *> &loop_blocks = (*L).getBlocksVector();
                    for (Instruction *I : prev_phi_users)
                    {
                        BasicBlock *users_block = I->getParent();

                        if ((std::find(loop_blocks.begin(), loop_blocks.end(), users_block) == loop_blocks.end()))
                        {
                            if (I != exit_phi)
                            {
                                I->replaceUsesOfWith(phi, exit_phi);
                            }
                        }
                    }
                }
            }
        }

        // Did not modify the incoming Function.
        return true;
    }

//    };

char LandingPad::ID = 2;
RegisterPass<LandingPad> landingpad("landing_pad", "ECE/CS 5544 Landing Pad");

//};
