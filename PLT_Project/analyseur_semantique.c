#include "util.h"
#include "analyseur_semantique.h"

// an array of all verification functions
void (*verification[SEMVERF_SIZE])() = {
    info_complete,
    valid_index,
    unique_edge,
    existe_path,
    stack_iobal};


// main
void analyseur_semantique() {
    graph_init();
    read_syntree();
    trans_sort(0, trans_num - 1);
    for (int i = 0; i < SEMVERF_SIZE; i++) {
        verification[i]();
    }
    printf("analyse sémantique terminée %d/%d\n", SEMVERF_SIZE, SEMVERF_SIZE);
}


void read_syntree() {
    static int addr[MAX_TREEDEPTH] = {};    // tree reader buffer
    static int d = 0;                       // depth counter
    static int i = 0;                       // stack counter
    static POS blockPOS = NONE;             // this node type
    addr[0] = tokens[0].addr;               // root

    POS nodePOS = NONE;
    nodePOS = tokens[addr[d]].type;
    switch (nodePOS) {
        case AUTOMATE:
            blockPOS = KW_AUTOMATE;
            break;
        case KW_ETATS:
        case KW_INITIAL:
        case KW_FINAL:
            blockPOS = nodePOS;
            break;
        case KW_TRANSITIONS:
            blockPOS = nodePOS;
            break;
        case OBJ:
            if (blockPOS == KW_ETATS) {
                strcpy(etats[etat_num++], tokens[addr[d]].str);
                break;
            } else if (blockPOS == KW_TRANSITIONS) {
                if (tokens[addr[d]].str[1] != '\0') {
                    sprintf(errmsg, "erreur sémantique: n'acceptez que char plutôt qu'une chaîne dans la transition de %d à %d\n",
                            transitions[trans_num - 1].start, transitions[trans_num - 1].end);
                    error();
                }
                if (tokens[addr[d - 1]].type == EDGE) {
                    transitions[trans_num - 1].ch = tokens[addr[d]].str[0];
                } else if (tokens[addr[d - 1]].ln == addr[d]) {
                    transitions[trans_num - 1].stacks[i].op = POP;
                    transitions[trans_num - 1].stacks[i].ch = tokens[addr[d]].str[0];
                } else {
                    transitions[trans_num - 1].stacks[i].op = PUSH;
                    transitions[trans_num - 1].stacks[i].ch = tokens[addr[d]].str[0];
                }
            }
            break;
        case NUM:
            if (blockPOS == KW_AUTOMATE) {
                stack_num = tokens[addr[d]].num;
            } else if (blockPOS == KW_INITIAL) {
                initial = tokens[addr[d]].num;
            } else if (blockPOS == KW_FINAL) {
                final[final_num++] = tokens[addr[d]].num;
            } else if (blockPOS == KW_TRANSITIONS) {
                if (tokens[addr[d - 1]].ln == addr[d]) {
                    transitions[trans_num - 1].start = tokens[addr[d]].num;
                } else {
                    transitions[trans_num - 1].end = tokens[addr[d]].num;
                }
            }
            break;
        case EDGE:
            if (tokens[addr[d - 1]].type == TRAN_UNIT) {
                trans_num += 1;
                i = -1;
            }
            break;
        case EANDS:
            if (tokens[addr[d - 1]].type == TRAN_UNIT) {
                trans_num += 1;
                i = -1;
            }
            break;
        case STACK:
            i += 1;
            if (i >= stack_num) {
                sprintf(errmsg, "erreur sémantique: le numéro de la pile dans la transition de %d à %d ne devrait pas excéder %d\n", 
                transitions[trans_num - 1].start, transitions[trans_num - 1].end, stack_num);
                error();
            }
            break;
        default: // <OBJS>|<NUMS>|<NTON>|<TRAN_UNIT>|<BLOCK>
            break;
    }

    if (tokens[addr[d]].ln == -1 && tokens[addr[d]].rn == -1) {
        d -= 1;
        return;
    }
    if (tokens[addr[d]].ln != -1) {
        addr[d + 1] = tokens[addr[d]].ln;
        d += 1;
        read_syntree();
    }
    if (tokens[addr[d]].rn != -1) {
        addr[d + 1] = tokens[addr[d]].rn;
        d += 1;
        read_syntree();
    }

    d -= 1;
    if (d < 0) {
        printf("l'extraction des informations de l'arbre syntaxique est terminée\n");
    }
}

