#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

int n;

class data {
public:
	int area, height, weight;
};

class list {
	int vertex, height;
	list* next;

public:
	list(int, int, list*);

	int getVertex() const;
	int getHeight() const;
	list* getNext() const;
	void setNext(list*);
};

class adj_list {
	list** adj_list_front, ** adj_list_rear;

public:
	adj_list();
	~adj_list();

	void add_edge(int, int, int);
	list* getFront(int) const;
};

class dfs {
	adj_list* graph;
	char* color;
	int* p;
	list* visit(list*, int);

public:
	dfs(adj_list*);
	~dfs();

	list* search(list*);
};

class topSort {
	list* front = NULL;

public:
	dfs* dfs_obj;

	topSort(adj_list*);
	~topSort();

	list* getList() const;
	void setList(list*);
};

class tower {
	std::ifstream fp;
	adj_list* graph;
	topSort* topSort_obj;
	int* d, * p;
	data* input;

	void inputData();
	void relax(int, int, int);
	void bellmanFord();
	void printData();

public:
	void func();
};

/*
	linked list
*/
list::list(int v, int h, list* l) : vertex(v), height(h), next(l) {}  // 생성자

int list::getVertex() const {   // return vertex
	return vertex;
}

int list::getHeight() const {   // return height
	return height;
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
	for (int i = 0; i <= n; i++) {
		adj_list_front[i] = NULL;
		adj_list_rear[i] = NULL;
	}
}

adj_list::~adj_list() {  // 소멸자
	for (int i = 0; i <= n; i++) {
		for (list* a = adj_list_front[i]; a; ) {
			list* temp = a;
			a = a->getNext();
			delete temp;
		}
	}
	delete[] adj_list_front;
	delete[] adj_list_rear;
}

void adj_list::add_edge(int a, int b, int c) {  // add edge
	list* temp = new list(b, c, NULL);

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
	depth first search
*/
dfs::dfs(adj_list* a) : graph(a) {
	color = new char[n + 1];
	p = new int[n + 1];

	for (int i = 0; i <= n; i++) {
		color[i] = 'W';
		p[i] = -1;
	}
}

dfs::~dfs() {
	delete[] color;
	delete[] p;
}

list* dfs::search(list* front) {  // dfs
	for (int i = 0; i <= n; i++) {
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

	list* temp = new list(u, 0, front);   // topsort 출력용 linked list
	front = temp;

	return front;
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

list* topSort::getList() const {
	return front;
}

void topSort::setList(list* a) {
	front = a;
}

/*
	tower class
*/
void tower::inputData() {
	fp.open("1.txt");
	fp >> n;

	d = new int[n + 2];
	p = new int[n + 2];
	input = new data[n + 1];
	graph = new adj_list();

	for (int i = 0; i < n + 2; i++) {
		d[i] = 0;
		p[i] = 0;
	}

	int a, h, w;
	for (int i = 1; i <= n; i++) {
		fp >> a >> h >> w;

		input[i].area = a;
		input[i].height = h;
		input[i].weight = w;

		graph->add_edge(0, i, 0);
	}

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (input[j].area < input[i].area && input[j].weight < input[i].weight) {
				graph->add_edge(i, j, input[i].height);
			}
		}
		graph->add_edge(i, n + 1, input[i].height);
	}

	topSort_obj = new topSort(graph);
}

void tower::relax(int u, int v, int w) {
	if (d[v] < d[u] + w) {
		d[v] = d[u] + w;
		p[v] = u;
	}
}

void tower::bellmanFord() {
	for (list* i = topSort_obj->getList(); i; i = i->getNext()) {
		for (list* j = graph->getFront(i->getVertex()); j; j = j->getNext()) {
			relax(i->getVertex(), j->getVertex(), input[i->getVertex()].height);
		}
	}
}

void tower::printData() {
	int count = 0;
	for (int i = p[n + 1]; i; i = p[i]) {
		count++;
	}
	cout << "벽돌 개수 : " << count << '\n';
	for (int i = p[n + 1]; i; i = p[i]) {
		cout << i << '\n';
	}
	cout << "height : " << d[n + 1] << '\n';
}

void tower::func() {
	inputData();
	topSort_obj->setList(topSort_obj->dfs_obj->search(topSort_obj->getList()));
	bellmanFord();
	printData();
}

int main() {
	tower temp;

	temp.func();

	return 0;
}