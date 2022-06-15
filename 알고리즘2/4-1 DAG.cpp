#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

int n;

// 선언
class list {
	int vertex;
	list* next;
	
public:
	list(int, list*);
	
	int getVertex() const;
	list* getNext() const;
	void setNext(list*);
};

class adj_list {
	list** adj_list_front, ** adj_list_rear;

public:
	adj_list();
	~adj_list();

	void add_edge(int, int);
	list* getFront(int) const;
};

class queue {
	list* front = NULL, * rear = NULL;

public:
	void enqueue(const int&);
	void dequeue();
	int frontQueue() const;
	bool notEmpty() const;
};

class fs {
protected:
	adj_list* graph;
	char* color;
	int* p;

public:
	fs(adj_list*);
	~fs();

	void printP() const;
};

class dfs: public fs {
	list* visit(list*, int);

public:
	dfs(adj_list*);

	list* search(list*);
};

class bfs : public fs {
	int* d;
	queue Q;

public:
	bfs(adj_list*);
	~bfs();
	
	void search();
};

class topSort {
	list* front = NULL;

public:
	dfs* dfs_obj;

	topSort(adj_list*);
	~topSort();

	void printList() const;
	list* getList() const;
	void setList(list*);
};

class dag {
	std::ifstream fp;
	adj_list* graph;
	dfs* dfs_obj;
	bfs* bfs_obj;
	topSort* topSort_obj;

	void inputData();
	void printList() const;

public:
	~dag();

	void func();
};

// 정의

/*
	linked list
*/
list::list(int v, list*l) : vertex(v), next(l) {}  // 생성자

int list::getVertex() const {   // return vertex;
	return vertex;
}

list* list::getNext() const {   // return next
	return next;
}

void list::setNext(list* a) {   // next 값 변경
	next = a;
}

/*
	adjacency list
*/
adj_list::adj_list() {   // 생성자
	adj_list_front = new list * [n + 1];
	adj_list_rear = new list * [n + 1];
	for (int i = 1; i <= n; i++) {
		adj_list_front[i] = NULL;
		adj_list_rear[i] = NULL;
	}
}

adj_list::~adj_list() {  // 소멸자
	for (int i = 1; i <= n; i++) {
		for (list* a = adj_list_front[i]; a; ) {
			list* temp = a;
			a = a->getNext();
			delete temp;
		}
	}
	delete[] adj_list_front;
	delete[] adj_list_rear;
}

void adj_list::add_edge(int a, int b) {  // add edge
	list* temp = new list(b, NULL);

	if (!adj_list_front[a]) {
		adj_list_front[a] = temp;
	}
	else {
		adj_list_rear[a]->setNext(temp);
	}
	adj_list_rear[a] = temp;
}

list* adj_list::getFront(int a) const {   // return adjacency list front
	return adj_list_front[a];
}

/*
	queue
*/
void queue::enqueue(const int& u) {    // add queue
	list *temp = new list(u, NULL);

	if (!front) {
		front = temp;
	}
	else {
		rear->setNext(temp);
	}
	rear = temp;
}

void queue::dequeue() {   // delete queue
	list* temp;

	temp = front;

	front = front->getNext();

	delete temp;
}

int queue::frontQueue() const {   // return queue head
	return front->getVertex();
}

bool queue::notEmpty() const {  // 큐가 비었는지 아닌지 반환
	if (front) {
		return true;
	}
	return false;
}

/*
	dfs, bfs 상속용 클래스
*/
fs::fs(adj_list* a) : graph(a) {   // 생성자
	color = new char[n + 1];
	p = new int[n + 1];

	for (int i = 1; i <= n; i++) {
		color[i] = 'W';
		p[i] = -1;
	}
}

fs::~fs() {  // 소멸자
	delete[] color;
	delete[] p;
}

void fs::printP() const {   // fs print parent
	for (int i = 1; i <= n; i++) {
		cout << p[i] << ' ';
	}
	cout << '\n';
}

