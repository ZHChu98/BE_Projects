#include <iostream>
#include <stdlib.h>
using namespace std;

// Min-heap class
// arranged in an array starting from 0 upon
template <class Elem> 
class MinHeap {
	private:
	  Elem *heap;
	  int size;
	  int max_size;

	public:
	  MinHeap(Elem *arr, int length, int maxsize);
	  ~MinHeap() { delete[] heap; }
	  inline void swap(int index1, int index2) {
		  Elem tmp = heap[index1];
		  heap[index1] = heap[index2];
		  heap[index2] = tmp;
	  }
	  void shiftup(int index);
	  void shiftdown(int index);
	  void insert(Elem val);
	  void removemin(Elem &tmp);
	  void remove(int index, Elem &tmp);
	  void print() const {
		  for (int i = 0; i < size; i++)
			  cout << " | " << heap[i];
		  cout << " | " << endl;
	  }
};

template <class Elem>
MinHeap<Elem>::MinHeap(Elem *arr, int length, int maxsize) 
{
	heap = new Elem[maxsize];
	size = 0;
	max_size = maxsize;
	for (int i = 0; i < length; i++)
		insert(arr[i]);
}

template <class Elem>
void MinHeap<Elem>::shiftup(int index)
{
	int i = index;
	while (i != 0 && heap[i] < heap[(i - 1) / 2]) {
		swap(i, (i - 1) / 2);
		i = (i - 1) / 2;
	}
}

template <class Elem>
void MinHeap<Elem>::shiftdown(int index)
{
	int dad = index, son = 2 * index + 1;
	while (son < size) {
        if (son + 1 < size && heap[son] > heap[son + 1])
            son++;
        if (heap[dad] <= heap[son])
            break;
        else {
            swap(dad, son);
            dad = son;
			son = dad * 2 + 1;
		}
	}
}

template <class Elem>
void MinHeap<Elem>::insert(Elem val)
{
	if (size == max_size)
		cout << "Fault: heap reached its maximum size" << endl;
	else {
		heap[size++] = val;
		shiftup(size - 1);
	}
}

template <class Elem>
void MinHeap<Elem>::removemin(Elem &tmp)
{
	swap(0, --size);
	tmp = heap[size];
	shiftdown(0);
}

template <class Elem>
void MinHeap<Elem>::remove(int index, Elem &tmp)
{
	swap(index, --size);
	tmp = heap[size];
	shiftdown(index);
}
