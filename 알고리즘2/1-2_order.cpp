#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::string;

class order {
	int n;
	string a;

	void rand_quicksort(int, int);
	int partition(int, int);
	void exchange(char&, char&);
	int rand_partition(int, int);

public:
	void inputData();
	void perm(int);
};

void order::rand_quicksort(int low, int high) {
	int pivotpoint;

	if (low < high) {
		pivotpoint = rand_partition(low, high);
		rand_quicksort(low, pivotpoint - 1);
		rand_quicksort(pivotpoint + 1, high);
	}
}

int order::partition(int low, int high) {
	int i, j;
	int pivotitem;

	pivotitem = a[low];
	j = low;
	for (i = low + 1; i <= high; i++) {
		if (a[i] < pivotitem) {
			j++;
			exchange(a[i], a[j]);
		}
	}

	exchange(a[low], a[j]);

	return j;
}

void order::exchange(char& a, char& b) {
	char temp;

	temp = a;
	a = b;
	b = temp;
}

int order::rand_partition(int low, int high) {
	int i;

	i = low + rand() % (high - low);
	exchange(a[low], a[i]);
	return partition(low, high);
}

void order::inputData() {
	cin >> a;
	n = a.length();
}

void order::perm(int k) {
	if (k >= n) {
		cout << a << '\n';
		return;
	}
	rand_quicksort(k, n - 1);
	perm(k + 1);
	rand_quicksort(k + 1, n - 1);

	for (int m = k + 1; m < n; m++) {
		if (a[m] > a[k]) {
			exchange(a[k], a[m]);
			perm(k + 1);
			rand_quicksort(k + 1, n - 1);
		}
	}
}

int main(void) {
	order temp;

	temp.inputData();
	temp.perm(0);

	return 0;
}