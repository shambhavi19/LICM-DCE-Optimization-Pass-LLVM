//--------------------------------------------------------------------//
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
//--------------------------------------------------------------------//
#include "dataflow.h"
#include "dominators.h"
//--------------------------------------------------------------------//
using namespace llvm;
using namespace std;
//--------------------------------------------------------------------//
DominatorsPass::DominatorsPass() :  FunctionPass(ID) {}
//--------------------------------------------------------------------//
void DominatorsPass::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.setPreservesAll();
}
//--------------------------------------------------------------------//
Domain DominatorsPass::getDomain()
{
    return domain;
}
//--------------------------------------------------------------------//
bool DominatorsPass::runOnFunction(Function &F)
{
    outs() << "Dominators Pass \n";
    outs() << "Function name: " << F.getName() << "\n";

    domain.clear();
    for (BasicBlock &BB : F)
    {
        domain.push_back(&BB);
    }
    outs() << "Domain set:\n";
    printSetBB (&domain);

    BitVector boundary(domain.size(), false);
    BitVector init(domain.size(), true);
    Analysis *analysis = new Analysis(domain.size(), domain, INTERSECTION, FORWARD, boundary, init);
            
    dfa_result = analysis->dataflowAnalysis(F);
    //displayResultsDFA(dfa_result.result, domain);

    outs()<<"\n Immediate Dominators:\n";
    std::map<int,int> idom_map = idomCompute(dfa_result.result, domain);

    std::map<int,int>::iterator i;
    for (i = idom_map.begin();i != idom_map.end(); ++i)
    {
        // B is the immediate dominator of A
        BasicBlock* A = (BasicBlock*)domain[i->first];
        BasicBlock* B = (BasicBlock*)domain[i->second];
        if (A->hasName())
            outs() << A->getName() << " is dominated by ";
        else
        {
            A->printAsOperand(outs(), false);
            outs() << " is dominated by ";
        }

        if (B->hasName())
            outs() << B->getName() << "\n";
        else
        {
            B->printAsOperand(outs(), false);
            outs() << "\n";
        }
    }
        return false;
    }
