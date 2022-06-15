#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

class algorithm {
	std::ifstream fp;
	int n;
	int* job, * deadline, * profit;
	int* S, * P;

	void simpleUnion(int, int);
	int collapsingFind(int);

public:
	~algorithm() {
		fp.close();
		delete[] job;
		delete[] deadline;
		delete[] profit;
		delete[] S;
		delete[] P;
	}

	void inputData();
	void schedule();
	void printData();
};

void algorithm::simpleUnion(int i, int j) {
	P[j] = i;
}

int algorithm::collapsingFind(int i) {
	int r;
	for (r = i; P[r] >= 0; r = P[r]);
	while (i != r) {
		int s = P[i];
		P[i] = r;
		i = s;
	}

	return r;
}

void algorithm::inputData() {
	fp.open("3ÁÖÂ÷ - Scheduling.txt");

	fp >> n;
	
	job = new int[n + 1];
	deadline = new int[n + 1];
	profit = new int[n + 1];
	S = new int[n + 1];
	P = new int[n + 1];

	for (int i = 1; i <= n; i++) {
		fp >> job[i] >> deadline[i] >> profit[i];
		P[i] = -1;
	}
	P[0] = n;
}

void algorithm::schedule() {
	int root;

	for (int i = 1; i <= n; i++) {
		root = collapsingFind(deadline[i]);
		S[root] = job[i];
		simpleUnion(collapsingFind(root - 1), root);
	}
}

void algorithm::printData() {
	for (int i = 1; i <= n; i++) {
		cout << S[i] << ' ';
	}
}

int main(void) {
	algorithm temp;

	temp.inputData();
	temp.schedule();
	temp.printData();

	return 0;
}