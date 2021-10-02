// final version
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

void swap(int &a, int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

class Heap{
    private:
      int data[100];
      int length;

    public:
      Heap() { init(); }
      ~Heap() = default;
      void init(); // initialization
      int insert(int val); // insert a value into the heap and return the maximum value (top of the heap)
      int pop(); // delete the maximum value (top of the heap)
};

void Heap::init()
{
    memset(data, 0, 100);
    length = 0;
}

// the maximum value will pop to the top
int Heap::insert(int val)
{
    data[length++] = val;
    for (int i = length - 1; i != 0 && data[i] > data[(i - 1) / 2]; i = (i - 1) / 2)
        swap(data[i], data[(i - 1) / 2]);
    return data[0];
}

// the minimum value will sink into the bottom
int Heap::pop()
{
    swap(data[0], data[(length--) - 1]);
    int dad = 0, son = 1;
    while (son < length) {
        if (son + 1 < length && data[son] < data[son + 1])
            son++;
        if (data[dad] > data[son])
            break;
        else {
            swap(data[dad], data[son]);
            dad = son;
            son = dad * 2 + 1;
        }
    }
    return data[0];
}

int main()
{
    Heap heap;
    int N;
    cin >> N;
    getchar();
    for (int i = 1; i <= N; i++) {
        heap.init();
        int n, m, tmp;
        cin >> n >> m;
        getchar();
        // building heap and reading data work at the same time
        for (int j = 1; j <= n; j++) {
            cin >> tmp;
            getchar();
            heap.insert(tmp);
        }
        char op;
        // reading operations
        for (int j = 1; j <= m; j++) {
            cin >> op;
            getchar();
            if (op == 'I') {
                cin >> tmp;
                getchar();
                cout << heap.insert(tmp) << endl;
            }
            else
                cout << heap.pop() << endl;
        }
    }
    return 0;
}