//--------------------------------------------------------------------//
std::map<BasicBlock*,BBAttributes*> * DominatorsPass::getDomResult()
{
    return &(dfa_result.result);
}
//--------------------------------------------------------------------//
DominatorsPass::Analysis::Analysis (int ds, Domain d, enum MeetOp m, enum Direction p, BitVector b, BitVector i) : Dataflow (ds, d, m, p, b, i) {}
//--------------------------------------------------------------------//
BBAttributes DominatorsPass::Analysis::transferFn (BitVector in, BasicBlock* current)
{
    StringRef assigned_var;
    Expression exp;
    BBAttributes output;
    BitVector null_set((int) domain.size(), false);
    BitVector gen = null_set;
    BitVector kill = null_set;

    gen.set(domainIndex(current));

    output.genSet = gen;
    output.killSet = kill;
    //output.tfOut = kill;
    //output.tfOut.flip();
    output.tfOut = in;
    output.tfOut |= gen;
    return output;
}
//--------------------------------------------------------------------//
int DominatorsPass::domainIndex(void* ptr)
{
    Domain D = domain;
    auto it = std::find(D.begin(), D.end(), ptr);
    int i = std::distance(D.begin(), it);
    if (i >= D.size() || i < 0) {
        i = -1;
    }
    return i;
}
//--------------------------------------------------------------------//
bool DominatorsPass::doms(BasicBlock* A, int index_a, BasicBlock*B, std::map<BasicBlock*, BBAttributes*> DFA_attribute_map_local )
{
    return (DFA_attribute_map_local[B]->output[index_a] == 1);
}
//--------------------------------------------------------------------//
std::map<int,int> DominatorsPass::idomCompute(std::map<BasicBlock*, BBAttributes*> DFA_attribute_map_local, Domain domain)
{
    BitVector tf_out;
    // std::map<BasicBlock*, BasicBlock*> idom_map;
    std::map<int,int> idom_map;
    bool flag_idom_found= false;

    // iterator i: indexes the block A whose immediate dominator is to be determined
    for(int i=0;i<domain.size();i++)
    {
        BasicBlock* A = (BasicBlock*)domain[i];
        tf_out = DFA_attribute_map_local[A]->output;
        flag_idom_found = false;

        // idom_map[A] = A;
        // iterator j: indexes the block B that could possibly be the immediate dominator of A.
        // Iterates over all the dominators of the block A
        for (int j : tf_out.set_bits()) 
        {
            if (flag_idom_found)
                break;
            BasicBlock* B = (BasicBlock*)domain[j];
            if(A==B)
                continue;

            // iterator k: indexes a block C
            for (int k : tf_out.set_bits()) 
            {
                BasicBlock* C= (BasicBlock*)domain[k];
                // iterates only over blocks that are beyond block B
                if (k<=j)
                    continue;
                if(A==C)
                {
                    // Final check to determine immediate dominator
                    if(doms(B,j,C, DFA_attribute_map_local))
                    {
                        flag_idom_found = true;
                        // idom found
                        // outs()<<i<<", "<<j<<"\n";
                        idom_map.insert(std::pair<int,int> (i,j));
                        break;
                    }
                }
                // Idea is to check if a case B dominates C and C dominates A occurs, then B cannot be immediate dominator of A
                if(doms(B,j,C,DFA_attribute_map_local) && doms(C,k,A,DFA_attribute_map_local))
                    break;
            }//C
        }//B
    }//A
    return idom_map; 
} 
//--------------------------------------------------------------------//
void DominatorsPass::displayResultsDFA(std::map<BasicBlock*, BBAttributes*> DFA_attribute_map_local, Domain domain)
{    
    std::map<BasicBlock*, BBAttributes*>::iterator i;

    for (i = DFA_attribute_map_local.begin(); i != DFA_attribute_map_local.end(); ++i)
    {

    struct BBAttributes *temp_bb_attr = DFA_attribute_map_local[i->first];
    if (temp_bb_attr->BB->hasName())
        outs() << "BB Name: " << temp_bb_attr->BB->getName() << "\n";
    else
    {
        outs() << "BB Name: ";
        temp_bb_attr->BB->printAsOperand(outs(), false);
        outs() << "\n";
    }

    Domain bb_gen_set_exp;
    //Domain bb_gen_set_exp;
    outs() << "gen[BB]: ";
    for (int j = 0; j < (int)temp_bb_attr->genSet.size(); ++j)
    {
        if (temp_bb_attr->genSet[j])
        bb_gen_set_exp.push_back(domain[j]);
    }
    printSetBB (&bb_gen_set_exp);

    Domain bb_kill_set_exp;
    outs() << "kill[BB]: ";
    for (int j = 0; j < temp_bb_attr->killSet.size(); ++j)
    {
        if (temp_bb_attr->killSet[j])
        bb_kill_set_exp.push_back(domain[j]);
    }
    printSetBB (&bb_kill_set_exp);

    Domain bb_in_set_exp;
    outs() << "IN[BB]: ";
    for (int j = 0; j < temp_bb_attr->input.size(); ++j)
    {
        if (temp_bb_attr->input[j])
            bb_in_set_exp.push_back(domain[j]);
    }
    printSetBB (&bb_in_set_exp);

    Domain bb_out_set_exp;
    outs() << "OUT[BB]: ";
    for (int j = 0; j < temp_bb_attr->output.size(); ++j)
    {
        if (temp_bb_attr->output[j])
            bb_out_set_exp.push_back(domain[j]);
    }
    printSetBB (&bb_out_set_exp);
    outs() << "\n";
    }
}
//--------------------------------------------------------------------//
char DominatorsPass::ID = 0;
RegisterPass<DominatorsPass> dominators("dominators", "ECE/CS 5544 Dominators Pass");
//--------------------------------------------------------------------//
