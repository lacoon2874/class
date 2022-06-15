#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

class list {
	int vertex, capacity, flow;
	list* pair, * next;

public:
	list(int, int);

	int getVertex() const;
	int getCapacity() const;
	int getFlow() const;
	list* getPair() const;
	list* getNext() const;
	void setCapacity(int);
	void setFlow(int);
	void setPair(list*);
	void setNext(list*);
};

class adjList {
	list** list_front, ** list_tail;
	int size, nodeNum;

public:
	adjList(int);
	~adjList();

	void addVertex(int, int, int);
	list* getFront(int) const;
	int getNodeNum() const;
};

class queueData {
	int index;
	queueData* next;

public:
	queueData(int);

	int getIndex() const;
	queueData* getNext() const;
	void setNext(queueData*);
};

class queue {
	queueData* front, * rear;

public:
	queue();

	bool notEmpty() const;
	void enQueue(int);
	int deQueue();
};

class bfs {
	int* parent;
	list** parent_list;
	char* color;
	int size;
	queue* Q;

public:
	bfs(int);
	~bfs();

	void search(adjList*);
	int getParent(int) const;
	list* getParentList(int) const;
};

class maxFlow {
	std::ifstream fp;
	int n = 0;
	adjList* graph = NULL;
	bfs* bfsObj = NULL;

	void inputData();
	void printGraph() const;
	bool calculate();

public:
	~maxFlow();

	void func();
	int getMaxFlow() const;
};

class escape {
	std::ifstream ifp;
	std::ofstream ofp;
	int n = 0, count = 0;
	adjList* graph = NULL;
	maxFlow* flowObj = NULL;

	void inputData();
	void printGraph();

public:
	void func();
};

list::list(int v, int c) : vertex(v), capacity(c), flow(0), pair(NULL), next(NULL) {}

int list::getVertex() const {
	return vertex;
}

int list::getCapacity() const {
	return capacity;
}

int list::getFlow() const {
	return flow;
}

list* list::getPair() const {
	return pair;
}

list* list::getNext() const {
	return next;
}

void list::setCapacity(int data) {
	capacity = data;
}

void list::setFlow(int data) {
	flow = data;
}

void list::setPair(list* ptr) {
	pair = ptr;
}

void list::setNext(list* ptr) {
	next = ptr;
}

adjList::adjList(int n) : size(n), nodeNum(0) {
	list_front = new list * [size];
	list_tail = new list * [size];

	for (int i = 0; i < size; i++) {
		list_front[i] = NULL;
	}
}

adjList::~adjList() {
	for (int i = 0; i < size; i++) {
		for (list* search = list_front[i]; search;) {
			list* temp = search;
			search = search->getNext();
			delete temp;
		}
	}
	delete[] list_front;
	delete[] list_tail;
}

void adjList::addVertex(int u, int v, int c) {
	nodeNum += 2;

	list* a = new list(v, c);
	list* b = new list(u, 0);

	a->setPair(b);
	b->setPair(a);

	if (!list_front[u]) {
		list_front[u] = a;
	}
	else {
		list_tail[u]->setNext(a);
	}
	list_tail[u] = a;

	if (!list_front[v]) {
		list_front[v] = b;
	}
	else {
		list_tail[v]->setNext(b);
	}
	list_tail[v] = b;
}

list* adjList::getFront(int vertex) const {
	return list_front[vertex];
}

int adjList::getNodeNum() const {
	return nodeNum;
}

queueData::queueData(int data) : index(data), next(NULL) {}

int queueData::getIndex() const {
	return index;
}

queueData* queueData::getNext() const {
	return next;
}

void queueData::setNext(queueData* ptr) {
	next = ptr;
}

queue::queue() : front(NULL), rear(NULL) {}

bool queue::notEmpty() const {
	if (front) {
		return true;
	}

	return false;
}

void queue::enQueue(int data) {
	queueData* temp = new queueData(data);

	if (!front) {
		front = temp;
	}
	else {
		rear->setNext(temp);
	}
	rear = temp;
}

int queue::deQueue() {
	int ret_val = front->getIndex();

	queueData* temp = front;
	front = front->getNext();
	delete temp;

	return ret_val;
}

bfs::bfs(int n) : size(n) {
	parent = new int[size];
	parent_list = new list * [size];
	color = new char[size];
	Q = new queue();
}

bfs::~bfs() {
	delete[] parent;
	delete[] parent_list;
	delete[] color;
	delete Q;
}

void bfs::search(adjList* graph) {
	for (int i = 0; i < size; i++) {
		parent[i] = -1;
		parent_list[i] = NULL;
		color[i] = 'W';
	}

	color[0] = 'G';

	Q->enQueue(0);
	int u;
	while (Q->notEmpty()) {
		u = Q->deQueue();

		for (list* search = graph->getFront(u); search; search = search->getNext()) {
			if (color[search->getVertex()] == 'W' && (search->getCapacity() - search->getFlow() > 0)) {
				color[search->getVertex()] = 'G';
				parent[search->getVertex()] = u;
				parent_list[search->getVertex()] = search;
				Q->enQueue(search->getVertex());
			}
		}

		color[u] = 'B';
	}
}

