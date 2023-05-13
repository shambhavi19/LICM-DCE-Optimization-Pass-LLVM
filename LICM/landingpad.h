// ECE/CS 5544 S23 Assignment 3: dominators.h
// Group: Shambhavi Kuthe, Rohit Mehta

////////////////////////////////////////////////////////////////////////////////
#ifndef __LANDINGPAD_H__
#define __LANDINGPAD_H__

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/LoopInfo.h"
#include <llvm/Analysis/LoopPass.h>
#include "dataflow.h"

using namespace llvm;
using namespace std;

class LandingPad : public LoopPass 
{
      private:
      BasicBlock *removePhiDep(BasicBlock *new_test, BasicBlock *header);

    	void updatePhiNotInLoop (Loop &loop, std::vector<Instruction *> &prevPhiUsers, PHINode *from, PHINode *to);

      void unifyPhiAtExit (BasicBlock *new_test, BasicBlock *unified_exit, BasicBlock *loop_exit, BasicBlock *header, BasicBlock *last_bdody, Loop *L);

      public:
        static char ID;
        LandingPad();
        virtual bool runOnLoop(Loop *L, LPPassManager &LPM);

        virtual void getAnalysisUsage(AnalysisUsage &AU) const;
       
};
#endif
