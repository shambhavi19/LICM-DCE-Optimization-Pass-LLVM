INC=-I/usr/local/include/
all: dominators.so reaching.so landingpad.so licm.so
CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

dataflow.o: dataflow.cpp dataflow.h
reaching.o: reaching.cpp reaching.h
dominators.o: dominators.cpp dominators.h
landingpad.o: landingpad.cpp landingpad.h	
%.so: %.o dataflow.o
	$(CXX) -dylib -shared $^ -o $@


run-dom-test: all
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/dominator_test.c -o ./tests/dominator_test.bc
	opt -mem2reg ./tests/dominator_test.bc -o ./tests/dominator_test_m2r.bc
	llvm-dis ./tests/dominator_test_m2r.bc
	opt -enable-new-pm=0 -load ./dominators.so -dominators ./tests/dominator_test_m2r.bc -o ./tests/dominator_test_out.bc
	llvm-dis ./tests/dominator_test_out.bc
		
run-licm-test1:  all
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/licm_test1.c -o ./tests/licm_test1.bc
	opt -mem2reg ./tests/licm_test1.bc -o ./tests/licm_test1_m2r.bc
	llvm-dis ./tests/licm_test1_m2r.bc
	opt -enable-new-pm=0 -load ./landingpad.so -landing_pad ./tests/licm_test1_m2r.bc -o ./tests/licm_test1_mod.bc
	llvm-dis ./tests/licm_test1_mod.bc
	#opt -enable-new-pm=0 -load ./reaching.so -load ./dominators.so -load ./licm.so -loop_invariant_code_motion ./tests/licm_test1_m2r.bc -o ./tests/licm_test1_out.bc
	opt -enable-new-pm=0 -load ./reaching.so -load ./dominators.so -load ./licm.so -loop-invariant-code-motion ./tests/licm_test1_mod.bc -o ./tests/licm_test1_out.bc
	llvm-dis ./tests/licm_test1_out.bc
	
run-licm-test2:  all
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/licm_test2.c -o ./tests/licm_test2.bc
	opt -mem2reg ./tests/licm_test2.bc -o ./tests/licm_test2_m2r.bc
	llvm-dis ./tests/licm_test2_m2r.bc
	opt -enable-new-pm=0 -load ./landingpad.so -landing_pad ./tests/licm_test2_m2r.bc -o ./tests/licm_test2_mod.bc
	llvm-dis ./tests/licm_test2_mod.bc
	opt -enable-new-pm=0 -load ./reaching.so -load ./dominators.so -load ./licm.so -loop-invariant-code-motion ./tests/licm_test2_mod.bc -o ./tests/licm_test2_out.bc
	llvm-dis ./tests/licm_test2_out.bc
	
run-licm-test3:  all
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/licm_test3.c -o ./tests/licm_test3.bc
	opt -mem2reg ./tests/licm_test3.bc -o ./tests/licm_test3_m2r.bc
	llvm-dis ./tests/licm_test3_m2r.bc
	opt -enable-new-pm=0 -load ./landingpad.so -landing_pad ./tests/licm_test3_m2r.bc -o ./tests/licm_test3_mod.bc
	llvm-dis ./tests/licm_test3_mod.bc
	opt -enable-new-pm=0 -load ./reaching.so -load ./dominators.so -load ./licm.so -loop-invariant-code-motion ./tests/licm_test3_mod.bc -o ./tests/licm_test3_out.bc
	llvm-dis ./tests/licm_test3_out.bc

clean:
	rm -f *.o *~ *.so

.PHONY: clean all
