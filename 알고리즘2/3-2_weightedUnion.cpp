#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

class algorithm {
	std::ifstream fp;
	int n;
	int* job, * deadline, * profit;
	int* S, * P, * R;

	void weightedUnion(int, int);
	int collapsingFind(int);

public:
	~algorithm() {
		fp.close();
		delete[] job;
		delete[] deadline;
		delete[] profit;
		delete[] S;
		delete[] P;
		delete[] R;
	}

	void inputData();
	void schedule();
	void printData();
};

void algorithm::weightedUnion(int i, int j) {
	int temp = P[i] + P[j];

	if (P[i] > P[j]) {
		P[i] = j;
		P[j] = temp;
		R[j] = R[i];
	}
	else {
		P[j] = i;
		P[i] = temp;
	}
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
	R = new int[n + 1];

	for (int i = 1; i <= n; i++) {
		fp >> job[i] >> deadline[i] >> profit[i];
		P[i] = -1;
		R[i] = i;
	}
	P[0] = n;
}

void algorithm::schedule() {
	int root;

	for (int i = 1; i <= n; i++) {
		root = collapsingFind(deadline[i]);
		S[R[root]] = job[i];
		weightedUnion(collapsingFind(R[root] - 1), root);
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