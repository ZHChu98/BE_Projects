#include <iostream>
#include <stdio.h>

using namespace std;

#define MAX_SIZE 500

int find_begin(int* array, int start_index, int end_index, int key)
{
    if (end_index == start_index)
        return start_index;
    int mid_index = (end_index + start_index) / 2;
    if (array[mid_index] > key)
        return find_begin(array, start_index, mid_index, key);
    else
        return find_begin(array, mid_index + 1, end_index, key);
}

int find_elem(int* array, int start_index, int end_index, int elem)
{
    if (start_index > end_index)
        return -1;
    int mid_index = (start_index + end_index) / 2;
    if (array[mid_index] == elem)
        return mid_index;
    else if (array[mid_index] > elem)
        return find_elem(array, mid_index + 1, end_index, elem);
    else
        return find_elem(array, start_index, mid_index - 1, elem);
}


int main()
{
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int array[MAX_SIZE] = {};
        int num_array = 0;
        char c = '\0';
        while (c != '\n') {
            cin >> array[num_array++];
            c = getchar();
        }
        int begin_index = find_begin(array, 0, num_array - 1, array[num_array - 1]);

        int search[MAX_SIZE] = {};
        int num_search = 0;
        c = '\0';
        while (c != '\n') {
            cin >> search[num_search++];
            c = getchar();
        }
        int pos = -1;
        for (int j = 0; j < num_search; j++) {
            if (search[j] < array[num_array - 1]) {
                pos = find_elem(array, 0, begin_index - 1, search[j]);
                if (pos != -1)
                    pos += num_array - begin_index;
            }
            else {
                pos = find_elem(array, begin_index, num_array - 1, search[j]);
                if (pos != -1)
                    pos -= begin_index;
            }
            cout << pos << endl;
        }
    }
    return 0;
}