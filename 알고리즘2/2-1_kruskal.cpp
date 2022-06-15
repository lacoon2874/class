#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

class edge {
	int i, j, weight;

public:
	edge(int a, int b, int c) : i(a), j(b), weight(c) {}

	int get_i() const;
	int get_j() const;
	int get_weight() const;
};

int edge::get_i() const {
	return i;
}

int edge::get_j() const {
	return j;
}

int edge::get_weight() const {
	return weight;
}

class sets {
	std::ifstream p;
	int n, m;
	int* parent;
	edge** E, ** F;

	int collapsingFind(int);
	void weightedUnion(int, int);

public:
	~sets() {
		delete[] parent;
		for (int i = 0; i < m; i++) {
			delete E[i];
		}
		delete[] E;
		delete[] F;
	}

	void inputData();
	void kruskal();
	void printData();
};

void sets::inputData() {
	int a, b, weight;

	p.open("02ÁÖÂ÷ - Kruskal.txt");
	p >> n >> m;
	parent = new int[n + 1];
	E = new edge * [m];
	F = new edge * [n - 1];
	for (int i = 0; i < m; i++) {
		p >> a >> b >> weight;
		E[i] = new edge(a, b, weight);
	}
}

int sets::collapsingFind(int i) {
	int r;
	for (r = i; parent[r] >= 0; r = parent[r]);
	while (i != r) {
		int s = parent[i];
		parent[i] = r;
		i = s;
	}

	return r;
}

void sets::weightedUnion(int i, int j) {
	int temp = parent[i] + parent[j];
	if (parent[i] > parent[j]) {
		parent[i] = j;
		parent[j] = temp;
	}
	else {
		parent[j] = i;
		parent[i] = temp;
	}
}

void sets::kruskal() {
	for (int i = 1; i <= n; i++) {
		parent[i] = -1;
	}
	edge *e;
	int i, j, p, q;
	for (int f_size = 0, k = 0; f_size < n - 1; k++) {
		e = E[k];
		i = e->get_i();
		j = e->get_j();
		p = collapsingFind(i);
		q = collapsingFind(j);
		if (p != q) {
			weightedUnion(p, q);
			F[f_size++] = e;
		}
	}
}

void sets::printData() {
	for (int i = 0; i < n - 1; i++) {
		cout << F[i]->get_i() << ' ' << F[i]->get_j() << ' ' << F[i]->get_weight() << '\n';
	}
}

int main(void) {
	sets temp;
	
	temp.inputData();
	temp.kruskal();
	temp.printData();

	return 0;
}