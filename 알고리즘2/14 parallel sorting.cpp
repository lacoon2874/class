#include <iostream>
#include <fstream>
#include <cmath>

using std::cout;

class parallel {
	std::ifstream fp;
	int n;
	int** arr;

	void inputData();
	void swap(int&, int&);
	void sort();
	void pSort(int);
	void pSortInv(int);
	void pSortCol(int);
	void printData();
	
public:
	~parallel();

	void func();
};

void parallel::inputData() {
	fp.open("14аж - Parallel Sorting.txt");

	fp >> n;

	arr = new int* [n];
	for (int i = 0; i < n; i++) {
		arr[i] = new int[n];
		for (int j = 0; j < n; j++) {
			fp >> arr[i][j];
		}
	}

	fp.close();
}

void parallel::swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

void parallel::sort() {
	for (int i = 0; i < std::log2(n); i++) {
		for (int j = 0; j < n; j++) {
			if (j % 2) {
				pSortInv(j);
			}
			else {
				pSort(j);
			}
		}
		for (int j = 0; j < n; j++) {
			pSortCol(j);
		}
	}
	for (int i = 0; i < n; i++) {
		pSort(i);
	}
}

void parallel::pSort(int row) {
	for (int i = 0; i < n; i++) {
		if (i % 2) {
			for (int j = 1; j < n - 1; j += 2) {
				if (arr[row][j] > arr[row][j + 1]) {
					swap(arr[row][j], arr[row][j + 1]);
				}
			}
		}
		else {
			for (int j = 0; j < n; j += 2) {
				if (arr[row][j] > arr[row][j + 1]) {
					swap(arr[row][j], arr[row][j + 1]);
				}
			}
		}
	}
}

void parallel::pSortInv(int row) {
	for (int i = 0; i < n; i++) {
		if (i % 2) {
			for (int j = 1; j < n - 1; j += 2) {
				if (arr[row][j] < arr[row][j + 1]) {
					swap(arr[row][j], arr[row][j + 1]);
				}
			}
		}
		else {
			for (int j = 0; j < n; j += 2) {
				if (arr[row][j] < arr[row][j + 1]) {
					swap(arr[row][j], arr[row][j + 1]);
				}
			}
		}
	}
}

void parallel::pSortCol(int col) {
	for (int i = 0; i < n; i++) {
		if (i % 2) {
			for (int j = 1; j < n - 1; j += 2) {
				if (arr[j][col] > arr[j + 1][col]) {
					swap(arr[j][col], arr[j + 1][col]);
				}
			}
		}
		else {
			for (int j = 0; j < n; j += 2) {
				if (arr[j][col] > arr[j + 1][col]) {
					swap(arr[j][col], arr[j + 1][col]);
				}
			}
		}
	}
}

void parallel::printData() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << arr[i][j] << ' ';
		}
		cout << '\n';
	}
}

parallel::~parallel() {
	for (int i = 0; i < n; i++) {
		delete[] arr[i];
	}
	delete[] arr;
}

void parallel::func() {
	inputData();
	sort();
	printData();
}

int main() {
	parallel parallelObj;

	parallelObj.func();

	return 0;
}