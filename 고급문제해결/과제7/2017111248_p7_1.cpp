#include <iostream>
using std::cin;
using std::cout;

enum ColorType {
	WHITE = 0,
	GRAY = 1,
	BLACK = 2
};

struct vertex {
	ColorType color;
	int d, f, pi;
};

class list {
	int data;
	list* next;

public:
	list(int);
	
	int getData();
	void setData(int);
	list* getNext();
	void setNext(list*);
};

class graph {
	int size;
	list** head, ** tail;

	void addList(int, int);

public:
	graph(int);

	int getSize();
	list* getHead(int);
	list* getTail(int);
	
	void addEdge(int, int);
};

void dfs(graph&, vertex*&);
void dfs_visit(graph&, vertex*&, int&, int);

int main(void) {
	int n;
	cout << "n = ";
	cin >> n;
	graph G(n);

	cout << "edges = ";
	char ch;
	int openCount = 0, a, b;
	cin >> ch;
	while (true) {
		switch (ch) {
		case '[':
			openCount++;
			break;
		case ']':
			openCount--;
			break;
		};
		if (openCount == 0) {
			break;
		}
		if (openCount == 2) {
			cin >> a;
			cin >> ch;
			cin >> b;
			G.addEdge(a, b);
		}
		cin >> ch;
	}

	int source, destination;
	cout << "source = ";
	cin >> source;
	cout << "destination = ";
	cin >> destination;

	vertex* vertices = new vertex[G.getSize()];
	dfs(G, vertices);
	int a_root, b_root;
	for (a_root = source; vertices[a_root].pi != -1; a_root = vertices[a_root].pi);
	for (b_root = destination; vertices[b_root].pi != -1; b_root = vertices[b_root].pi);
	if (a_root == b_root) {
		cout << "true\n";
	}
	else {
		cout << "false\n";
	}

	return 0;
}

list::list(int key) : data(key), next(NULL) {

}

int list::getData() {
	return data;
}

void list::setData(int key) {
	data = key;
}

list* list::getNext() {
	return next;
}

void list::setNext(list* ptr) {
	next = ptr;
}

void graph::addList(int a, int b) {
	list* newList = new list(b);

	if (!head[a]) {
		head[a] = newList;
	}
	else {
		tail[a]->setNext(newList);
	}
	tail[a] = newList;
}

graph::graph(int n) : size(n) {
	head = new list * [size];
	tail = new list * [size];
	for (int i = 0; i < size; i++) {
		head[i] = NULL;
		tail[i] = NULL;
	}
}

int graph::getSize() {
	return size;
}

list* graph::getHead(int idx) {
	return head[idx];
}

list* graph::getTail(int idx) {
	return tail[idx];
}

void graph::addEdge(int a, int b) {
	addList(a, b);
	addList(b, a);
}

void dfs(graph& G, vertex*& vertices) {
	int graphSize = G.getSize();
	for (int u = 0; u < graphSize; u++) {
		vertices[u].color = WHITE;
		vertices[u].pi = -1;
	}
	int time = 0;
	for (int u = 0; u < graphSize; u++) {
		if (vertices[u].color == WHITE) {
			dfs_visit(G, vertices, time, u);
		}
	}
}

void dfs_visit(graph& G, vertex*& vertices, int& time, int u) {
	vertices[u].d = ++time;
	vertices[u].color = GRAY;
	for (list* v = G.getHead(u); v; v = v->getNext()) {
		if (vertices[v->getData()].color == WHITE) {
			vertices[v->getData()].pi = u;
			dfs_visit(G, vertices, time, v->getData());
		}
	}
	vertices[u].color = BLACK;
	vertices[u].f = ++time;
}