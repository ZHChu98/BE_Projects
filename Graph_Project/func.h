#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUM_NODE 10 // number of node

/**
 * print matrix
 */
void print_graph(int Mat[][NUM_NODE]);

/**
 * generate graph
 * you can set seed as you wish, or just time(NULL)
 **/ 
void generate_graph(int Mat[][NUM_NODE], double prob, unsigned int seed);

/**
 * read the file storing the graph
 * file name is "graph.txt"
 * nodes should be named by a series of consecutive characters, started with 'A'
 */
void read_graph(int Mat[][NUM_NODE]);

/**
 * manual input
 * element number must match matrix size
 */
void input_graph(int Mat[][NUM_NODE]);

/**
 * <exhaustion algorithm>
 * node_color[1:NUM_NODE] stroes the coloration
 * node_color[0] has no meaning but is necessary for function
 * node_color[NUM_NODE+1] stores the chromatic number
 **/
void exhaustion(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2]);
void exhaustion_k(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2]); // do not call this function 

/**
 * <Zykov>
 * node_color[0:NUM_NODE-1] stores the coloration
 * node_color[NUM_NODE] is the size of graph which should be initialized with NUM_NODE
 * node_color[NUM_NODE+1] stores the chromatic number
 **/
void Zykov(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2]);

/**
 * <Leighton>
 * node_color[0:NUM_NODE-1] stores the coloration
 * node_color[NUM_NODE] is the size of graph which should be initialized with NUM_NODE
 * node_color[NUM_NODE+1] stores the chromatic number
 */
void Leighton(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2]);

/**
 * <Brelaz>
 * node_color[0:NUM_NODE-1] stores the coloration
 * node_color[NUM_NODE] is the size of graph which should be initialized with NUM_NODE
 * node_color[NUM_NODE+1] stores the chromatic number
 */
void Brelaz(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2]);

#endif
