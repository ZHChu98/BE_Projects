/* Since char = uint8, I think there is no need to consider MinHeap<char> and MinHeap<int> sepeartely
 * So I decided to delete those two class: IntComp and CharComp, as well as changing definition of MinHeap
 * Inevidably, this file MinHeapMain.cpp have been changed a little
 * A little bit more notation: the heap starts with the index 0 not 1
 */
#include "MinHeap.h"

int main()
{
	const int size = 63, n = 7;

	// Initialize int data
	int h[size], a;
	cout << "Batch initial int data loading ...\n";
	for (int i = 0; i < n; i++) {
		h[i] = 12 - i;
		cout << " | " << h[i];
	}
    cout << " | " << endl;

    // Create an int maxheap tree
	MinHeap<int> hp(h, n, size);
	cout << "After fast heap construction ...\n";
	hp.print();

	cout << "Insert 0 into the heap \n";
	hp.insert(0);
	hp.print();
	cout << "Insert 2 into the heap \n";
	hp.insert(2);
	hp.print();
	cout << "Insert 3 into the heap \n";
	hp.insert(3);
	hp.print();

	cout << "Remove min \n";
	hp.removemin(a);
	hp.print();
	cout << "Remove node indexed 1 \n";
	hp.remove(1, a);
	hp.print();
    return 0;
}

