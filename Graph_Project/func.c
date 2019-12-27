#include "func.h"

void print_graph(int Mat[][NUM_NODE])
{
    printf("<matrix>\n");
    for (int i = 0; i < NUM_NODE; i++) {
        for (int j = 0; j < NUM_NODE; j++)
            printf("%d ", Mat[i][j]);
        printf("\n");
    }
    printf("\n");
}


void generate_graph(int Mat[][NUM_NODE], double prob, unsigned int seed)
{
    printf("seed %d\n", seed);
    srand(seed);
    for (int i = 0; i < NUM_NODE; i++) {
        Mat[i][i] = 0;
        for (int j = i + 1; j < NUM_NODE; j++) {
            if ((rand() % 1000) < 1000 * prob)
                Mat[i][j] = Mat[j][i] = 1;
            else
                Mat[i][j] = Mat[j][i] = 0;
        }
    }
    printf("generating graph finished\n\n");
}

void input_graph(int Mat[][NUM_NODE])
{
    printf("matrix size: %d * %d\n", NUM_NODE, NUM_NODE);
    for (int i = 0; i < NUM_NODE; i++)
        for (int j = 0; j < NUM_NODE; j++)
            scanf("%d", &Mat[i][j]);
    getchar();
    printf("input finished\n\n");
}

void read_graph(int Mat[][NUM_NODE])
{
    FILE *fp = NULL;
    fp = fopen("graph.txt", "r");
    if (fp ==  NULL) {
        printf("failed open graph.txt\n");
        return;
    }
    char a = '\0', b = '\0', c = '\0';
    while (c != EOF) {
        a = getc(fp);
        getc(fp);
        b = getc(fp);
        c = getc(fp);
        Mat[a - 'A'][b - 'A'] = 1;
        Mat[b - 'A'][a - 'A'] = 1;
    }
    fclose(fp);
    printf("reading graph finished\n\n");
}


void exhaustion(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2])
{
    node_color[NUM_NODE + 1] = NUM_NODE;
    while (true) {
        exhaustion_k(Mat, node_color);
        if (node_color[NUM_NODE + 1] <= 0) {
            node_color[NUM_NODE + 1] += NUM_NODE;
            break;
        }
    }
}

void exhaustion_k(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2])
{
    int new_node_color[NUM_NODE + 2] = {};
    new_node_color[0] = 0;
    for (int i = 1; i <= NUM_NODE; i++)
        new_node_color[i] = 1;
    int num_color = node_color[NUM_NODE + 1];

    while (true) {
        for (int i = NUM_NODE; i > 0; i--)
            if (new_node_color[i] > num_color) {
                new_node_color[i] -= num_color;
                new_node_color[i - 1]++;
            }
            else
                break;
        
        if (new_node_color[0] == 1)
            break;
        
        bool legal = true;
        for (int i = 0; legal == true && i < NUM_NODE; i++)
            for (int j = i + 1; legal == true && j < NUM_NODE; j++)
                if (Mat[i][j] == 1 && new_node_color[i + 1] == new_node_color[j + 1])
                    legal = false;
        if (legal == true) {
            for (int i = 1; i <= NUM_NODE; i++)
                node_color[i] = new_node_color[i];
            node_color[NUM_NODE + 1]--;
            return;
        }
        new_node_color[NUM_NODE]++;
    }
    node_color[NUM_NODE + 1] -= (NUM_NODE - 1);
}


void Zykov(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2])
{
    int size = node_color[NUM_NODE];
    bool isComplete = true;
    int a = -1, b = -1;
    for (int i = 0; isComplete == true && i < size - 1; i++) {
        for (int j = i + 1; isComplete == true && j < size; j++) {
            if (Mat[i][j] == 0) {
                isComplete = false;
                a = i;
                b = j;
            }
        }
    }
    if (isComplete == true) {
        for (int i = 0; i < size; i++)
            node_color[i] = i + 1;
        node_color[NUM_NODE + 1] = size;
        return;
    }
    
    int new_Mat1[NUM_NODE][NUM_NODE] = {};
    int new_Mat2[NUM_NODE][NUM_NODE] = {};
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            new_Mat1[i][j] = Mat[i][j];
            new_Mat2[i][j] = Mat[i][j];
        }
    new_Mat1[a][b] = 1;
    new_Mat1[b][a] = 1;

    int node_color1[NUM_NODE + 2] = {};
    node_color1[NUM_NODE] = size;
    Zykov(new_Mat1, node_color1);

    for (int i = 0; i < size; i++) {
        if (new_Mat2[a][i] == 1 || new_Mat2[b][i] == 1) {
            new_Mat2[a][i] = 1;
            new_Mat2[i][a] = 1;
        }
    }
    for (int i = b; i < size - 1; i++)
        for (int j = 0; j < size; j++)
            new_Mat2[i][j] = new_Mat2[i + 1][j];
    for (int i = b; i < size - 1; i++)
        for (int j = 0; j < size; j++)
            new_Mat2[j][i] = new_Mat2[j][i + 1];

    int node_color2[NUM_NODE + 2] = {};
    node_color2[NUM_NODE] = size - 1;
    Zykov(new_Mat2, node_color2);

    if (node_color1[NUM_NODE + 1] < node_color2[NUM_NODE + 1]) {
        for (int i = 0; i < size; i++)
            node_color[i] = node_color1[i];
        node_color[NUM_NODE + 1] = node_color1[NUM_NODE + 1];
        return;
    }
    else {
        for (int i = 0; i < b; i++)
            node_color[i] = node_color2[i];
        node_color[b] = node_color[a];
        for (int i = b + 1; i < size; i++)
            node_color[i] = node_color2[i - 1];
        node_color[NUM_NODE + 1] = node_color2[NUM_NODE + 1];
        return;
    }
}


