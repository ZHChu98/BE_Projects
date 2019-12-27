#ifndef ANALYSEUR_SEMANTIQUE_H
#define ANALYSEUR_SEMANTIQUE_H

// main
void analyseur_semantique();

// Function: extract information from the syntax tree stored in tokens
void read_syntree();

// Function: print extracted file information at terminal
void print_graphinfo();

// Function: sort transitions by index from smallest to largest
void trans_sort(int start, int end);

// Function: swap two transitions with two given index
void trans_swap(int t1, int t2);

// Function: copy transition from index t2 to index t1
void trans_copy(int t1, int t2);

/**
 * Function: whether transitions[t1] should be posed in front of transitions[t2]
 * Output: if "equal" then 0 else if transitions[t1] smaller than transitions[t2] then -1 else 1
 */
int trans_cmp(int t1, int t2);

// Function: check whether all basic information has been given
void info_complete();

// Function: check whether all index are valid
void valid_index();

// Function: check whether there are multiple repeated edge
void unique_edge();

// Function: check whether there is a possible path from start vertice to end vertice
void existe_path();

// Function: check whether each stack contens both push and pop operations
void stack_iobal();

#endif