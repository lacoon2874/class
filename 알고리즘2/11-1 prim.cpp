#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

class list {
	int vertex, weight;
	list* next;

public:
	list(int, int);

	int getVertex() const;
	int getWeight() const;
	list* getNext() const;
	void setNext(list*);
};

class adjList {
	list** list_head, ** list_tail;
	int size, node_num;

public:
	adjList(int);
	~adjList();

	void addVertex(int, int, int);
	list* getHead(int) const;
	int getNodeNum() const;
	void printAllList() const;
};

class heapObj {
public:
	int vertex;
	int distance;

	heapObj();
	heapObj(int, int);
};

class heap {
	const int max_size = 1000;
	heapObj* minHeap = new heapObj[max_size];
	int size = 0;

public:
	~heap();

	void insertHeap(heapObj);
	heapObj deleteHeap();
	void printHeap() const;
	bool isEmpty() const;
};

class prim {
	std::ifstream fp;
	adjList* graph = NULL;
	heap minHeap;
	int* d, * p, * flag;
	int n;

	void inputData();
	void mst();

public:
	~prim();

	void func();
};

list::list(int v, int w) : vertex(v), weight(w), next(NULL) {}

int list::getVertex() const {
	return vertex;
}

int list::getWeight() const {
	return weight;
}

list* list::getNext() const {
	return next;
}

void list::setNext(list* ptr) {
	next = ptr;
}

adjList::adjList(int n) : size(n), node_num(0) {
	list_head = new list * [size + 1];
	list_tail = new list * [size + 1];
	for (int i = 1; i <= size; i++) {
		list_head[i] = NULL;
	}
}

adjList::~adjList() {
	delete[] list_head;
	delete[] list_tail;
}

void adjList::addVertex(int u, int v, int w) {
	list* a = new list(v, w);

	node_num++;

	if (!list_head[u]) {
		list_head[u] = a;
	}
	else {
		list_tail[u]->setNext(a);
	}
	list_tail[u] = a;
}

list* adjList::getHead(int v) const {
	return list_head[v];
}

int adjList::getNodeNum() const {
	return node_num;
}

void adjList::printAllList() const {
	for (int i = 1; i <= size; i++) {
		cout << i << " : ";
		for (list* search = list_head[i]; search; search = search->getNext()) {
			cout << "(" << search->getVertex() << ", " << search->getWeight() << ") ";
		}
		cout << '\n';
	}
}

heapObj::heapObj() {}

heapObj::heapObj(int v, int d) : vertex(v), distance(d) {}

heap::~heap() {
	delete[] minHeap;
}

void heap::insertHeap(heapObj item) {
	int i;
	i = ++size;

	while ((i != 1) && item.distance < minHeap[i / 2].distance) {
		minHeap[i] = minHeap[i / 2];
		i /= 2;
	}
	minHeap[i] = item;
}

heapObj heap::deleteHeap() {
	heapObj item = minHeap[1], temp = minHeap[size--];
	int parent = 1, child = 2;

	while (child <= size) {
		if (child < size && minHeap[child].distance > minHeap[child + 1].distance) {
			child++;
		}
		if (temp.distance < minHeap[child].distance) {
			break;
		}
		minHeap[parent] = minHeap[child];
		parent = child;
		child *= 2;
	}
	minHeap[parent] = temp;

	return item;
}

void heap::printHeap() const {
	for (int i = 1; i <= size; i++) {
		cout << minHeap[i].distance << ' ';
	}
	cout << '\n';
}

bool heap::isEmpty() const {
	if (size) {
		return false;
	}
	return true;
}

void prim::inputData() {
	fp.open("11аж - Prim.txt");

	int e;
	fp >> n >> e;

	graph = new adjList(n);
	int u, v, w;
	for (int i = 0; i < e; i++) {
		fp >> u >> v >> w;
		
		graph->addVertex(u, v, w);
		graph->addVertex(v, u, w);
	}

	fp.close();

	d = new int[n + 1];
	p = new int[n + 1];
	flag = new int[n + 1];
}

void prim::mst() {
	for (int i = 1; i <= n; i++) {
		d[i] = INT_MAX;
		flag[i] = 0;
	}
	d[1] = 0;
	p[1] = -1;

	heapObj u(1, d[1]);
	minHeap.insertHeap(u);
	while (!minHeap.isEmpty()) {
		u = minHeap.deleteHeap();
		if (!flag[u.vertex]) {
			for (list* search = graph->getHead(u.vertex); search; search = search->getNext()) {
				if (!flag[search->getVertex()] && search->getWeight() < d[search->getVertex()]) {
					p[search->getVertex()] = u.vertex;
					d[search->getVertex()] = search->getWeight();
					heapObj temp(search->getVertex(), d[search->getVertex()]);
					minHeap.insertHeap(temp);
				}
			}
			minHeap.printHeap();
			flag[u.vertex] = 1;
		}
	}
}

prim::~prim() {
	delete graph;
	delete[] d;
	delete[] p;
	delete[] flag;
}

void prim::func() {
	inputData();
	graph->printAllList();
	cout << "count : " << graph->getNodeNum() << "\n\n";
	mst();
	cout << "\nparent : ";
	for (int i = 1; i <= n; i++) {
		cout << p[i] << ' ';
	}
	cout << "\nweight гу : ";
	int sum = 0;
	for (int i = 1; i <= n; i++) {
		sum += d[i];
	}
	cout << sum << '\n';
}

int main() {
	prim primObj;

	primObj.func();

	return 0;
}