void print_graphinfo() {
    printf("%s\n", underline);
    printf("[GRAPHE INFO]\n");

    printf("nombre des piles: %d\n", stack_num);

    printf("etats: ");
    for (int i = 0; i < etat_num; i++)
        printf("`%s` ", etats[i]);
    printf("\n");

    printf("initial: %d\n", initial);

    printf("final: ");
    for (int i = 0; i < final_num; i++)
        printf("%d ", final[i]);
    printf("\n");

    printf("transitions:\n");
    for (int i = 0; i < trans_num; i++) {
        printf("de: %2d, à: %2d, char: %c, opérations de pile: ",
               transitions[i].start, transitions[i].end, transitions[i].ch);
        for (int j = 0; j < stack_num; j++)
            printf("(%-4s %2c) ",
                   pos[transitions[i].stacks[j].op], transitions[i].stacks[j].ch);
        
        printf("\n");
    }
}

void trans_sort(int indx1, int indx2) {
    if (indx1 >= indx2)
        return;
    int i = indx1, j = indx2, key = MAX_TRANSNUM + 1;
    trans_copy(key, indx1);
    while (i < j) {
        while (i < j && trans_cmp(key, j) <= 0)
            j--;
        trans_copy(i, j);
        while (i < j && trans_cmp(key, i) >= 0)
            i++;
        trans_copy(j, i);
    }
    trans_copy(i, key);
    trans_sort(indx1, i - 1);
    trans_sort(i + 1, indx2);
}

void trans_swap(int t1, int t2) {
    trans_copy(MAX_TRANSNUM, t1);
    trans_copy(t1, t2);
    trans_copy(t2, MAX_TRANSNUM);
}

void trans_copy(int t1, int t2) {
    transitions[t1].start = transitions[t2].start;
    transitions[t1].end = transitions[t2].end;
    transitions[t1].ch = transitions[t2].ch;
    for (int j = 0; j < stack_num; j++) {
        transitions[t1].stacks[j].op = transitions[t2].stacks[j].op;
        transitions[t1].stacks[j].ch = transitions[t2].stacks[j].ch;
    }
}

int trans_cmp(int t1, int t2) {
    if (transitions[t1].start < transitions[t2].start) {
        return -1;
    } else if (transitions[t1].start > transitions[t2].start) {
        return 1;
    } else if (transitions[t1].end < transitions[t2].end) {
        return -1;
    } else if (transitions[t1].end > transitions[t2].end) {
        return 1;
    } else {
        return 0;
    }
}

void info_complete() {
    if (etat_num == 0) {
        sprintf(errmsg, "erreur sémantique: informations des etats manquantes\n");
        error();
    }
    if (initial == -1) {
        sprintf(errmsg, "erreur sémantique: informations initiales manquantes\n");
        error();
    }
    if (final_num == 0){
        sprintf(errmsg, "erreur sémantique: informations finales manquantes\n");
        error();
    }
    if (trans_num == 0){
        sprintf(errmsg, "erreur sémantique: informations des transitions manquantes\n");
        error();
    }
}

void valid_index() {
    for (int i = 0; i < final_num; i++)
        if (final[i] >= etat_num || final[i] < 0) {
            sprintf(errmsg, "erreur sémantique: index invalide %d en final = [", final[i]);
            for (int j = 0; j <= i; j++)
                sprintf(errmsg + strlen(errmsg), "%d,", final[j]);
            strcat(errmsg, " ... ]\n");
            error();
        }

    for (int i = 0; i < trans_num; i++) {
        if (transitions[i].start >= etat_num || transitions[i].start < 0) {
            sprintf(errmsg, "erreur sémantique: index invalide %d en (%d → %d,`%c`, ... )\n",
                    transitions[i].start, transitions[i].start, transitions[i].end, transitions[i].ch);
            error();
        }
        if (transitions[i].end >= etat_num || transitions[i].end < 0) {
            sprintf(errmsg, "erreur sémantique: index invalide %d en (%d → %d,`%c`, ... )\n",
                    transitions[i].end, transitions[i].start, transitions[i].end, transitions[i].ch);
            error();
        }
    }
}

