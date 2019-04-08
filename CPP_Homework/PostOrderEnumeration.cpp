/* Since I have tried solving problems with tree structure before
 * This time I am trying to propose a solution with little code as well as fast speed
 */
#include <iostream>
#include <stdio.h>

using namespace std;

#define max_size 50

int find(char *assistArray, char tok)
{
    int start = 0, ref = 0;
    while (assistArray[start] == '\0' && start < max_size)
        start++;
    while (assistArray[start + ref] != tok && ref < max_size - start)
        ref++;
    if (start + ref < max_size)
        assistArray[start + ref] = '\0';
    return ref;
}

void build(char *mainArray, char *assistArray, char *tmpArray, int start, int end)
{
    if (end - start >= 2) {
        int ref = find(assistArray, mainArray[start]);
        if (ref == 0 || ref == end - start)
            build(mainArray, assistArray, tmpArray, start + 1, end);
        else if (end - start >= 2) {
            for (int i = 0; i < ref; i++)
                tmpArray[i] = mainArray[start + 1 + i];
            for (int i = 0; i < end - start - ref; i++)
                mainArray[start + 1 + i] = mainArray[start + ref + 1 + i];
            for (int i = 0; i < ref; i++)
                mainArray[end - ref + 1 + i] = tmpArray[i];
            build(mainArray, assistArray, tmpArray, end - ref + 1, end);
            build(mainArray, assistArray, tmpArray, start + 1, end - ref);
        }
    }
    else {
        find(assistArray, mainArray[start]);
        find(assistArray, mainArray[end]);
    }
}

int main()
{
    int N;
    cin >> N;
    getchar();
    for (int i = 1; i <= N; i++) {
        char mainArray[max_size] = {0}, assistArray[max_size] = {0}, tmpArray[max_size] = {0};
        int length = 0, index = 0;
        char c;
        while ((c = getchar()) != '\n' && index < max_size)
            mainArray[index++] = c;
        length = index;
        index = 0;
        while ((c = getchar()) != '\n' && index < max_size)
            assistArray[index++] = c;
        build(mainArray, assistArray, tmpArray, 0, length - 1);
        for (int j = 0; j < length; j++)
            cout << mainArray[length - 1 - j];
        cout << endl;
    }
    return 0;
}