#ifndef __DOMINATORS_H__
#define __DOMINATORS_H__


#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "dataflow.h"
#include <vector>

using namespace llvm;
using namespace std;


typedef std::map<BasicBlock*, BBAttributes*> BB_attributes_map;

class DominatorsPass : public FunctionPass 
{

    private: 
        // std::map<BasicBlock*, BBDeps*> bb_dep_map;
        // std::map<Expression*, int> domain_index_map;
        // std::map<int, Expression> index_expression_map;
        DFAResult dfa_result, dfa_result_idom;

        int domainIndex(void* ptr);
        void displayInstrMap(std::map<BasicBlock*, std::vector<llvm::BitVector>*> bb_instr_map, int size);
        void displayResultsDFA(std::map<BasicBlock*, BBAttributes*> DFA_attribute_map_local, Domain domain);
        std::map<int,int> idomCompute(std::map<BasicBlock*, BBAttributes*> DFA_attribute_map_local, Domain domain);
            // Domain domain);
        bool doms(BasicBlock* A, int index_a, BasicBlock*B,std::map<BasicBlock*, BBAttributes*> DFA_attribute_map_local );
    
    public:
        static char ID;
        Domain domain;

        DominatorsPass();
        virtual bool runOnFunction(Function &F);
        virtual void getAnalysisUsage(AnalysisUsage &AU) const;

        Domain getDomain();
        std::map<BasicBlock*,BBAttributes*> * getDomResult();

        


class Analysis : public Dataflow
{
    protected:

    std::map<BasicBlock*, std::vector<llvm::BitVector>*> bb_instr_map;

    public:
        Analysis (int ds, Domain d, enum MeetOp m, enum Direction p, BitVector b, BitVector i);

        std::map<BasicBlock*, std::vector<llvm::BitVector>*> getBBAndInstrMap();        
        BBAttributes transferFn (BitVector in, BasicBlock* current);        
};
        

};

#endif