int bfs::getParent(int vertex) const {
	return parent[vertex];
}

list* bfs::getParentList(int vertex) const {
	return parent_list[vertex];
}

void maxFlow::inputData() {
	fp.open("escape_output.txt");

	int e_size;
	fp >> n >> e_size;
	n += 2;
	graph = new adjList(n);

	int u, v, c;
	list* search;
	for (int i = 0; i < e_size; i++) {
		fp >> u >> v >> c;
		for (search = graph->getFront(u); search; search = search->getNext()) {
			if (search->getVertex() == v) {
				search->setCapacity(c);
				break;
			}
		}
		if (!search) {
			graph->addVertex(u, v, c);
		}
	}

	bfsObj = new bfs(n);
}

void maxFlow::printGraph() const {
	cout << "ÃÑ ³ëµå °³¼ö : " << graph->getNodeNum() << '\n';
	for (int i = 0; i < n; i++) {
		cout << i << " : ";
		for (list* search = graph->getFront(i); search; search = search->getNext()) {
			cout << "( ";
			cout << search->getVertex();
			cout << ", ";
			cout << search->getCapacity();
			cout << ", ";
			cout << search->getFlow();
			cout << ", ";
			cout << search->getPair();
			cout << " ) ";
		}
		cout << '\n';
	}
}

bool maxFlow::calculate() {
	int min = INT_MAX;

	if (bfsObj->getParent(n - 1) == -1) {
		return false;
	}

	for (int i = n - 1; i > 0; i = bfsObj->getParent(i)) {
		list* temp = bfsObj->getParentList(i);
		if (temp->getCapacity() - temp->getFlow() < min) {
			min = temp->getCapacity() - temp->getFlow();
		}
	}
	for (int i = n - 1; i > 0; i = bfsObj->getParent(i)) {
		list* temp = bfsObj->getParentList(i);
		temp->setFlow(temp->getFlow() + min);
		temp->getPair()->setFlow(-(temp->getFlow()));
	}

	return true;
}

int maxFlow::getMaxFlow() const {
	int sum = 0;

	for (list* search = graph->getFront(0); search; search = search->getNext()) {
		sum += search->getFlow();
	}

	return sum;
}

maxFlow::~maxFlow() {
	delete graph;
}

void maxFlow::func() {
	inputData();
	//printGraph();

	bfsObj->search(graph);
	while (calculate()) {
		bfsObj->search(graph);
	}
}

void escape::inputData() {
	ifp.open("escape_input7.txt");

	ifp >> n;

	graph = new adjList(n * n * 2 + 2);

	char ch;
	int temp;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ifp >> ch;
			while (ch < '0' || ch>'9') {
				ifp >> ch;
			}
			temp = ch - '0';
			if (temp) {
				count++;
				graph->addVertex(0, (i * n + j) * 2 + 1, 1);
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			graph->addVertex((i * n + j) * 2 + 1, (i * n + j) * 2 + 2, 1);
		}
	}

	for (int i = 0; i < n; i++) {
		graph->addVertex(i * 2 + 2, n * n * 2 + 1, 1);
	}
	for (int i = 1; i < n - 1; i++) {
		graph->addVertex(i * n * 2 + 2, n * n * 2 + 1, 1);
		graph->addVertex((i * n + n) * 2, n * n * 2 + 1, 1);
	}
	for (int i = 0; i < n; i++) {
		graph->addVertex(((n - 1) * n + i) * 2 + 2, n * n * 2 + 1, 1);
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n - 1; j++) {
			graph->addVertex((i * n + j) * 2 + 2, (i * n + j + 1) * 2 + 1, 1);
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 1; j < n; j++) {
			graph->addVertex((i * n + j) * 2 + 2, (i * n + j - 1) * 2 + 1, 1);
		}
	}
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n; j++) {
			graph->addVertex((i * n + j) * 2 + 2, ((i + 1) * n + j) * 2 + 1, 1);
		}
	}
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < n; j++) {
			graph->addVertex((i * n + j) * 2 + 2, ((i - 1) * n + j) * 2 + 1, 1);
		}
	}

	flowObj = new maxFlow();
}

void escape::printGraph() {
	ofp.open("escape_output.txt");

	ofp << n * n * 2 << ' ' << graph->getNodeNum() / 2 << '\n';
	for (int i = 0; i < n * n * 2 + 2; i++) {
		for (list *search = graph->getFront(i); search; search = search->getNext()) {
			if (search->getCapacity()) {
				ofp << i << ' ' << search->getVertex() << ' ' << search->getCapacity() << '\n';
			}
		}
	}

	ofp.close();
}

void escape::func() {
	inputData();
	printGraph();
	flowObj->func();

	int ret_val;
	ret_val = flowObj->getMaxFlow();

	if (count == ret_val) {
		cout << "start : " << count << " = max : " << ret_val << '\n';
		cout << "escape success!!\n";
	}
	else {
		cout << "start : " << count << " = max : " << ret_val << '\n';
		cout << "escape fail!!\n";
	}
}

int main() {
	escape escapeObj;

	escapeObj.func();

	return 0;
}