/*
	depth first search
*/
dfs::dfs(adj_list* a) : fs(a) {}

list* dfs::search(list* front) {  // dfs
	for (int i = 1; i <= n; i++) {
		if (color[i] == 'W') {
			front = visit(front, i);
		}
	}

	return front;
}

list* dfs::visit(list* front, int u) {  // dfs visit
	color[u] = 'G';
	
	for (list* temp = graph->getFront(u); temp; temp = temp->getNext()) {
		if (color[temp->getVertex()] == 'W') {
			p[temp->getVertex()] = u;
			front = visit(front, temp->getVertex());
		}
	}

	color[u] = 'B';

	list* temp = new list(u, front);   // topsort 출력용 linked list
	front = temp;

	return front;
}

 /*
	breadth first search
 */
bfs::bfs(adj_list* a) : fs(a) {  // 생성자
	d = new int[n + 1];
}

bfs::~bfs() {  // 소멸자
	delete[] d;
}

void bfs::search() {   // bfs
	int u;

	for (u = 2; u <= n; u++) {
		color[u] = 'W'; d[u] = INT_MAX; p[u] = -1;
	}
	color[1] = 'G'; d[1] = 0; p[1] = -1;

	Q.enqueue(1);

	while (Q.notEmpty()) {
		u = Q.frontQueue();

		for (list* temp = graph->getFront(u); temp; temp = temp->getNext()) {
			if (color[temp->getVertex()] == 'W') {
				color[temp->getVertex()] = 'G';
				d[temp->getVertex()] = d[u] + 1;
				p[temp->getVertex()] = u;
				Q.enqueue(temp->getVertex());
			}
		}

		Q.dequeue();

		color[u] = 'B';
	}
}

/*
	topological sort
*/
topSort::topSort(adj_list* a) {
	dfs_obj = new dfs(a);
}

topSort::~topSort() {
	for (list* s = front; s; ) {
		list* temp = s;
		s = s->getNext();
		delete temp;
	}
}

void topSort::printList() const {   // topsort 출력
	for (list* temp = front; temp; temp = temp->getNext()) {
		cout << temp->getVertex() << ' ';
	}
	cout << '\n';
}

list* topSort::getList() const {
	return front;
}

void topSort::setList(list* a) {
	front = a;
}

/*
	directed acyclic graph
*/
dag::~dag() {   // 소멸자
	delete graph;
	delete dfs_obj;
	delete bfs_obj;
	delete topSort_obj;
}

void dag::inputData() {   // 데이터 입력
	fp.open("4주 - bfs DFS & Topological Sort.txt");

	int e;

	fp >> n >> e;

	graph = new adj_list();
	
	int u, v;
	for (int i = 0; i < e; i++) {
		fp >> u >> v;

		graph->add_edge(u, v);
	}

	dfs_obj = new dfs(graph);
	bfs_obj = new bfs(graph);
	topSort_obj = new topSort(graph);
}

void dag::printList() const {    // adjacency list 출력
	for (int i = 1; i <= n; i++) {
		cout << i << ' ';
		for (list* temp = graph->getFront(i); temp; temp = temp->getNext()) {
			cout << '(';
			cout << temp->getVertex();
			cout << ") ";
		}
		cout << '\n';
	}
}

void dag::func() {   // 함수 실행
	inputData();
	printList();

	list* temp = NULL;
	dfs_obj->search(temp);
	cout << '\n';
	cout << "dfs p[i] : ";
	dfs_obj->printP();

	bfs_obj->search();
	cout << "bfs p[i] : ";
	bfs_obj->printP();

	topSort_obj->setList(topSort_obj->dfs_obj->search(topSort_obj->getList()));
	cout << '\n';
	cout << "top. sort : ";
	topSort_obj->printList();
}

int main() {
	dag obj;

	obj.func();

	return 0;
}