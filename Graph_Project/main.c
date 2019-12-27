#include "func.c"

int main()
{
    // NUM_NODE is defined in file func.h
    int Mat[NUM_NODE][NUM_NODE] = {};
    
    // generate_graph(Mat, 0.7, time(NULL)); // way 1
    // input_graph(Mat); // way 2
    read_graph(Mat); // way 3

    print_graph(Mat);

    clock_t start, end;

    int node_color[NUM_NODE + 2] = {};
    
    // exhaustion algorithm
    for (int i = 0; i < NUM_NODE + 2; i++)
        node_color[i] = 0;
    start = clock();
    exhaustion(Mat, node_color);
    end = clock();
    printf("<exhaustion algorithm>\n");
    printf("chromatic number: %d\n", node_color[NUM_NODE + 1]);
    printf("coloration: \n");
    for (int i = 1; i <= NUM_NODE; i++)
        printf("%d ", node_color[i]);
    printf("\ntime: %.3f s\n\n", (double)(end - start) / CLK_TCK);
    
    // Zykov algorithm (contraction algorithm)
    for (int i = 0; i < NUM_NODE + 2; i++)
        node_color[i] = 0;
    node_color[NUM_NODE] = NUM_NODE;
    start = clock();
    Zykov(Mat, node_color);
    end = clock();
    printf("<Zykov algorithm>\n");
    printf("chromatic number: %d\n", node_color[NUM_NODE + 1]);
    printf("coloration: \n");
    for (int i = 0; i < NUM_NODE; i++)
        printf("%d ", node_color[i]);
    printf("\ntime: %.3f s\n\n", (double)(end - start) / CLK_TCK);

    // Leighton
    for (int i = 0; i < NUM_NODE + 2; i++)
        node_color[i] = 0;
    node_color[NUM_NODE] = NUM_NODE;
    start = clock();
    Leighton(Mat, node_color);
    end = clock();
    printf("<Leighton algorithm>\n");
    printf("chromatic number: %d\n", node_color[NUM_NODE + 1]);
    printf("coloration: \n");
    for (int i = 0; i < NUM_NODE; i++)
        printf("%d ", node_color[i]);
    printf("\ntime: %.3f s\n\n", (double)(end - start) / CLK_TCK);

    // Brelaz
    for (int i = 0; i < NUM_NODE + 2; i++)
        node_color[i] = 0;
    node_color[NUM_NODE] = NUM_NODE;
    start = clock();
    Brelaz(Mat, node_color);
    end = clock();
    printf("<Brelaz algorithm>\n");
    printf("chromatic number: %d\n", node_color[NUM_NODE + 1]);
    printf("coloration: \n");
    for (int i = 0; i < NUM_NODE; i++)
        printf("%d ", node_color[i]);
    printf("\ntime: %.3f s\n\n", (double)(end - start) / CLK_TCK);
    
    getchar();
    return 0;
}