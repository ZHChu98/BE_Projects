#include "util.h"
#include "analyseur_syntaxique.h"

// main
void analyseur_syntaxique() {
    parser_init();
    int rp = 1, wp = 0;
    while (rp < tokens[0].num) {
        tokencpy(parbuf + wp, tokens + rp);
        rp++;
        while (parser(&wp))
            ;
    }
    if (wp != 1) {
        sprintf(errmsg, "erreur syntaxique\n[LEXEMES RESTANTS] ");
        for (int i = 0; i < wp; i++)
            sprintf(errmsg + strlen(errmsg), "%s ", pos[parbuf[i].type]);
        strcat(errmsg, "\n");
        error();
    } else {
        printf("analyse syntaxique terminée\n");
    }
}


bool parser(int* wp) {
    for (int i = 0; i < BNF_SIZE; i++) {
        if (BNFequ(parbuf, *wp - BNFlen(BNF[i]) + 1, BNF[i]) == true) {
            reduce(wp, i);
            return true;
        }
    }
    *wp += 1;
    return false;
}

void reduce(int* wp, int type) {
    switch (type) {
        case 0:
        case 1:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = OBJS;
            tokens[parbuf[*wp - 1].addr].ln = parbuf[*wp - 2].addr;
            tokens[parbuf[*wp - 1].addr].rn = parbuf[*wp].addr;
            // reduce
            parbuf[*wp - 2].type = OBJS;
            parbuf[*wp - 2].addr = parbuf[*wp - 1].addr;
            *wp -= 2;
            break;
        case 2:
        case 3:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = NUMS;
            tokens[parbuf[*wp - 1].addr].ln = parbuf[*wp - 2].addr;
            tokens[parbuf[*wp - 1].addr].rn = parbuf[*wp].addr;
            // reduce
            parbuf[*wp - 2].type = NUMS;
            parbuf[*wp - 2].addr = parbuf[*wp - 1].addr;
            *wp -= 2;
            break;
        case 4:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = NTON;
            tokens[parbuf[*wp - 1].addr].ln = parbuf[*wp - 2].addr;
            tokens[parbuf[*wp - 1].addr].rn = parbuf[*wp].addr;
            // reduce
            parbuf[*wp - 2].type = NTON;
            parbuf[*wp - 2].addr = parbuf[*wp - 1].addr;
            *wp -= 2;
            break;
        case 5:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = EDGE;
            tokens[parbuf[*wp - 1].addr].ln = parbuf[*wp - 2].addr;
            tokens[parbuf[*wp - 1].addr].rn = parbuf[*wp].addr;
            // reduce
            parbuf[*wp - 2].type = EDGE;
            parbuf[*wp - 2].addr = parbuf[*wp - 1].addr;
            *wp -= 2;
            break;
        case 6:
        case 7:
            // build dependency
            tokens[parbuf[*wp - 2].addr].type = STACK;
            tokens[parbuf[*wp - 2].addr].ln = parbuf[*wp - 3].addr;
            tokens[parbuf[*wp - 2].addr].rn = parbuf[*wp - 1].addr;
            // reduce
            parbuf[*wp - 4].type = STACK;
            parbuf[*wp - 4].addr = parbuf[*wp - 2].addr;
            *wp -= 4;
            break;
        case 8:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = STACK;
            // reduce
            parbuf[*wp - 1].type = STACK;
            *wp -= 1;
            break;
        case 9:
        case 10:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = EANDS;
            tokens[parbuf[*wp - 1].addr].ln = parbuf[*wp - 2].addr;
            tokens[parbuf[*wp - 1].addr].rn = parbuf[*wp].addr;
            // reduce
            parbuf[*wp - 2].type = EANDS;
            parbuf[*wp - 2].addr = parbuf[*wp - 1].addr;
            *wp -= 2;
            break;
        case 11:
        case 12:
            // build dependency
            tokens[parbuf[*wp - 2].addr].type = TRAN_UNIT;
            tokens[parbuf[*wp - 2].addr].ln = parbuf[*wp - 1].addr;
            // reduce
            parbuf[*wp - 2].type = TRAN_UNIT;
            *wp -= 2;
            break;
        case 13:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = TRAN_UNIT;
            tokens[parbuf[*wp - 1].addr].ln = parbuf[*wp - 2].addr;
            tokens[parbuf[*wp - 1].addr].rn = parbuf[*wp].addr;
            // reduce
            parbuf[*wp - 2].type = TRAN_UNIT;
            parbuf[*wp - 2].addr = parbuf[*wp - 1].addr;
            *wp -= 2;
            break;
        case 14:
        case 15:
            // build dependency
            tokens[parbuf[*wp - 3].addr].type = BLOCK;
            tokens[parbuf[*wp - 3].addr].ln = parbuf[*wp - 4].addr;
            tokens[parbuf[*wp - 3].addr].rn = parbuf[*wp - 1].addr;
            // reduce
            parbuf[*wp - 4].type = BLOCK;
            parbuf[*wp - 4].addr = parbuf[*wp - 3].addr;
            *wp -= 4;
            break;
        case 16:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = BLOCK;
            tokens[parbuf[*wp - 1].addr].ln = parbuf[*wp - 2].addr;
            tokens[parbuf[*wp - 1].addr].rn = parbuf[*wp].addr;
            // reduce
            parbuf[*wp - 2].type = BLOCK;
            parbuf[*wp - 2].addr = parbuf[*wp - 1].addr;
            *wp -= 2;
            break;
        case 17:
        case 18:
            // build dependency
            tokens[parbuf[*wp - 3].addr].type = BLOCK;
            tokens[parbuf[*wp - 3].addr].ln = parbuf[*wp - 4].addr;
            tokens[parbuf[*wp - 3].addr].rn = parbuf[*wp - 1].addr;
            // reduce
            parbuf[*wp - 4].type = BLOCK;
            parbuf[*wp - 4].addr = parbuf[*wp - 3].addr;
            *wp -= 4;
            break;
        case 19:
            // build dependency
            tokens[parbuf[*wp - 3].addr].type = BLOCK;
            tokens[parbuf[*wp - 3].addr].ln = parbuf[*wp - 4].addr;
            tokens[parbuf[*wp - 3].addr].rn = parbuf[*wp - 1].addr;
            // reduce
            parbuf[*wp - 4].type = BLOCK;
            parbuf[*wp - 4].addr = parbuf[*wp - 3].addr;
            *wp -= 4;
            break;
        case 20:
            // build dependency
            tokens[parbuf[*wp - 1].addr].type = BLOCK;
            tokens[parbuf[*wp - 1].addr].ln = parbuf[*wp - 2].addr;
            tokens[parbuf[*wp - 1].addr].rn = parbuf[*wp].addr;
            // reduce
            parbuf[*wp - 2].type = BLOCK;
            parbuf[*wp - 2].addr = parbuf[*wp - 1].addr;
            *wp -= 2;
            break;
        case 21:
            // build dependency
            tokens[parbuf[*wp - 4].addr].type = AUTOMATE;
            tokens[parbuf[*wp - 4].addr].ln = parbuf[*wp - 6].addr;
            tokens[parbuf[*wp - 4].addr].rn = parbuf[*wp - 1].addr;
            // reduce
            parbuf[*wp - 8].type = AUTOMATE;
            parbuf[*wp - 8].addr = parbuf[*wp - 4].addr;
            *wp -= 8;
            break;
        default:
            sprintf(errmsg, "code error [NOT EXCEPTED]\n");
            error();
    }
    if (*wp == 0) {
        tokens[0].addr = parbuf[0].addr;
    }
}

