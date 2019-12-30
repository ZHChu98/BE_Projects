#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_FILESIZE    4096    // accepted maximum file size
#define MAX_STRLEN      20      // all strings' maximum length in this project
#define POS_SIZE        30      // number of POS
#define BNF_SIZE        22      // number of BNF
#define PARSER_SIZE     64      // parser buffer maximum size
#define MAX_TREEDEPTH   64      // maximum syntactic tree depth
#define MAX_STACKNUM    32      // maximum number of stack stored
#define MAX_TRANSNUM    64      // maximum transitions stored
#define MAX_ETATS       32      // maximum "etats" stored
#define SEMVERF_SIZE    5       // number of semantic verifications
#define MAX_VMSIZE      1024    // virtual machine size
#define MAX_STACKSIZE   64      // maximum stack depth

#define print_arbre print_syntree


// transform POS into string in order to print
extern char pos[POS_SIZE][MAX_STRLEN];

typedef enum
{
    // PRI POS
    NONE,           // no type
    KW_AUTOMATE,    // Keyword Automate
    KW_ETATS,       // Keyword etats
    KW_INITIAL,     // Keyword initial
    KW_FINAL,       // Keyword final
    KW_TRANSITIONS, // Keyword transitions
    SEPAR,          // before Keywords
    PAREN_L,        // ( {40}
    PAREN_R,        // ) {41}
    BRACKET_L,      // [ {91}
    BRACKET_R,      // ] {93}
    BRACE_L,        // { {123}
    BRACE_R,        // } {125}
    ARROW,          // → {-30, -122, -110}
    EQUAL,          // = {61}
    COMMA,          // , {44}
    OBJ,            // string
    NUM,            // number
    // DEV POS
    OBJS,      // (<OBJ>|<OBJS>) <COMMA> <OBJ>
    NUMS,      // (<NUM>|<NUMS>) <COMMA> <NUM>
    NTON,      // <NUM> <ARROW> <NUM>
    EDGE,      // <NTON> <COMMA> <OBJ>
    STACK,     // <PAREN_L> <ARROW> <COMMA> <OBJ> <PAREN_R>
               // <PAREN_L> <OBJ> <COMMA> <ARROW> <PAREN_R>
               // <PAREN_L> <COMMA> <PAREN_R>
    EANDS,     // (<EDGE>|<EANDS>) <COMMA> <STACK>
    TRAN_UNIT, // <PAREN_L> (<EDGE>|<EANDS>) <PAREN_R>
               // <TRAN_UNIT> <COMMA> <TRAN_UNIT>
    BLOCK,     // <KW_ETATS> <EQUAL> <BRACKET_L> <OBJ>|<OBJS> <BRACKET_R>
               // <KW_INITIAL> <EQUAL> <NUM>
               // <KW_FINAL> <EQUAL> <BRACKET_L> <NUM>|<NUMS> <BRACKET_R>
               // <KW_TRANSITIONS> <EQUAL> <BRACKET_L> <TRAN_UNIT> <BRACKET_R>
               // <BLOCK> <BLOCK>
    AUTOMATE,  // <KW_AUTOMATE> <PAREN_L> <NUM> <PAREN_R> <EQUAL> <BRACE_L> <BLOCK> <BRACE_R>
    ROOT,      // point to <AUTOMATE>
    // OTHER POS
    PUSH, // arrow in <STACK> indicating push operation
    POP   // arrow in <STACK> indicating pop operation
} POS;

// BNF grammar
extern POS BNF[BNF_SIZE][10];

typedef struct {
    POS type;   // TOKEN type
    int addr;   // this node index
    int ln;     // left node index
    int rn;     // right node index
    // data
    char str[MAX_STRLEN]; // <OBJ>
    int num;              // <NUM>
} Token;

// token stream buffer
extern Token tokens[MAX_FILESIZE];

typedef struct {
    POS type;   // TOKEN type
    int addr;   // this node index
} Token_B;

// parser buffer
extern Token_B parbuf[PARSER_SIZE];

typedef struct {
    int start;  // start index
    int end;    // end index
    char ch;    // <OBJ>
    struct {
        POS op;             // <POP>|<PUSH>
        char ch;            // <OBJ>
    } stacks[MAX_STACKNUM]; // operation for all stacks
} Transition;

/**
 * an assemble of all transition information
 * transitions[MAX_TRANSNUM] used for trans_copy
 * transitions[MAX_TRANSNUM + 1] used for trans_sort
 */
extern Transition transitions[MAX_TRANSNUM + 2];

typedef struct {
    char str[MAX_STRLEN];   // etat name
    int addr;               // etat address
} Symbol;

// symbol table
extern Symbol symtab[MAX_ETATS];

// virtual machine
extern int VM[MAX_VMSIZE];

// virtual machine size
extern int VM_size;

typedef struct {
    char mem[MAX_STACKSIZE];    // stack memory
    int size;                   // current stack size
} Stack;

extern Stack stacks[MAX_STACKNUM];

// each nodes' name
extern char etats[MAX_ETATS][MAX_STRLEN];

// final node index
extern int final[MAX_ETATS];

// initial node index
extern int initial;

// number of stack
extern int stack_num;

// number of transitions
extern int trans_num;

// number of nodes
extern int etat_num;

// number of end nodes
extern int final_num;

// error message
extern char errmsg[1024];

// underline
extern char underline[64];


// Function: initiate token memory
void tokens_init();

// Function: initiate parser buffer
void parser_init();

// Function: initiate graph data memory
void graph_init();

// Function: copy token (note: not copy everything inside TOKEN)
void tokencpy(Token_B *token1, Token *token2);

// Function: return BNF[i] length
int BNFlen(POS *exp);

// Function: match token stream with BNF[i]
bool BNFequ(Token_B *stream, int p, POS *exp);

// Function: push operation of stack[num]
void stack_push(int num, char key);

// Function: pop operation of stack[num] and verify the element poped out
bool stack_pop(int num, char key);

// Function: print error message and exit program
void error();

#endif