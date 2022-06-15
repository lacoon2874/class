#include <iostream>
#include <random>
using std::cin;
using std::cout;

class sort {
	int n;
	int* S;

	void inputData();
	void rand_quicksort(int, int);
	int partition(int, int);
	void exchange(int&, int&);
	int rand_partition(int, int);
	void printData();

public:
	~sort() {
		delete[] S;
	}

	void func();
};

void sort::inputData() {
	cin >> n;
	S = new int[n];
	for (int i = 0; i < n; i++) {
		cin >> S[i];
	}
}

void sort::rand_quicksort(int low, int high) {
	int pivotpoint;

	if (low < high) {
		pivotpoint = rand_partition(low, high);
		rand_quicksort(low, pivotpoint - 1);
		rand_quicksort(pivotpoint + 1, high);
	}
}

int sort::partition(int low, int high) {
	int i, j;
	int pivotitem;

	pivotitem = S[low];
	j = low;
	for (i = low + 1; i <= high; i++) {
		if (S[i] < pivotitem) {
			j++;
			exchange(S[i], S[j]);
		}
	}

	exchange(S[low], S[j]);

	return j;
}

void sort::exchange(int& a, int& b) {
	int temp;

	temp = a;
	a = b;
	b = temp;
}

int sort::rand_partition(int low, int high) {
	int i;

	i = low + rand() % (high - low);
	exchange(S[low], S[i]);
	return partition(low, high);
}

void sort::printData() {
	for (int i = 0; i < n; i++) {
		cout << S[i] << ' ';
	}
}

void sort::func() {
	inputData();
	rand_quicksort(0, n - 1);
	printData();
}

int main(void) {
	sort temp;
	
	temp.func();

	return 0;
}