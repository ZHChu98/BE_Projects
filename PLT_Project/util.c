#include "util.h"

char pos[POS_SIZE][MAX_STRLEN] = {
    "NONE", "KW_AUTOMATE", "KW_ETATS", "KW_INITIAL", "KW_FINAL", "KW_TRANSITIONS",
    "SEPAR", "PAREN_L", "PAREN_R", "BRACKET_L", "BRACKET_R", "BRACE_L", "BRACE_R",
    "ARROW", "EQUAL", "COMMA", "OBJ", "NUM", "OBJS", "NUMS", "NTON", "EDGE",
    "STACK", "EANDS", "TRAN_UNIT", "BLOCK", "AUTOMATE", "ROOT", "PUSH", "POP"};

POS BNF[BNF_SIZE][10] = {
    {OBJ, COMMA, OBJ},                                                          // [0] OBJS
    {OBJS, COMMA, OBJ},                                                         // [1] OBJS
    {NUM, COMMA, NUM},                                                          // [2] NUMS
    {NUMS, COMMA, NUM},                                                         // [3] NUMS
    {NUM, ARROW, NUM},                                                          // [4] NTON
    {NTON, COMMA, OBJ},                                                         // [5] EDGE
    {PAREN_L, ARROW, COMMA, OBJ, PAREN_R},                                      // [6] STACK
    {PAREN_L, OBJ, COMMA, ARROW, PAREN_R},                                      // [7] STACK
    {PAREN_L, PAREN_R},                                                         // [8] STACK
    {EDGE, COMMA, STACK},                                                       // [9] EANDS
    {EANDS, COMMA, STACK},                                                      // [10] EANDS
    {PAREN_L, EDGE, PAREN_R},                                                   // [11] TRAN_UNIT
    {PAREN_L, EANDS, PAREN_R},                                                  // [12] TRAN_UNIT
    {TRAN_UNIT, COMMA, TRAN_UNIT},                                              // [13] TRAN_UNIT
    {KW_ETATS, EQUAL, BRACKET_L, OBJ, BRACKET_R},                               // [14] BLOCK (ETATS)
    {KW_ETATS, EQUAL, BRACKET_L, OBJS, BRACKET_R},                              // [15] BLOCK (ETATS)
    {KW_INITIAL, EQUAL, NUM},                                                   // [16] BLOCK (INITIAL)
    {KW_FINAL, EQUAL, BRACKET_L, NUM, BRACKET_R},                               // [17] BLOCK (FINAL)
    {KW_FINAL, EQUAL, BRACKET_L, NUMS, BRACKET_R},                              // [18] BLOCK (FINAL)
    {KW_TRANSITIONS, EQUAL, BRACKET_L, TRAN_UNIT, BRACKET_R},                   // [19] BLOCK (TRANSITIONS)
    {BLOCK, SEPAR, BLOCK},                                                      // [20] BLOCK
    {KW_AUTOMATE, PAREN_L, NUM, PAREN_R, EQUAL, BRACE_L, SEPAR, BLOCK, BRACE_R} // [21] AUTOMATE
};

Token tokens[MAX_FILESIZE];

Token_B parbuf[PARSER_SIZE];

Transition transitions[MAX_TRANSNUM + 2];

Symbol symtab[MAX_ETATS] = {};

int VM[MAX_VMSIZE] = {};

int VM_size = 0;

Stack stacks[MAX_STACKNUM] = {};

char etats[MAX_ETATS][MAX_STRLEN] = {};

int final[MAX_ETATS] = {
    [0 ... MAX_ETATS - 1] = -1
};

int initial = -1;

int stack_num = 0;

int trans_num = 0;

int etat_num = 0;

int final_num = 0;

char errmsg[1024] = {};

char underline[64] = {[0 ... 63] = '_'};


void tokens_init() {
    for (int i = 0; i < MAX_FILESIZE; i++) {
        tokens[i].type = NONE;
        tokens[i].addr = i;
        tokens[i].ln = tokens[i].rn = -1;
        memset(tokens[i].str, 0, MAX_STRLEN);
        tokens[i].num = 0;
    }
    tokens[0].type = ROOT;
}

void parser_init() {
        for (int i = 0; i < PARSER_SIZE; i++) {
        parbuf[i].type = NONE;
        parbuf[i].addr = -1;
    }
}

void graph_init() {
    for (int i = 0; i < MAX_TRANSNUM; i++) {
        transitions[i].start = transitions[i].end = -1;
        transitions[i].ch = '\0';
        for (int j = 0; j < MAX_STACKNUM; j++) {
            transitions[i].stacks[j].op = NONE;
            transitions[i].stacks[j].ch = '\0';
        }
    }
}

void tokencpy(Token_B *token1, Token *token2) {
    token1->type = token2->type;
    token1->addr = token2->addr;
}

int BNFlen(POS *exp) {
    int len = 0;
    while (exp[len] != NONE && len < 10)
        len++;
    return len;
}

bool BNFequ(Token_B *stream, int p, POS *exp) {
    if (p < 0)
        return false;
    for (int i = 0; i < BNFlen(exp); i++)
        if (stream[p + i].type != exp[i])
            return false;
    
    return true;
}

void stack_push(int num, char key) {
    stacks[num].mem[stacks[num].size++] = key;
}

bool stack_pop(int num, char key) {
    if (stacks[num].size == 0)
        return false;
    if (stacks[num].mem[stacks[num].size - 1] != key)
        return false;
    stacks[num].mem[stacks[num].size - 1] = '\0';
    stacks[num].size--;
    return true;
}

void error() {
    printf("%s\n%s", underline, errmsg);
    exit(1);
}
