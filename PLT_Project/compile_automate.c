#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "analyseur_semantique.h"
#include "util.h"

// Function: build symbol table
void symtab_generator();

// Function: build VM
void VM_generator();

// Function: store VM INFO
void VM_store();

// Function: print VM INFO
void print_VM();


int main(int argc, char* argv[]) {
    bool tokstr  = false;   // print token stream
    bool syntree = false;   // print syntax tree
    bool graph   = false;   // print graph information
    bool vm      = false;   // print vm memory
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-show_tokens") == 0) {
            tokstr = true;
        } else if (strcmp(argv[i], "-show_syntree") == 0) {
            syntree = true;
        } else if (strcmp(argv[i], "-show_graph") == 0) {
            graph = true;
        } else if (strcmp(argv[i], "-show_vm") == 0) {
            vm = true;
        } else if (strcmp(argv[i], "-show_all") == 0) {
            tokstr = syntree = graph = vm = true;
        } else {
            sprintf(errmsg, "erreur de commande: commande inconnue \"%s\"\nest-ce que vous voulez:\n", argv[i]);
            sprintf(errmsg + strlen(errmsg),
                    "\"-show_tokens\", \"-show_syntree\", \"-show_graph\", \"-show_vm\" ou \"show_all\"\n");
            error();
        }
    }

    analyseur_lexical(argv[argc - 1]);
    if (tokstr == true)
        print_tokens();
    
    analyseur_syntaxique();
    if (syntree == true)
        print_syntree();
    
    analyseur_semantique();
    if (graph == true)
        print_graphinfo();
    
    VM_generator();
    if (vm == true)
        print_VM();
    
    VM_store();
    
    return 0;
}


void symtab_generator() {
    int rp = 0, wp = 4 + final_num;
    for (int etat = 0; etat < etat_num; etat++) {
        strcpy(symtab[etat].str, etats[etat]);
        symtab[etat].addr = wp;

        int k = 0;
        while (rp + k < trans_num && transitions[rp + k].start == etat) {
            k += 1;
        }
        VM[wp] = k;
        wp += 1 + (2 + 2 * stack_num) * k;
        rp += k;
    }
}

void VM_generator() {
    symtab_generator();
    VM[0] = stack_num;
    VM[1] = symtab[initial].addr;
    VM[2] = final_num;

    int wp = 3;
    for (int i = 0; i < final_num; i++) {
        VM[wp++] = symtab[final[i]].addr;
    }

    VM[wp++] = etat_num;
    int rp = 0;
    for (int etat = 0; etat < etat_num; etat++) {
        int k = VM[wp++];
        for (int i = 0; i < k; i++) {
            VM[wp++] = transitions[rp + i].ch;
            VM[wp++] = symtab[transitions[rp + i].end].addr;
            for (int j = 0; j < stack_num; j++) {
                VM[wp++] = transitions[rp + i].stacks[j].ch;
                if (transitions[rp + i].stacks[j].op == NONE) {
                    VM[wp++] = 0;
                } else if (transitions[rp + i].stacks[j].op == PUSH) {
                    VM[wp++] = 1;
                } else {
                    VM[wp++] = -1;
                }
            }
        }
        rp += k;
    }
    VM_size = wp;
    printf("VM généré\n");
}

void VM_store() {
    FILE *fp = fopen("symtable.txt", "w");
    if (fp == NULL) {
        sprintf(errmsg, "file \"symtable.txt\" cannot open\n");
        error();
    }
    for (int etat = 0; etat < etat_num; etat++) {
        fprintf(fp, "%s %d\n", symtab[etat].str, symtab[etat].addr);
    }
    if (fclose(fp) != 0) {
        sprintf(errmsg, "file \"symtable.txt\" cannot be closed\n");
        error();
    }

    fp = fopen("VM.csv", "w");
    if (fp == NULL) {
        sprintf(errmsg, "file \"VM.csv\" cannot open\n");
        error();
    }
    for (int i = 0; i < VM_size; i++) {
        fprintf(fp, "%d, ", VM[i]);
    }
    if (fclose(fp) != 0) {
        sprintf(errmsg, "file \"VM.csv\" cannot be closed\n");
        error();
    }
    printf("VM mémorisé\n");
}

void print_VM() {
    printf("%s\n[VM INFO]\n", underline);
    printf("table des symboles: \n");
    for (int etat = 0; etat < etat_num; etat++)
        printf("nom: %6s, adresse: %3d\n", symtab[etat].str, symtab[etat].addr);
    printf("Mémoire de VM: ");
    for (int i = 0; i < VM_size; i++)
        printf("%d ", VM[i]);
    printf("\n");
}

