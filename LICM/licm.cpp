// ECE/CS 5544 S23 Assignment 2: licm.cpp
// Group: Shambhavi Kuthe, Rohit Mehta


////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include <llvm/Analysis/ValueTracking.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Transforms/Utils/SSAUpdater.h>
#include <vector>

#include "dataflow.h"
#include "landingpad.h"
#include "dominators.h"
#include "reaching.h"

// using namespace llvm;
using namespace std;

namespace llvm{

  enum VarType{VARIANT = 0, INVARIANT = 1};
  class LICM: public LoopPass
  {
	  
public:
    static char ID;
    Domain dominatorDomain;
    std::map<Instruction *, VarType> loopInvariantList;
    LoopInfo *LI;
    std::map<BasicBlock *, BBAttributes *> *reaching_result;
    std::map<BasicBlock *, BBAttributes *> *dominator_result;

    LICM() : LoopPass(ID) {}

    virtual void getAnalysisUsage(AnalysisUsage& AU) const 
    {
      // AU.setPreservesAll();
      AU.setPreservesCFG();
      AU.addRequired<Reaching>();
      AU.addRequired<DominatorsPass>();
      AU.addRequired<LoopInfoWrapperPass>();

    }

    /*Checks if the instruction in the list is invariant. Returns true for invariant instruction*/
    bool checkInvariant(Instruction *I, std::vector<Instruction*> instrList)
    {
      bool isInvariant = isSafeToSpeculativelyExecute(I) && !I->mayReadFromMemory() && !isa<LandingPadInst>(I);
      if (isInvariant && loopInvariantList[I] == VARIANT)
      {
        for (int i=0; i<I->getNumOperands(); i++)
        {
          auto it = std::find(instrList.begin(), instrList.end(), I->getOperand(i));
          
          //Variant: if found in the list
          if (it != instrList.end())
          {
              if (loopInvariantList[*it] == VARIANT)
              {
                  isInvariant = false;
              }
          }
        }

        //Invariant if result is true
        if (isInvariant)
        {
          loopInvariantList[I] = INVARIANT;
        }
      }

      return isInvariant;
    }

    virtual bool runOnLoop(Loop *L, LPPassManager &LPM)
    {

      outs() << "\n\nLICM Transformation\n";

      //Get analysis from reaching nd dominator pass and store the result and domain
      reaching_result = getAnalysis<Reaching>().getReachingResult();
      dominator_result = getAnalysis<DominatorsPass>().getDomResult();
      dominatorDomain = getAnalysis<DominatorsPass>().getDomain();

      std::vector<Instruction *> loopInstrs, invarInstrs;
      int oldSize = 0;
      BasicBlock *preheader = L->getLoopPreheader();

      if(preheader == NULL)
      {
        outs() << "Loop preheader not found\n";
      }
      else
      {
        outs() << "Loop transform possible\n";
        outs() << "Loop preheader " << *preheader << "\n";
      }

      outs() << "Binary instructions in the loop:\n";
      for(Loop::block_iterator bi = L->block_begin(); bi != L->block_end(); ++bi)
      {
        BasicBlock *BB = *bi;
        for(BasicBlock::iterator it = BB->begin(); it != BB->end(); ++it)
        {
          Instruction *I = &*it;
          /*Instert binary, phi and arguments to the list*/
          if(isa<BinaryOperator>(I) || isa<PHINode>(I) || isa<Argument>(I))
          {
            loopInstrs.push_back(I);
            loopInvariantList[I] = VARIANT;
            outs() << "\t" << *I << "\n";
          }
        }
      }

      //Convergence
      do
      {
        outs() << "Old size: "<< oldSize << "Old iterator: " << invarInstrs.size() << "\n";
        oldSize = invarInstrs.size();

        if(preheader != NULL)
        {
          for (Loop::block_iterator bi = L->block_begin(); bi != L->block_end(); ++bi)
          {
            BasicBlock *BB = *bi;
            for (BasicBlock::iterator it = BB->begin(); it != BB->end(); ++it)
            {
              Instruction *I = &*it;

              if (isa<BinaryOperator>(I) || isa<PHINode>(I) || isa<Argument>(I))
              {
                //Check invariant 
                bool result = checkInvariant(I, loopInstrs);
                outs() << *I;

                /*Add invariant instructions to invariant list*/
                if(result)
                {
                  auto iter = std::find(invarInstrs.begin(), invarInstrs.end(), I);
                  if(iter == invarInstrs.end())
                  {
                    invarInstrs.push_back(I);
                  }
                  outs() << " - invariant\n";
                }
                else
                {
                  outs() << " - not invariant\n";
                }
              }
            }
          }
        }
      }while(oldSize != invarInstrs.size()); 

      for(auto i : invarInstrs)
      {
        Instruction *invariant = dyn_cast<Instruction>(i);
        int numUses = (i)->getNumUses();
        BasicBlock *invarParent = invariant->getParent();
        bool minOneUse = false;

        //Check for usage in other blocks
        for (Loop::block_iterator bi = L->block_begin(); bi != L->block_end(); ++bi)
        {
          BasicBlock *BB = *bi;
          if (i->isUsedInBasicBlock(BB))
          {
            //Usage found in another block
            minOneUse = true;
            // Check if invariant instruction parent block dominates identified user block
            BitVector dom = (*dominator_result)[BB]->output;
            auto iterate = std::find(dominatorDomain.begin(), dominatorDomain.end(), invarParent);
            if (iterate != dominatorDomain.end())
            {
              if (dom[iterate - dominatorDomain.begin()] == 1)
              {
                  // If it dominates, move invariant instruction to preheader
                  ((Instruction *)i)->moveBefore(preheader->getTerminator());                                
              }
            }
          }
        }

        // If invariant instruction is unused, move to preheader
        if (!minOneUse)
        {
          ((Instruction *)i)->moveBefore(preheader->getTerminator());
        }

        // Check if the invariant instruction has constant operands
        bool constOp = true;
        for (auto op = invariant->op_begin(), op_end = invariant->op_end(); op != op_end; ++op)
        {
          if (!isa<Constant>(*op))
          {
            constOp = false;
          }
        }

        // if operands are constant move invariant instruction to preheader 
        if (constOp)
        {
            ((Instruction *)i)->moveBefore(preheader->getTerminator());
        }
        outs() << "\n";

      }
      return true;
    }


  };

  char LICM::ID = 1;
  RegisterPass<LICM> licm("loop-invariant-code-motion", "ECE/CS 5544 LICM");

}






