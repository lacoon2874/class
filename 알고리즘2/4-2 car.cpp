#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

int n;

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

class car {
	std::ifstream fp;
	int max_distance;
	int* distance, * time;
	int* t, * p;
	adj_list* list_obj;

	void inputData();
	void min();
	void printAnswer(int, int);

public:
	void func();
};

list::list(int v, list* l) : vertex(v), next(l) {}

int list::getVertex() const {
	return vertex;
}

list* list::getNext() const {
	return next;
}

void list::setNext(list* a) {
	next = a;
}

adj_list::adj_list() {
	adj_list_front = new list * [n + 1];
	adj_list_rear = new list * [n + 1];
	for (int i = 0; i <= n; i++) {
		adj_list_front[i] = NULL;
		adj_list_rear[i] = NULL;
	}
}

adj_list::~adj_list() {
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

void adj_list::add_edge(int a, int b) {
	list* temp = new list(b, NULL);

	if (!adj_list_front[a]) {
		adj_list_front[a] = temp;
	}
	else {
		adj_list_rear[a]->setNext(temp);
	}
	adj_list_rear[a] = temp;
}

list* adj_list::getFront(int a) const {
	return adj_list_front[a];
}

void car::inputData() {
	fp.open("4주 - 자동차경주.txt");
	
	fp >> max_distance >> n;

	distance = new int[n + 1];
	time = new int[n + 1];
	t = new int[n + 2];
	p = new int[n + 2];

	for (int i = 0; i <= n; i++) {
		fp >> distance[i];
	}
	for (int i = 1; i <= n; i++) {
		fp >> time[i];
	}
	time[n + 1] = 0;

	t[0] = 0;
	p[0] = -1;
	for (int i = 1; i <= n + 1; i++) {
		t[i] = INT_MAX;
		p[i] = -1;
	}

	list_obj = new adj_list();
	int sum;
	for (int i = 0; i <= n; i++) {
		sum = 0;
		for (int j = i; j <= n; j++) {
			sum += distance[j];
			if (sum > max_distance) {
				break;
			}

			list_obj->add_edge(i, j + 1);
		}
	}
}

void car::min() {
	for (int i = 0; i <= n; i++) {
		for (list* temp = list_obj->getFront(i); temp; temp = temp->getNext()) {
			if (t[i] + time[temp->getVertex()] < t[temp->getVertex()]) {
				t[temp->getVertex()] = t[i] + time[temp->getVertex()];
				p[temp->getVertex()] = i;
			}
		}
	}
}

void car::printAnswer(int cur, int count) {
	if (!cur) {
		cout << count << '\n';
		return;
	}

	printAnswer(p[cur], count + 1);
	cout << cur << ' ';
}

void car::func() {
	inputData();
	min();
	cout << t[n + 1] << '\n';
	printAnswer(p[n + 1], 0);
}

int main() {
	car car_obj;
	
	car_obj.func();

	return 0;
}