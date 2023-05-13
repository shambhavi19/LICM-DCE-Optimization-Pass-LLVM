// ECE/CS 5544 S23 Assignment 3: faintness.cpp
// Group: Shambhavi Kuthe, Rohit Mehta


////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include "dataflow.h"
#include "faintness.h"

using namespace llvm;
using namespace std;

//  static char ID;

  Faintness::Faintness() : FunctionPass(ID) { }

  void Faintness::getAnalysisUsage(AnalysisUsage& AU) const {
    AU.setPreservesAll();
  }

  std::map<BasicBlock*, std::vector<llvm::BitVector>*>* Faintness::getBBInstrsMap()
  {
    return &BBInstrsFaintMap;
  }

  Domain Faintness::getDomain()
  {
    return domain;
  }

  bool Faintness::runOnFunction(Function &F)
  {
    outs() << "Faintness DFA\n";
    outs() << "Function name: " << F.getName() << "\n";

    int idx = 0;
    DFAResult faintness_result;
    bool flag = false;
  
    // Domain domain;
    for (BasicBlock &BB : F)
    {
      for (Instruction &I : BB)
      {
	    
        std::pair<BasicBlock*, std::vector<BitVector> *> BBMap;
        BBMap.first = &BB;
        BBMap.second = new std::vector<BitVector>();
        BBInstrsFaintMap.insert(BBMap);

          if (isa<BinaryOperator>(I) || isa<PHINode>(I) || isa<CmpInst>(I)|| isa<CastInst>(I))
          {
              domain.push_back(&I);
	      /*((Instruction*)domain[idx++])->print(outs());	
	      outs()<<"\n";*/
          }
      }
    }

    outs() << "DOMAIN set: \n";
    printSetInstr (&domain);

    //Initializing boundary and init vectors
    BitVector boundary(domain.size(), true);
    BitVector init(domain.size(), true);
    //Pass set of expressions(domain), size of domain, meet operator, direction, boundary and initial conditions to dataflow framework 
    //Faint analysis : Meet operator = Intersection, Direction = Forward
    Analysis *analysis = new Analysis(domain.size(), domain, INTERSECTION, FORWARD, boundary, init, BBInstrsFaintMap);
    //Run dataflow analysis and store result 
    faintness_result = analysis->dataflowAnalysis(F);
    BBInstrsFaintMap = analysis->getBBInstrsMap();
    //Display instruction map
    //displayInstrs(BBInstrsFaintMap, domain.size());

  }

  Faintness::Analysis::Analysis (int size, Domain domain, enum MeetOp mo, enum Direction dir, BitVector bv, BitVector iv, std::map<BasicBlock*, std::vector<llvm::BitVector>*> BBInstMap) : Dataflow (size, domain, mo, dir, bv, iv) 
  {
    BBInstrsMap =  BBInstMap;
  }

  std::map<BasicBlock*, std::vector<llvm::BitVector>*> Faintness::Analysis::getBBInstrsMap()
  {
    return BBInstrsMap;
  }

  //Transfer function: Faintness analysis. Function takes current Basic block and input vector as parameters
  //IN[BB] = fn(OUT[BB])
  //fn = genSet U (IN[BB] - killSet)
  BBAttributes Faintness::Analysis::transferFn(BitVector in, BasicBlock* current)
  {
    BBAttributes output;

    StringRef assigned_var;
    BitVector nullSet((int) domain.size(), false);
    BitVector faintGen = nullSet;
    BitVector faintKill = nullSet;
    unsigned long BBsize = (*current).size();

    for(BasicBlock::reverse_iterator i = current->rbegin(); i != current->rend(); ++i)
    {
      faintGen = getGen(&*i);
      faintKill = getKill(&*i, in);
      BitVector tempKill = faintKill;
      output.tfOut = tempKill.flip();
      output.tfOut &= in;
      output.tfOut |= faintGen;

      std::vector<llvm::BitVector> *out = (BBInstrsMap.find(current))->second;
      if (out->size() >= BBsize + 1)
      {
        out->clear();
      }

      if (out->empty())
      {
        out->insert(out->begin(), in);
      }

      if (out->size() <= BBsize)
      {
        out->insert(out->begin(), output.tfOut);
      }

    }

    return output;
  }

  BitVector Faintness::Analysis::getGen(llvm::Instruction *I)
  {
    BitVector gen(domain.size());
    if(isa<BinaryOperator>(*I) || isa<PHINode>(*I) || isa<CastInst>(*I) || isa<CmpInst>(*I))
    {
      std::vector<void *>::iterator it = std::find(domain.begin(), domain.end(), (void *)I);

      if (it != domain.end()) 
      {
        gen.set((size_t)(it - domain.begin()));
      } 
    }

    return gen;
  }

  BitVector Faintness::Analysis::getKill(llvm::Instruction *I, BitVector in)
  {
    BitVector kill(domain.size());

    kill = getConstKill(I);
    kill |= getDepKill(I, in);

    return kill;
  }

  BitVector Faintness::Analysis::getConstKill(llvm::Instruction *I)
  {
    BitVector ConstKill(domain.size());

    if(isa<LandingPadInst>(*I) || I->mayHaveSideEffects() || (*I).isTerminator())
    {
      std::vector<void *>::iterator it = std::find(domain.begin(), domain.end(), (void *)I->getOperand(0));

      if(it != domain.end())
      {
        ConstKill.set((size_t)(it - domain.begin()));
      }

    }

    if(isa<CallInst>(*I))
    {
	    for(auto op= I->op_begin(); op != I->op_end(); ++op)
	    {
      std::vector<void *>::iterator it = std::find(domain.begin(), domain.end(), (void *)op->get());

      if(it != domain.end())
      {
        ConstKill.set((size_t)(it - domain.begin()));
      }
	    }

    }

    return ConstKill;
  }

  BitVector Faintness::Analysis::getDepKill(llvm::Instruction *I, BitVector in)
  {
    BitVector DepKill(domain.size());
    bool checkSetBit = false;

    if(isa<BinaryOperator>(*I) || isa<PHINode>(*I) || isa<CastInst>(*I) || isa<CmpInst>(*I))
    {
      for(int i : in.set_bits())
      {
          if(i < (int)domain.size())
          {
            if((void *)I == domain[i])
            {
              checkSetBit = true;
            }
          }
      }

      if(!checkSetBit)
      {
	      for(auto op = I->op_begin(); op != I->op_end(); ++op)
	      {
        std::vector<void *>::iterator it = std::find(domain.begin(), domain.end(), (void *)op->get());

        if (it != domain.end()) 
        {
          DepKill.set((size_t)(it - domain.begin()));
        }
      }
      } 
    }

    return DepKill;
  }

  void Faintness::displayBitVector(const llvm::BitVector bitv, int num)
  {
    for (int i = 0; i < num; ++i) {
      outs() << bitv[i];
    }
    outs() << "\n";
  }

  void Faintness::displayInstrs(std::map<BasicBlock*, std::vector<llvm::BitVector>*> BBInstrsMap, int size)
  {
    std::map<BasicBlock*, std::vector<llvm::BitVector>*>::iterator i;
    for (i = BBInstrsMap.begin(); i != BBInstrsMap.end(); ++i)
    {
      if ((i->first)->hasName())
        outs() << "BB Name: " << (i->first)->getName() << "\n";
      else
      {
        outs() << "BB Name: ";
        (i->first)->printAsOperand(outs(), false);
        outs() << "\n";
      }
      
      std::vector<llvm::BitVector> *temp_bb_instr = BBInstrsMap[i->first];
      int BBSize = (i->first)->size();
      for (int j = 0; j<BBSize; ++j)
      {
        displayBitVector(((*temp_bb_instr)[j]), size);
      }
      outs() << "\n";

    }

  }
  
  //Function to print the dataflow analysis results
  void Faintness::displayResultsDFA(std::map<BasicBlock*, BBAttributes*> BBAttrsMap_print, Domain domain)
  {
    std::map<BasicBlock*, BBAttributes*>::iterator i;

    for (i = BBAttrsMap_print.begin(); i != BBAttrsMap_print.end(); ++i)
    {
      struct BBAttributes *temp_bb_attr = BBAttrsMap_print[i->first];

      if (temp_bb_attr->BB->hasName())
        outs() << "BB Name: " << temp_bb_attr->BB->getName() << "\n";
      else
      {
        outs() << "BB Name: ";
        temp_bb_attr->BB->printAsOperand(outs(), false);
        outs() << "\n";
      }

      // Domain bb_gen_set_exp;
      // outs() << "gen[BB]: ";
      // for (int j = 0; j < temp_bb_attr->genSet.size(); ++j)
      // {
      //   if (temp_bb_attr->genSet[j])
      //       bb_gen_set_exp.push_back(domain[j]);
      // }
      // printSetInstr (&bb_gen_set_exp);

      // Domain bb_kill_set_exp;
      // outs() << "kill[BB]: ";
      // for (int j = 0; j < temp_bb_attr->killSet.size(); ++j)
      // {
      //   if (temp_bb_attr->killSet[j])
      //       bb_kill_set_exp.push_back(domain[j]);
      // }
      // printSetInstr (&bb_kill_set_exp);

      Domain bb_in_set_exp;
      outs() << "IN[BB]: ";
      for (int j = 0; j < temp_bb_attr->input.size(); ++j)
      {
        if (temp_bb_attr->input[j])
            bb_in_set_exp.push_back(domain[j]);
      }
      printSetInstr (&bb_in_set_exp);

      Domain bb_out_set_exp;
      outs() << "OUT[BB]: ";
      for (int j = 0; j < temp_bb_attr->output.size(); ++j)
      {
        if (temp_bb_attr->output[j])
            bb_out_set_exp.push_back(domain[j]);
      }
      printSetInstr (&bb_out_set_exp);
      outs() << "\n";
    }
  }

  char Faintness::ID = 0;
  RegisterPass<Faintness> faintness("faintness", "ECE/CS 5544 Faintness");
