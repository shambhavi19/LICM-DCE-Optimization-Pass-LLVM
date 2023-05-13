// ECE/CS 5544 S23 Assignment 3: dce.cpp
// Group: Shambhavi Kuthe, Rohit Mehta

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "dataflow.h"
#include "faintness.h"


using namespace llvm;
using namespace std;


namespace{

    class DCE : public FunctionPass
    {

    public:
        static char ID;

        DCE() : FunctionPass(ID) {}

        virtual void getAnalysisUsage(AnalysisUsage &AU) const
        {
            // AU.setPreservesAll();
            AU.setPreservesCFG();
            AU.addRequired<Faintness>();
        }


    virtual bool runOnFunction(Function& F)
    {
        outs() << "DCE\n";
        outs() << "Function name: " << F.getName() << "\n";

        std::map<BasicBlock*, std::vector<llvm::BitVector>*>* FaintMap = getAnalysis<Faintness>().getBBInstrsMap();      
        Domain domain = getAnalysis<Faintness>().getDomain();
        std::vector<Instruction*> deleteInstrs;

        for(BasicBlock &BB : F)
        {
            unsigned long i = 0;
            std::vector<BitVector> *out = (*FaintMap)[&BB];

            for(Instruction &I : BB)
            {
                if(!(I).isTerminator())
                {
                    bool flag = false;
                    BitVector in = (*out)[i];
                    for(int idx: in.set_bits())
                    {
                        if(idx < (int) domain.size())
                        {
                            if((void *)&I == domain[idx])
                            {
                                flag = true;
                            }
                        }
                    }
                    if(flag)
                    {
                        deleteInstrs.push_back(&I);
                    }

                }
                ++i;
            }
        }

        outs() << "Deleted Instructions: \n";
        for(Instruction *I: deleteInstrs)
        {
            outs() << *I << "\n";
            UndefValue *undef = UndefValue::get(I->getType());
            I->replaceAllUsesWith(undef);
            I->eraseFromParent();
        }

        return true;
    }

    private:
        
    };

    char DCE::ID = 1;
    RegisterPass<DCE> X("dead-code-elimination", "ECE/CS 5544 Dead Code Elimination");
};

