#include "util.h"
#include "analyseur_lexical.h"

bool KeyWords[6] = {[0 ... 5] = false};

// main
void analyseur_lexical(char *filename) {
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        sprintf(errmsg, "file cannot open\n");
        error();
    }

    char buf[MAX_FILESIZE] = {};
    int f_size = fread(buf, 1, MAX_FILESIZE, fp);
    int rp = 0, wp = 1;
    int state = 0;
    int line = 1;

    tokens_init();

    while (rp < f_size) {
        FSM_Lex(&state, &rp, &wp, buf, &line);
    }
    tokens[0].num = wp;
    if (rp != f_size) {
        sprintf(errmsg, "code error [NOT EXPECTED]\n");
        error();
    }

    printf("lexical analysis finished\n");

    if (fclose(fp) != 0) {
        sprintf(errmsg, "file \"%s\" cannot be closed\n", filename);
        error();
    }
}


void FSM_Lex(int* state, int* rp, int* wp, char* buf, int* line) {
    switch (*state) {
        case 0:
            if (buf[*rp] == '\r' || buf[*rp] == ' ' || buf[*rp] == '\t') {
                *rp += 1;
            } else if (buf[*rp] == '\n') {
                *line += 1;
                *rp += 1;
            } else if (strncmp(buf + *rp, "//", 2) == 0) {
                *state = 1;
                *rp += 2;
            } else if (strncmp(buf + *rp, "/*", 2) == 0) {
                *state = 2;
                *rp += 2;
            } else if (strncmp(buf + *rp, "Automate", 8) == 0) {
                if (KeyWords[KW_AUTOMATE] == true) {
                    sprintf(errmsg, "lexical error: keyword Automate repitition\n");
                    error();
                }
                tokens[(*wp)++].type = KW_AUTOMATE;
                KeyWords[KW_AUTOMATE] = true;
                *rp += 8;
            } else if (strncmp(buf + *rp, "etats", 5) == 0) {
                if (KeyWords[KW_ETATS] == true) {
                    sprintf(errmsg, "lexical error: keyword etats repitition\n");
                    error();
                }
                tokens[(*wp)++].type = SEPAR;
                tokens[(*wp)++].type = KW_ETATS;
                KeyWords[KW_ETATS] = true;
                *rp += 5;
            } else if (strncmp(buf + *rp, "initial", 7) == 0) {
                if (KeyWords[KW_INITIAL] == true) {
                    sprintf(errmsg, "lexical error: keyword initial repitition\n");
                    error();
                }
                tokens[(*wp)++].type = SEPAR;
                tokens[(*wp)++].type = KW_INITIAL;
                KeyWords[KW_INITIAL] = true;
                *rp += 7;
            } else if (strncmp(buf + *rp, "final", 5) == 0) {
                if (KeyWords[KW_FINAL] == true) {
                    sprintf(errmsg, "lexical error: keyword final repitition\n");
                    error();
                }
                tokens[(*wp)++].type = SEPAR;
                tokens[(*wp)++].type = KW_FINAL;
                KeyWords[KW_FINAL] = true;
                *rp += 5;
            } else if (strncmp(buf + *rp, "transitions", 11) == 0) {
                if (KeyWords[KW_TRANSITIONS] == true) {
                    sprintf(errmsg, "lexical error: keyword transitions repitition\n");
                    error();
                }
                tokens[(*wp)++].type = SEPAR;
                tokens[(*wp)++].type = KW_TRANSITIONS;
                KeyWords[KW_TRANSITIONS] = true;
                *rp += 11;
            } else if (buf[*rp] == '(') {
                tokens[(*wp)++].type = PAREN_L;
                *rp += 1;
            } else if (buf[*rp] == ')') {
                tokens[(*wp)++].type = PAREN_R;
                *rp += 1;
            } else if (buf[*rp] == '[') {
                tokens[(*wp)++].type = BRACKET_L;
                *rp += 1;
            } else if (buf[*rp] == ']') {
                tokens[(*wp)++].type = BRACKET_R;
                *rp += 1;
            } else if (buf[*rp] == '{') {
                tokens[(*wp)++].type = BRACE_L;
                *rp += 1;
            } else if (buf[*rp] == '}') {
                tokens[(*wp)++].type = BRACE_R;
                *rp += 1;
            } else if (strncmp(buf + *rp, "→", 3) == 0) {
                tokens[(*wp)++].type = ARROW;
                *rp += 3;
            } else if (buf[*rp] == '=') {
                tokens[(*wp)++].type = EQUAL;
                *rp += 1;
            } else if (buf[*rp] == ',') {
                tokens[(*wp)++].type = COMMA;
                *rp += 1;
            } else if (buf[*rp] == '\'') {
                tokens[*wp].type = OBJ;
                *state = 3;
                *rp += 1;
            } else if (buf[*rp] == '\"') {
                tokens[*wp].type = OBJ;
                *state = 4;
                *rp += 1;
            } else if (buf[*rp] == '`') {
                tokens[*wp].type = OBJ;
                *state = 5;
                *rp += 1;
            } else if (buf[*rp] >= 48 && buf[*rp] <= 57) {
                tokens[*wp].type = NUM;
                tokens[*wp].num = buf[*rp] - '0';
                *state = 6;
                *rp += 1;
            } else {
                sprintf(errmsg, "lexical error: unknown word \"");
                while (buf[*rp] != '\n' && buf[*rp] != ' ' && buf[*rp] != '\t')
                    sprintf(errmsg + strlen(errmsg), "%c", buf[(*rp)++]);
                sprintf(errmsg + strlen(errmsg), "\" in line %d\n", *line);
                error();
            }
            break;
        case 1:
            if (buf[*rp] == '\n') {
                *state = 0;
                *line += 1;
                *rp += 1;
            } else {
                *rp += 1;
            }
            break;
        case 2:
            if (strncmp(buf + *rp, "*/", 2) == 0) {
                *state = 0;
                *rp += 2;
            } else if (buf[*rp] == '\n') {
                *line += 1;
                *rp += 1;
            } else {
                *rp += 1;
            }
            break;
        case 3:
            if (buf[*rp] == '\'') {
                *state = 0;
                *wp += 1;
                *rp += 1;
            } else {
                strncat(tokens[*wp].str, buf + *rp, 1);
                *rp += 1;
            }
            break;
        case 4:
            if (buf[*rp] == '\"') {
                *state = 0;
                *wp += 1;
                *rp += 1;
            } else {
                strncat(tokens[*wp].str, buf + *rp, 1);
                *rp += 1;
            }
            break;
        case 5:
            if (buf[*rp] == '`') {
                *state = 0;
                *wp += 1;
                *rp += 1;
            } else {
                strncat(tokens[*wp].str, buf + *rp, 1);
                *rp += 1;
            }
            break;
        case 6:
            if (buf[*rp] >= 48 && buf[*rp] <= 57) {
                tokens[*wp].num = tokens[*wp].num * 10 + buf[*rp];
                *rp += 1;
            } else {
                *state = 0;
                *wp += 1;
            }
            break;
    }
}

void print_tokens() {
    printf("%s\n[TOKENS]\n", underline);
    int i = 0;
    while (tokens[i].type != NONE) {
        printf("%s ", pos[tokens[i++].type]);
    }
    printf("\n");
}

