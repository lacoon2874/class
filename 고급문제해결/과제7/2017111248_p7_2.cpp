#include <iostream>
#include <vector>
#include <string>
using std::cin;
using std::cout;

class list {
	int data;
	list* next;

public:
	list(int, list*);

	int getData();
	list* getNext();
	void setNext(list*);
};

class stack {
	list* top;

public:
	stack();

	list* getTop();

	void push(int);
	int pop();
};

class graph {
	int size;
	std::vector<list*> head, tail;

public:
	graph();

	int getSize();
	list* getHead(int);
	list* getTail(int);

	void addEdge(int, int);
	void addEmpty();
};

graph G;
stack S;
bool isFirst = true;

void dfs(int);
void printStack(list*);

int main(void) {
	cout << "adj_list = ";
	std::string input;
	cin >> input;
	int openCount = 0, index = 0;
	for (int i = 0; i < input.length(); i++) {
		switch (input[i]) {
		case '[':
			if (openCount) {
				if (input[i + 1] == ']') {
					G.addEmpty();
					i++;
				}
				else {
					for (int j = i + 1; j < input.length(); j++, i++) {
						if (input[j] == ']') {
							break;
						}
						else if (input[j] >= '0' && input[j] <= '9') {
							G.addEdge(index, input[j] - '0');
						}
					}
					i++;
				}
				index++;
			}
			else {
				openCount++;
			}
			break;
		case ']':
			openCount--;
			i = input.length();
			break;
		}
	}

	cout << '[';
	dfs(0);
	cout << ']';

	return 0;
}

list::list(int key, list* ptr) : data(key), next(ptr) {

}

int list::getData() {
	return data;
}

list* list::getNext() {
	return next;
}

void list::setNext(list* ptr) {
	next = ptr;
}

stack::stack() : top(NULL) {

}

list* stack::getTop() {
	return top;
}

void stack::push(int v) {
	list* newData = new list(v, top);
	top = newData;
}

int stack::pop() {
	list* temp = top;
	int retVal = top->getData();
	top = top->getNext();

	delete temp;

	return retVal;
}

graph::graph() : size(0) {

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
	list* newList = new list(b, NULL);

	if (a >= size) {
		size++;
		head.push_back(newList);
		tail.push_back(newList);
	}
	else {
		tail[a]->setNext(newList);
		tail[a] = newList;
	}
}

void graph::addEmpty() {
	size++;
	head.push_back(NULL);
	tail.push_back(NULL);
}

void dfs(int cur) {
	S.push(cur);
	if (cur == G.getSize() - 1) {
		if (isFirst) {
			cout << '[';
			isFirst = false;
		}
		else {
			cout << ",[";
		}
		printStack(S.getTop());
		cout << "]";
	}
	else {
		for (list* search = G.getHead(cur); search; search = search->getNext()) {
			dfs(search->getData());
		}
	}
	S.pop();
}

void printStack(list* ptr) {
	if (ptr->getNext()) {
		printStack(ptr->getNext());
	}
	if (ptr->getData() == 0) {
		cout << ptr->getData();
	}
	else {
		cout << ',' << ptr->getData();
	}
}