void print_syntree() {
    static FILE *fp = NULL;                 // writing file
    static int addr[MAX_TREEDEPTH] = {};    // tree reader buffer
    static int d = 0;                       // depth counter
    addr[0] = tokens[0].addr;               // root

    if (fp == NULL && (fp = fopen("syntree.csv", "w")) == NULL) {
        sprintf(errmsg, "file \"%s\" cannot open\n", "syntree.csv");
        error();
    }

    char nodePOS[MAX_STRLEN] = {};
    strncpy(nodePOS, pos[tokens[addr[d]].type], MAX_STRLEN);
    if (tokens[addr[d]].type == NUM) {
        fprintf(fp, "%s %d,", nodePOS, tokens[addr[d]].num);
    } else if (tokens[addr[d]].type == OBJ) {
        fprintf(fp, "%s %s,", nodePOS, tokens[addr[d]].str);
    } else {
        fprintf(fp, "%s,", nodePOS);
    }

    if (tokens[addr[d]].ln == -1 && tokens[addr[d]].rn == -1) {
        d -= 1;
        fprintf(fp, "\n");
        return;
    }

    if (tokens[addr[d]].ln != -1) {
        addr[d + 1] = tokens[addr[d]].ln;
        d += 1;
        print_syntree("syntree.csv");
    }
    if (tokens[addr[d]].rn != -1) {
        for (int i = 0; i <= d; i++) {
            fprintf(fp, ",");
        }
        addr[d + 1] = tokens[addr[d]].rn;
        d += 1;
        print_syntree("syntree.csv");
    }

    d -= 1;
    if (d < 0) {
        if (fclose(fp) != 0) {
            sprintf(errmsg, "file \"syntree.csv\" cannot be closed\n");
            error();
        }
        printf("%s\narbre syntaxique disponible sur \"syntree.csv\"\n", underline);
    }
}
