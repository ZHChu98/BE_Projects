# **Programming Language Theory Project**

Project for **Programming Language Theory**

## **Project Description**

The goal of this project is to implement an automate compiler to read the file and build a virtual machine, then execute the virtual machine.

## **Project Structure**

1. Design implement Finite State Machine (FSM) for lexical analysis
2. Use *BNF* as the regular expression and use *FSM* to realize syntactic analysis in *LR(0)* manner
3. Check semantic error, and then compile and execute the code

## **Usage**

### **Build**

    make

### **Run**

    ./compile_automate -options filename

options: -show_tokens -show_syntree -show_graph -show_vm

    ./Executeur -options filename

options: -debug

### **Note**

There are 10 test files with each of them has a lexical/syntac/semantic error.  
You can play with Zpile.txt, Upile.txt, Dpile.txt, or you can design whatever you like.  
