# **Programming Language Theory Project**

## **About**

Implement an automate compiler to read the file and build a virtual machine, then execute the virtual machine. The pipeline consists of lexical analysis, syntactic analysis, semantic analysis, compilation, and execution. The key part is syntactic analysis, where I choose to use *BNF* (Backus Normal Form), build a *FSM* (Finite State Machine), and then apply *LR(0)* parser for syntactic analysis.


## **Build**

    make

## **Run**

    ./compile_automate -options filename  
options: -show_tokens -show_syntree -show_graph -show_vm

    ./Executeur -options filename
options: -debug

## **Note**

There are 10 test files with each of them has a lexical/syntac/semantic error.  
Play with Zpile.txt, Upile.txt, Dpile.txt, or you can design whatever you like.  
