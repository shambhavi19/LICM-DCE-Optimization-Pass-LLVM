
-----------------------------------------------------------
Instructions:
Copy the folders in Transfroms folder in llvm-project/llvm/lib/


-----------------------------------------------------------
TO TEST THE LICM AND DOMINATORS PASS
-----------------------------------------------------------
Follow the instructions given below:
1. cd Transforms
3. cd LICM
4. make clean
5. make
6. make run-dom-test  - This is to test the Dominator Pass
7. make run-licm-test1 - This will run the licm-test1.c
8. make run-licm-test2 - This will run the licm-test2.c
9. make run-licm-test3 - This will run the licm-test3.c
------------------------------------------------------------
TO TEST THE DCE PASS
------------------------------------------------------------
Follow the instructions given below:
1. cd Transforms
3. cd DCE
4. make clean
5. make
6. make dce_test1 - This will run the dce_test1.c
7. make dce_test2 - This will run the dce_test2.c
------------------------------------------------------------
