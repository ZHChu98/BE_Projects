#ifndef ANALYSEUR_LEXICAL_H
#define ANALYSEUR_LEXICAL_H

// main
void analyseur_lexical(char *filename);

/**
 * Function: transform a string stream into a token stream
 * Input: string stream (buf)
 * Output: token stream  (tokens)
 */
void FSM_Lex(int *state, int *rp, int *wp, char *buf, int *line);

// Function: print tokens
void print_tokens();

#endif