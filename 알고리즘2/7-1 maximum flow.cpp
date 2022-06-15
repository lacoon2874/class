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
	void printMaxFlow() const;

public:
	~maxFlow();

	void func();
};

list::list(int v, int c) : vertex(v), capacity(c), flow(0), pair(NULL), next(NULL) {}

int list::getVertex() const {
	return vertex;
}

int list::getCapacity() const{
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
	fp.open("Maxflow_input2.txt");

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

void maxFlow::printMaxFlow() const {
	int sum = 0;

	for (list* search = graph->getFront(0); search; search = search->getNext()) {
		sum += search->getFlow();
	}

	cout << '\n' << "max flow is " << sum << '\n' << '\n';
}

maxFlow::~maxFlow() {
	delete graph;
}

void maxFlow::func() {
	inputData();
	printGraph();

	bfsObj->search(graph);
	while (calculate()) {
		bfsObj->search(graph);
	}

	printMaxFlow();
}

int main() {
	maxFlow maxFlowObj;

	maxFlowObj.func();

	return 0;
}