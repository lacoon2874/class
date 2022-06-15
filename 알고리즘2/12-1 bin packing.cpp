#include <iostream>
#include <fstream>
#include <cmath>
using std::cin;
using std::cout;

class bin {
	std::ifstream fp;
	int capacity, objNum, n;
	int* objSize, * tree, * player;

	void inputData();
	void winnerTree();

public:
	~bin();

	void func();
};

void bin::inputData() {
	fp.open("12аж - Bin Packing tournament4.txt");

	fp >> capacity >> objNum >> n;
	
	objSize = new int[objNum];
	tree = new int[n];
	player = new int[n + 1];

	for (int i = 0; i < objNum; i++) {
		fp >> objSize[i];
	}
	for (int i = 1; i <= n; i++) {
		player[i] = capacity;
	}

	fp.close();
}

void bin::winnerTree() {
	int s = (int)pow(2, ceil(log2(n)) - 1);
	int lowExt = 2 * (n - s);
	int offset = 2 * s - 1;

	int k, p;

	if (lowExt == n) {
		for (k = 1; k <= n; k += 2) {
			p = (k + n - 1) / 2;
			tree[p] = k;
		}
	}
	else {
		for (k = 1; k <= lowExt; k += 2) {
			p = (k + offset) / 2;
			tree[p] = k;
		}
		for (; k <= n; k += 2) {
			p = (k - lowExt + n - 1) / 2;
			tree[p] = k;
		}
	}
	for (k = (n - 1) / 2; k > 0; k--) {
		tree[k] = tree[2 * k];
	}

	int ptr;
	for (int i = 0; i < objNum; i++) {
		for (ptr = 1; ptr <= n - 1;) {
			if (objSize[i] <= player[tree[ptr]]) {
				ptr *= 2;
			}
			else {
				ptr++;
			}
		}
		int ext = ptr;
		if (ext <= offset) {
			ext = ext - (n - 1) + lowExt;
		}
		else {
			ext = ext - offset;
		}
		if (player[ext] < objSize[i]) {
			ext++;
			ptr++;
		}
		player[ext] -= objSize[i];

		if (ptr > offset) {
			if (ptr % 2) {
				ext--;
			}
			ptr /= 2;
			if (player[ext] >= player[ext + 1]) {
				tree[ptr] = ext;
			}
			else {
				tree[ptr] = ext + 1;
			}
		}
		if (n % 2) {
			if (ptr > n) {
				if (ptr % 2) {
					ext--;
				}
				ptr /= 2;
				if (player[ext] >= player[ext + 1]) {
					tree[ptr] = ext;
				}
				else {
					tree[ptr] = ext + 1;
				}
			}
			else if (ptr > n - 2) {
				ptr /= 2;
				if (player[tree[ptr * 2]] >= player[lowExt + 1]) {
					tree[ptr] = tree[ptr * 2];
				}
				else {
					tree[ptr] = lowExt + 1;
				}
			}
		}
		else if (ptr >= n) {
			if (ptr % 2) {
				ext--;
			}
			ptr /= 2;
			if (player[ext] >= player[ext + 1]) {
				tree[ptr] = ext;
			}
			else {
				tree[ptr] = ext + 1;
			}
		}
		for (ptr /= 2; ptr > 0; ptr /= 2) {
			if (player[tree[ptr * 2]] >= player[tree[ptr * 2 + 1]]) {
				tree[ptr] = tree[ptr * 2];
			}
			else {
				tree[ptr] = tree[ptr * 2 + 1];
			}
		}
	}
}

bin::~bin() {
	delete[] objSize;
	delete[] tree;
	delete[] player;
}

void bin::func() {
	inputData();
	winnerTree();
	for (int i = 1; i <= n; i++) {
		cout << player[i] << ' ';
	}
	cout << '\n';
}

int main() {
	bin binObj;

	binObj.func();

	return 0;
}