void unique_edge() {
    for (int i = 1; i < trans_num; i++) {
        if (trans_cmp(i - 1, i) == 0) {
            sprintf(errmsg, "erreur sémantique: redéfinition de transition ");
            sprintf(errmsg + strlen(errmsg), "(%d → %d,`%c`", 
                    transitions[i].start, transitions[i].end, transitions[i].ch);
            for (int j = 0; j < stack_num; j++) {
                if (transitions[i].stacks[j].op == NONE) {
                    sprintf(errmsg + strlen(errmsg), ",()");
                } else if (transitions[i].stacks[j].op == PUSH) {
                    sprintf(errmsg + strlen(errmsg), "(`%c`,→)", transitions[i].stacks[j].ch);
                } else {
                    sprintf(errmsg + strlen(errmsg), "(→,`%c`)", transitions[i].stacks[j].ch);
                }
            }
            sprintf(errmsg + strlen(errmsg), ")");
            sprintf(errmsg + strlen(errmsg), " vs ");
            sprintf(errmsg + strlen(errmsg), "(%d → %d,`%c`", 
                    transitions[i - 1].start, transitions[i - 1].end, transitions[i - 1].ch);
            for (int j = 0; j < stack_num; j++) {
                if (transitions[i - 1].stacks[i - 1].op == NONE) {
                    sprintf(errmsg + strlen(errmsg), ",()");
                } else if (transitions[i - 1].stacks[i - 1].op == PUSH) {
                    sprintf(errmsg + strlen(errmsg), "(`%c`,→)", transitions[i - 1].stacks[i - 1].ch);
                } else {
                    sprintf(errmsg + strlen(errmsg), "(→,`%c`)", transitions[i - 1].stacks[i - 1].ch);
                }
            }
            sprintf(errmsg + strlen(errmsg), ")\n");
            error();
        }
    }
}

void existe_path() {
    bool nodes[MAX_ETATS] = {};
    nodes[initial] = true;
    int rest = etat_num - 1;

    while (true) {
        int prev = rest;
        for (int i = 0; i < etat_num; i++) {
            if (nodes[i] == false)
                continue;
            for (int j = 0; j < trans_num; j++) {
                if (nodes[transitions[j].end] == false && transitions[j].start == i) {
                    nodes[transitions[j].end] = true;
                    rest -= 1;
                }
            }
        }
        if (prev == rest)
            break;
    }

    for (int i = 0; i < final_num; i++) {
        if (nodes[final[i]] == false) {
            sprintf(errmsg, "erreur sémantique: aucun chemin disponible de %d à %d\n",
                    initial, final[i]);
            error();
        }
    }
}

void stack_iobal() {
    int stacks[MAX_STACKNUM] = {};

    for (int i = 0; i < trans_num; i++) {
        for (int j = 0; j < stack_num; j++) {
            if (transitions[i].stacks[j].op == PUSH) {
                stacks[j] |= 1;
            } else if (transitions[i].stacks[j].op == POP) {
                stacks[j] |= 2;
            }
        }
    }

    for (int j = 0; j < stack_num; j++) {
        switch (stacks[j]) {
            case 0:
                sprintf(errmsg, "erreur sémantique: pile %d non utilisé\n", j);
                error();
                break;
            case 1:
                sprintf(errmsg, "erreur sémantique: pile %d nécessite une opération pop\n", j);
                error();
                break;
            case 2:
                sprintf(errmsg, "erreur sémantique: pile %d nécessite une opération push\n", j);
                error();
                break;
        }
    }
}
