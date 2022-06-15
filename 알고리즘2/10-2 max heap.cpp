#include <iostream>
#include <fstream>
#include <vector>
using std::cin;
using std::cout;

class maxHeap {
	const int max_size = 1000;
	std::ifstream fp;
	int* heap = new int[max_size];
	int size = 0;

	void insertMaxHeap(int);
	int deleteMaxHeap();
	void heapSort();
	void printHeap() const;

public:
	void func();
};

void maxHeap::insertMaxHeap(int item) {
	int i;
	i = ++size;

	while ((i != 1) && item > heap[i / 2]) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}

int maxHeap::deleteMaxHeap() {
	int item = heap[1], temp = heap[size--];
	int parent = 1, child = 2;

	while (child <= size) {
		if (child < size && heap[child] < heap[child + 1]) {
			child++;
		}
		if (temp >= heap[child]) {
			break;
		}
		heap[parent] = heap[child];
		parent = child;
		child *= 2;
	}
	heap[parent] = temp;

	return item;
}

void maxHeap::heapSort() {
	maxHeap a;

	for (int i = 1; i <= size; i++) {
		a.insertMaxHeap(heap[i]);
	}
	for (int i = size; i > 0; i--) {
		cout << a.deleteMaxHeap() << ' ';
	}
	cout << '\n';
}

void maxHeap::printHeap() const {
	for (int i = 1; i <= size; i++) {
		cout << heap[i] << ' ';
	}
	cout << '\n';
}

void maxHeap::func() {
	fp.open("10аж - Heap.txt");

	int n, item;
	fp >> n;
	for (int i = 0; i < n; i++) {
		fp >> item;
		insertMaxHeap(item);
	}
	printHeap();

	heapSort();

	fp >> item;
	while (item != 9999) {
		if (item == -9999) {
			deleteMaxHeap();
		}
		else {
			insertMaxHeap(item);
		}
		printHeap();
		fp >> item;
	}
}

int main() {
	maxHeap heapObj;

	heapObj.func();

	return 0;
}