void Leighton(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2])
{
    int size = node_color[NUM_NODE];
    bool isComplete = true;
    int node_degre[NUM_NODE + 1] = {};
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            node_degre[i] += Mat[i][j];
            if (Mat[i][j] == 0 && i != j)
                isComplete = false;
        }
    
    if (isComplete == true) {
        for (int i = 0; i < size; i++)
            node_color[i] = i + 1;
        node_color[NUM_NODE + 1] = size;
        return;
    }

    int a = NUM_NODE, b = -1;
    for (int i = 0; i < size; i++)
        if (node_color[i] == 0 && node_degre[i] > node_degre[a] && node_degre[i] != size - 1)
            a = i;
    
    for (int i = 0; i < size; i++)
        if (i != a && Mat[a][i] == 0) {
            b = i;
            break;
        }
    
    int new_Mat[NUM_NODE][NUM_NODE] = {};
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            new_Mat[i][j] = Mat[i][j];
    for (int i = 0; i < size; i++) {
        if (new_Mat[a][i] == 1 || new_Mat[b][i] == 1) {
            new_Mat[a][i] = 1;
            new_Mat[i][a] = 1;
        }
    }        
    for (int i = b; i < size - 1; i++)
        for (int j = 0; j < size; j++)
            new_Mat[i][j] = new_Mat[i + 1][j];
    for (int i = b; i < size - 1; i++)
        for (int j = 0; j < size; j++)
            new_Mat[j][i] = new_Mat[j][i + 1];
    
    int new_node_color[NUM_NODE + 2] = {};
    new_node_color[NUM_NODE] = size - 1;
    Leighton(new_Mat, new_node_color);

    for (int i = 0; i < b; i++)
        node_color[i] = new_node_color[i];
    for (int i = b + 1; i < size; i++)
        node_color[i] = new_node_color[i - 1];
    node_color[b] = node_color[a];
    node_color[NUM_NODE + 1] = new_node_color[NUM_NODE + 1];
    return;
}


void Brelaz(const int Mat[][NUM_NODE], int node_color[NUM_NODE + 2])
{
    int size = node_color[NUM_NODE];
    int color = 1;
    int node_degre[NUM_NODE + 1] = {};
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            node_degre[i] += Mat[i][j];

    for (int k = 0; k < size; k++) {
        int node_b[NUM_NODE + 1] = {};
        for (int i = 0; i < size; i++) {
            int tmp_color = 0;
            int tmp[NUM_NODE] = {};
            for (int j = 0; j < size; j++) {
                if (Mat[i][j] == 1 && node_color[j] != 0) {
                    bool exist = false;
                    for (int r = 0; r < tmp_color; r++)
                        if (node_color[j] == tmp[r]) {
                            exist = true;
                            break;
                        }
                    if (exist == false) {
                        tmp[tmp_color] = node_color[j];
                        tmp_color++;
                    }
                }
            }
            node_b[i] = tmp_color;
        }
        int next = NUM_NODE;
        for (int i = 0; i < size; i++)
            if (node_color[i] == 0) {
                if (node_degre[i] > node_degre[next])
                    next = i;
                else if (node_degre[i] == node_degre[next] && node_b[i] > node_b[next])
                    next = i;
            }

        int try_color = 1;
        for (try_color = 1; try_color <= color; try_color++) {
            bool legal = true;
            for (int j = 0; j < size; j++)
                if (Mat[next][j] == 1 && node_color[j] == try_color)
                    legal = false;
            if (legal == true)
                break;
        }
        node_color[next] = try_color;
        if (try_color == color + 1)
            color++;
    }
    node_color[NUM_NODE + 1] = color;
}