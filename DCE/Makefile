INC=-I/usr/local/include/
all: faintness.so dce.so 

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC -DLLVM_ENABLE_STATS

dataflow.o: dataflow.cpp dataflow.h
faintness.o: faintness.cpp faintness.h	

%.so: %.o dataflow.o
	$(CXX) -dylib -shared $^ -o $@

dce_test1: all
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/dce_test1.c -o ./tests/dce_test1.bc
	llvm-dis ./tests/dce_test1.bc
	opt -enable-new-pm=0 -load ./faintness.so -load ./dce.so -dead-code-elimination ./tests/dce_test1.bc -o ./tests/dce_test1_out.bc
	llvm-dis ./tests/dce_test1_out.bc



dce_test2: all
	clang -Xclang -disable-O0-optnone -O0 -emit-llvm -c ./tests/dce_test2.c -o ./tests/dce_test2.bc
	llvm-dis ./tests/dce_test2.bc
	opt -enable-new-pm=0 -load ./faintness.so -load ./dce.so -dead-code-elimination ./tests/dce_test2.bc -o ./tests/dce_test2_out.bc
	llvm-dis ./tests/dce_test2_out.bc


clean:
	rm -f *.o *~ *.so

.PHONY: clean all
