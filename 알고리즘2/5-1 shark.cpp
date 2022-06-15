#include <iostream>
#include <fstream>
using std::cin;
using std::cout;

int n;
int answer = 0;

class index {
public:
	int row, col;
};

class list {
	index data;
	list* next;

public:
	list(index, list*);

	index getData() const;
	list* getNext() const;
	void setNext(list*);
};

class queue {
	list* front = NULL, * rear = NULL;

public:
	void enqueue(const index&);
	void dequeue();
	index frontQueue() const;
	bool notEmpty() const;
};

class bfs {
	char** color;
	int** d;
	queue Q;

public:
	bfs();

	index search(index, int**, int);
};

class shark {
	std::ifstream fp;
	int** arr;
	bfs* bfs_obj;
	index shark_index;
	int size = 2, count = 0;

	void inputData();
	void eatFish();

public:
	void func();
};

list::list(index a, list* l) : next(l) {
	data.row = a.row;
	data.col = a.col;
}

index list::getData() const {
	return data;
}

list* list::getNext() const {
	return next;
}

void list::setNext(list* a) {
	next = a;
}

void queue::enqueue(const index &u) {
	list* temp = new list(u, NULL);

	if (!front) {
		front = temp;
	}
	else {
		rear->setNext(temp);
	}
	rear = temp;
}

void queue::dequeue() {
	list* temp;

	temp = front;

	front = front->getNext();

	delete temp;
}

index queue::frontQueue() const {
	return front->getData();
}

bool queue::notEmpty() const {
	if (front) {
		return true;
	}
	return false;
}

bfs::bfs() {
	color = new char* [n];
	d = new int* [n];
	for (int i = 0; i < n; i++) {
		color[i] = new char[n];
		d[i] = new int[n];
	}
}

index bfs::search(index s, int** arr, int size) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			color[i][j] = 'W';
		}
	}
	color[s.row][s.col] = 'G'; d[s.row][s.col] = 0;

	
	Q.enqueue(s);
	
	index u;
	while (Q.notEmpty()) {
		u = Q.frontQueue();

		if (arr[u.row][u.col] > 0 && arr[u.row][u.col] < size) {
			index ret_val = u;
			index temp;

			answer += d[u.row][u.col];
			Q.dequeue();

			while (Q.notEmpty()) {
				temp = Q.frontQueue();

				if (d[temp.row][temp.col] > d[u.row][u.col]) {
					break;
				}

				if (arr[temp.row][temp.col] > 0 && arr[temp.row][temp.col] < size) {
					if (temp.row < ret_val.row) {
						ret_val = temp;
					}
					else if (temp.row == ret_val.row && temp.col < ret_val.col) {
						ret_val = temp;
					}
				}

				Q.dequeue();
			}
			while (Q.notEmpty()) {
				Q.dequeue();
			}

			return ret_val;
		}

		index temp;
		temp.row = u.row - 1; temp.col = u.col;
		if (temp.row > -1 && color[temp.row][temp.col] == 'W' && arr[temp.row][temp.col] <= size) {
			color[temp.row][temp.col] = 'G';
			d[temp.row][temp.col] = d[u.row][u.col] + 1;
			Q.enqueue(temp);
		}

		temp.row = u.row + 1; temp.col = u.col;
		if (temp.row < n && color[temp.row][temp.col] == 'W' && arr[temp.row][temp.col] <= size) {
			color[temp.row][temp.col] = 'G';
			d[temp.row][temp.col] = d[u.row][u.col] + 1;
			Q.enqueue(temp);
		}

		temp.row = u.row; temp.col = u.col - 1;
		if (temp.col > -1 && color[temp.row][temp.col] == 'W' && arr[temp.row][temp.col] <= size) {
			color[temp.row][temp.col] = 'G';
			d[temp.row][temp.col] = d[u.row][u.col] + 1;
			Q.enqueue(temp);
		}

		temp.row = u.row; temp.col = u.col + 1;
		if (temp.col < n && color[temp.row][temp.col] == 'W' && arr[temp.row][temp.col] <= size) {
			color[temp.row][temp.col] = 'G';
			d[temp.row][temp.col] = d[u.row][u.col] + 1;
			Q.enqueue(temp);
		}

		Q.dequeue();

		color[u.row][u.col] = 'B';
	}

	index temp;

	temp.row = n;
	temp.col = n;

	return temp;
}

//shark
void shark::inputData() {
	fp.open("input1.txt");

	fp >> n;
	bfs_obj = new bfs();

	arr = new int* [n];
	for (int i = 0; i < n; i++) {
		arr[i] = new int[n];
		for (int j = 0; j < n; j++) {
			fp >> arr[i][j];
			if (arr[i][j] == 9) {
				shark_index.row = i;
				shark_index.col = j;
			}
		}
	}
}

void shark::eatFish() {
	index ret_val;

	ret_val = bfs_obj->search(shark_index, arr, size);
	while (ret_val.row < n) {
		count++;
		if (count == size) {
			count = 0;
			size++;
		}
		
		arr[shark_index.row][shark_index.col] = 0;
		arr[ret_val.row][ret_val.col] = 9;
		shark_index = ret_val;

		ret_val = bfs_obj->search(shark_index, arr, size);
	}

	cout << answer << '\n';
}

void shark::func() {
	inputData();
	eatFish();
}

int main() {
	shark shark_obj;

	shark_obj.func();

	return 0;
}