#include "util.h"

// Function: reload VM data
void VM_reload();

// Function: executer VM
void VM_executer(bool debug);


// main
int main(int argc, char* argv[]) {
    if (argc == 1) {
        VM_reload();
        VM_executer(false);
    } else if (strcmp(argv[1], "-debug") == 0) {
        VM_reload();
        VM_executer(true);
    } else {
        sprintf(errmsg, "erreur de commande: commande inconnue\n");
        sprintf(errmsg + strlen(errmsg), "est-ce que vous voulez:\n \"%s\" ou \"%s -debug\"\n", argv[0], argv[0]);
        error();
    }
    return 0;
}

void VM_reload() {
    FILE *fp = fopen("symtable.txt", "r");
    int i;
    // reload symbol table
    if (fp == NULL) {
        sprintf(errmsg, "file \"symtable.txt\" cannot open\n");
        error();
    }
    i = 0;
    while (fscanf(fp, "%s %d\n", symtab[i].str, &(symtab[i].addr)) != EOF) {
        i++;
    }
    if (fclose(fp) != 0) {
        sprintf(errmsg, "file \"symtable.txt\" cannot be closed\n");
        error();
    }
    // reload VM
    fp = fopen("VM.csv", "r");
    if (fp == NULL) {
        sprintf(errmsg, "file \"VM.csv\" cannot open\n");
        error();
    }
    i = 0;
    while (fscanf(fp, "%d, ", VM + i) != EOF) {
        i++;
    }
    if (fclose(fp) != 0) {
        sprintf(errmsg, "file \"VM.csv\" cannot be closed\n");
        error();
    }
    printf("VM reloaded\n");

    stack_num = VM[0];
    initial = VM[1]; // attention, here it means initial etat address, not initial etat index
    final_num = VM[2];
    for (int i = 0; i < final_num; i++)
        final[i] = VM[3 + i]; // attention, here it means final etat address, not final etat index
    etat_num = VM[3 + VM[2]];
}

void VM_executer(bool debug) {
    printf("Donner le mot d'entrée: ");
    char input[1024] = {};
    scanf("%s", input);
    getchar();

    int rp = 0, vmp = initial;
    // debug mode: status before reading input word
    if (debug == true) {
        int etat = 0;
        while (symtab[etat].addr != vmp) {
            etat++;
        }
        printf("  -> État : %-6s ", symtab[etat].str);
        for (int j = 0; j < stack_num; j++) {
            printf("Pile %2d : ", j + 1);
            if (stacks[j].size == 0) {
                printf("%-8s ", "Vide");
            } else {
                printf("%-8s ", stacks[j].mem);
            }
        }
        printf("\n");
    }
    // begin reading input word
    while (input[rp] != '\0') {
        int k = VM[vmp], i = 0;
        while (i < k && input[rp] != VM[vmp + 1 + (2 + 2 * stack_num) * i]) {
            i++;
        }
        if (i == k) {
            // debug mode
            if (debug == true) {
                printf("Erreur : La lettre %c inconnu\n", input[rp]);
            }
            printf("Le mot \"%s\" est refusé !\n", input);
            return;
        }
        for (int j = 0; j < stack_num; j++) {
            if (VM[vmp + 4 + (2 + 2 * stack_num) * i + 2 * j] == 1) {
                stack_push(j, VM[vmp + 3 + (2 + 2 * stack_num) * i + 2 * j]);
            } else if (VM[vmp + 4 + (2 + 2 * stack_num) * i + 2 * j] == -1) {
                if (stack_pop(j, VM[vmp + 3 + (2 + 2 * stack_num) * i + 2 * j]) == false) {
                    // debug mode
                    if (debug == true) {
                        int etat = 0;
                        while (symtab[etat].addr != vmp) {
                            etat++;
                        }
                        printf("%c -> Erreur : Pile %d : ", input[rp], j + 1);
                        if (stacks[j].size == 0) {
                            printf("Vide vs ");
                        } else {
                            printf("%s vs ", stacks[j].mem);
                        }
                        printf("l'opération (\'%c\', →)\n", VM[vmp + 3 + (2 + 2 * stack_num) * i + 2 * j]);
                    }
                    printf("Le mot %s est refusé !\n", input);
                    return;
                }
            }
        }
        // debug mode: after reading each char, print the status
        if (debug == true) {
            int etat = 0;
            while (symtab[etat].addr != vmp) {
                etat++;
            }
            printf("%c -> État : %-6s ", input[rp], symtab[etat].str);
            for (int j = 0; j < stack_num; j++) {
                printf("Pile %2d : ", j + 1);
                if (stacks[j].size == 0) {
                    printf("%-8s ", "Vide");
                } else {
                    printf("%-8s ", stacks[j].mem);
                }
            }
            printf("\n");
        }
        vmp = VM[vmp + 2 + (2 + 2 * stack_num) * i];
        rp++;
    }

    // check whether all stacks are vide at the end
    int vides[MAX_STACKNUM] = {};
    int non_vide = 0;
    for (int j = 0; j < stack_num; j++) {
        if (stacks[j].size != 0) {
            vides[non_vide++] = j;
        }
    }
    if (non_vide != 0) {
        // debug mode
        if (debug == true) {
            if (non_vide == 1) {
                printf("Erreur : Pile %d non vide\n", vides[0]);
            } else {
                printf("Erreur : ");
                for (int i = 0; i < non_vide - 1; i++) {
                    printf("Pile %d, ", vides[i]);
                }
                printf("et Pile %d non vides\n", vides[non_vide - 1]);
            }
        }
        printf("Le mot \"%s\" est refusé !\n", input);
        return;
    }

    // check whether final etat is included in final
    for (int i = 0; i < final_num; i++) {
        if (final[i] == vmp) {
            printf("Le mot %s est accepté !\n", input);
            return;
        }
    }
    // debug mode
    if (debug == true) {
        int etat = 0;
        while (symtab[etat].addr != vmp) {
            etat++;
        }
        printf("  -> Erreur : État %s n'est pas un etat final\n", symtab[etat].str);
    }
    printf("Le mot \"%s\" est refusé !\n", input);   
}
