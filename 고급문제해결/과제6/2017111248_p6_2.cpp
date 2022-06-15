#include <iostream>
using std::cin;
using std::cout;

class unionFind {
	int size;
	int* root = NULL;
	int* rank = NULL;

public:
	unionFind(int);
	~unionFind();

	int getSize() const;
	int find(int);
	void unionNode(int, int);
	bool connected(int, int);
};

bool validateTree(unionFind&);

int main(void) {
	int n;
	cout << "n = ";
	cin >> n;

	unionFind graph(n);

	if (validateTree(graph)) {
		cout << "true";
	}
	else {
		cout << "false";
	}
	cout << std::endl;

	return 0;
}

unionFind::unionFind(int n) : size(n) {
	root = new int[size];
	rank = new int[size];

	for (int i = 0; i < size; i++) {
		root[i] = i;
		rank[i] = 1;
	}
}

unionFind::~unionFind() {
	delete[] root, rank;
}

int unionFind::getSize() const {
	return size;
}

int unionFind::find(int x) {
	while (x != root[x]) {
		x = root[x];
	}
	return x;
}

void unionFind::unionNode(int x, int y) {
	int rootX = find(x);
	int rootY = find(y);

	if (rootX != rootY) {
		if (rootX > rootY) {
			root[rootY] = rootX;
		}
		else if (rootX < rootY) {
			root[rootX] = rootY;
		}
		else {
			root[rootY] = rootX;
			rank[rootX]++;
		}
	}
}

bool unionFind::connected(int x, int y) {
	return find(x) == find(y);
}

bool validateTree(unionFind& graph) {
	char ch;
	cin >> ch;
	while (ch != '[') {
		cin >> ch;
	}

	bool isEnd = false;
	int a, b, edgeCount = 0;
	cin >> ch;
	while (!isEnd) {
		if (ch == '[') {
			edgeCount++;
			cin >> a;
			cin >> ch;
			cin >> b;
			cin >> ch;

			graph.unionNode(a, b);
		}
		else if (ch == ']') {
			isEnd = true;
		}
		else {
			// ',' skip
		}
		cin >> ch;
	}

	if (edgeCount != graph.getSize() - 1) {
		return false;
	}
	int root = graph.find(0);
	for (int i = 1; i < graph.getSize(); i++) {
		if (root != graph.find(i)) {
			return false;
		}
	}

	return true;
}