#ifndef __FAINTNESS_H__
#define __FAINTNESS_H__

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "dataflow.h"

using namespace llvm;
using namespace std;

class Faintness : public FunctionPass 
{
      private:
    	DFAResult faintness_result;
      std::map<BasicBlock*, std::vector<llvm::BitVector>*> BBInstrsFaintMap;
      void displayResultsDFA(std::map<BasicBlock*, BBAttributes*> BBAttrsMap_print, Domain domain);
      void displayInstrs(std::map<BasicBlock*, std::vector<llvm::BitVector>*> BBInstrsMap, int size);
      void displayBitVector(const llvm::BitVector bv, int num);

      public:
        static char ID;
    	  Domain domain;
        Faintness();
        virtual bool runOnFunction(Function &F);

        virtual void getAnalysisUsage(AnalysisUsage &AU) const;

        std::map<BasicBlock*,std::vector<llvm::BitVector>*> *getBBInstrsMap();

	Domain getDomain();
        class Analysis : public Dataflow 
        {
        public:
        	std::map<BasicBlock*, std::vector<llvm::BitVector>*> BBInstrsMap;
        	Analysis (int size, Domain domain, enum MeetOp mo, enum Direction dir, BitVector bv, BitVector iv, std::map<BasicBlock*, std::vector<llvm::BitVector>*> BBInstMap);
          std::map<BasicBlock*, std::vector<llvm::BitVector>*> getBBInstrsMap();
          BBAttributes transferFn (BitVector in, BasicBlock* current);
          BitVector getGen(llvm::Instruction *I);
          BitVector getKill(llvm::Instruction *I, BitVector in);
          BitVector getConstKill(llvm::Instruction *I);
          BitVector getDepKill(llvm::Instruction *I, BitVector in);
        };
       
};
#endif
