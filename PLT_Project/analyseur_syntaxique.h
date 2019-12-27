#ifndef ANALYSEUR_SYNTAXIQUE_H
#define ANALYSEUR_SYNTAXIQUE_H

// main
void analyseur_syntaxique();

/**
 * Function: reduce or to shift, accroding to FR(1) analysis
 * Output: if reduce then true, else false
 */
bool parser(int* wp);

// Function: reduce according to BNF (variable wp will not be set to next postion in this function)
void reduce(int* wp, int type);

// Function: write syntactic tree in a csv file
void print_syntree();

#endif