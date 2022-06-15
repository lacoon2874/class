#include <iostream>
#include <fstream>

using std::cout;

struct nodetype {
	char key;
	nodetype* left;
	nodetype* right;
};

class obst {
	std::ifstream fp;
	int n;
	double** A;
	int** R;
	char* name;
	double* p;
	nodetype* tree;

	void inputData();
	void optsearchtree();
	nodetype* makeTree(int, int) const;
	void printPreOrder(nodetype*) const;
	void printPostOrder(nodetype*) const;
	void deleteTree(nodetype*);
	
public:
	~obst();

	void func();
};

void obst::inputData() {
	fp.open("13аж - Optimal Binary Search Tree.txt");

	fp >> n;

	A = new double* [n + 2];
	R = new int* [n + 2];
	name = new char[n + 1];
	p = new double[n + 1];
	for (int i = 1; i <= n + 1; i++) {
		A[i] = new double[n + 1];
		R[i] = new int[n + 1];
	}

	char ch;
	double num;
	for (int i = 1; i <= n; i++) {
		fp >> ch;
		while (ch < 'A' || ch>'Z') {
			fp >> ch;
		}
		name[i] = ch;
		
		fp >> num;
		p[i] = num;
	}

	fp.close();
}

void obst::optsearchtree() {
	for (int i = 1; i <= n; i++) {
		A[i][i - 1] = 0;
		A[i][i] = p[i];
		R[i][i - 1] = 0;
		R[i][i] = i;
	}
	A[n + 1][n] = 0;
	R[n + 1][n] = 0;

	for (int diagonal = 1; diagonal <= n - 1; diagonal++) {
		for (int i = 1; i <= n - diagonal; i++) {
			int j = i + diagonal;
			double min = 100000;
			for (int k = i; k <= j; k++) {
				if (A[i][k - 1] + A[k + 1][j] < min) {
					min = A[i][k - 1] + A[k + 1][j];
					R[i][j] = k;
				}
			}
			for (int k = i; k <= j; k++) {
				min += p[k];
			}
			A[i][j] = min;
		}
	}
}

nodetype* obst::makeTree(int i, int j) const {
	int k = R[i][j];
	if (!k) {
		return NULL;
	}
	else {
		nodetype* p = new nodetype();
		p->key = name[k];
		p->left = makeTree(i, k - 1);
		p->right = makeTree(k + 1, j);
		return p;
	}
}

void obst::printPreOrder(nodetype* p) const {
	if (p) {
		cout << p->key << ' ';
		printPreOrder(p->left);
		printPreOrder(p->right);
	}
}

void obst::printPostOrder(nodetype* p) const {
	if (p) {
		printPostOrder(p->left);
		printPostOrder(p->right);
		cout << p->key << ' ';
	}
}

void obst::deleteTree(nodetype* p) {
	if (p) {
		deleteTree(p->left);
		deleteTree(p->right);
		delete p;
	}
}

obst::~obst() {
	delete[] A, R, name, p;
	deleteTree(tree);
}

void obst::func() {
	inputData();
	optsearchtree();
	tree = makeTree(1, n);

	cout << A[1][n] << '\n';
	cout << "preorder : ";
	printPreOrder(tree);
	cout << '\n';
	cout << "postorder : ";
	printPostOrder(tree);
	cout << '\n';
}

int main() {
	obst obstObj;

	obstObj.func();

	return 0;
}