#include <iostream>
#include <vector>
using namespace std;

struct element {
	char alpha;
	double frequent;

	bool operator<(const element&) const;
	bool operator<=(const element&) const;
	bool operator>(const element&) const;
	bool operator>=(const element&) const;
};

class tree {
	element data;
	tree* lchild, * rchild;

public:
	tree(const element&);

	element getData() const;
	tree* getLchild() const;
	void setLchild(tree*);
	tree* getRchild() const;
	void setRchild(tree*);

	bool operator<(const tree&) const;
	bool operator<=(const tree&) const;
	bool operator>(const tree&) const;
	bool operator>=(const tree&) const;
};

class heap {
	vector<tree*> data;

	void minHeapify(int);
	void exchange(int, int);

public:
	heap();

	void addData(tree*);
	void buildMinHeap();
	int getDataSize() const;
	void insertData(tree*);
	tree* deleteData();
};

void printPreOrder(tree*, string, vector<string>&);

int main(void) {
	heap minHeap;

	char ch;
	double f;
	cout << "Input (Character Frequency)\n";
	for (int i = 0; i < 26; i++) {
		cin >> ch;
		while (ch < 'a' || ch > 'z') {
			cin >> ch;
		}
		cin >> f;

		element temp = { ch, f };
		tree* newTree = new tree(temp);
		minHeap.addData(newTree);
	}
	minHeap.buildMinHeap();

	while (minHeap.getDataSize() > 2) {
		tree* a = minHeap.deleteData();
		tree* b = minHeap.deleteData();
		element temp = { '.', a->getData().frequent + b->getData().frequent};
		tree* newTree = new tree(temp);
		newTree->setLchild(a);
		newTree->setRchild(b);
		minHeap.insertData(newTree);
	}

	vector<string> result(26);
	printPreOrder(minHeap.deleteData(), "", result);
	
	cout << "\nOutput (Character Huffman_code)\n";
	for (int i = 0; i < 26; i++) {
		cout << (char)(i + 'a') << ' ' << result[i] << ' ';
	}
	cout << endl;

	return 0;
}

bool element::operator<(const element& e) const {
	return this->frequent < e.frequent;
}

bool element::operator<=(const element& e) const {
	return this->frequent <= e.frequent;
}

bool element::operator>(const element& e) const {
	return this->frequent > e.frequent;
}

bool element::operator>=(const element& e) const {
	return this->frequent <= e.frequent;
}

tree::tree(const element& e) : data(e), lchild(NULL), rchild(NULL) {

}

element tree::getData() const {
	return data;
}

tree* tree::getLchild() const {
	return lchild;
}

void tree::setLchild(tree* ptr) {
	lchild = ptr;
}

tree* tree::getRchild() const {
	return rchild;
}

void tree::setRchild(tree* ptr) {
	rchild = ptr;
}

bool tree::operator<(const tree& t) const {
	return this->data < t.data;
}

bool tree::operator<=(const tree& t) const {
	return this->data <= t.data;
}

bool tree::operator>(const tree& t) const {
	return this->data > t.data;
}

bool tree::operator>=(const tree& t) const {
	return this->data >= t.data;
}

void heap::minHeapify(int idx) {
	int l = idx * 2;
	int r = l + 1;
	int min;
	if (l < data.size() && *(data[l]) <= *(data[idx])) {
		min = l;
	}
	else {
		min = idx;
	}
	if (r < data.size() && *(data[r]) <= *(data[min])) {
		min = r;
	}
	if (min != idx) {
		exchange(min, idx);
		minHeapify(min);
	}
}

void heap::exchange(int a, int b) {
	tree* temp = data[a];
	data[a] = data[b];
	data[b] = temp;
}

heap::heap() {
	element temp;
	tree* ptr = new tree(temp);
	data.push_back(ptr);
}

void heap::addData(tree* ptr) {
	data.push_back(ptr);
}

void heap::buildMinHeap() {
	for (int i = (data.size() - 1) / 2; i >= 1; i--) {
		minHeapify(i);
	}
}

int heap::getDataSize() const {
	return data.size();
}

void heap::insertData(tree* ptr) {
	addData(ptr);
	for (int i = data.size() - 1; i > 1; i /= 2) {
		if (*(data[i / 2]) > *(data[i])) {
			exchange(i / 2, i);
		}
		else {
			break;
		}
	}
}

tree* heap::deleteData() {
	exchange(1, data.size() - 1);
	tree* retVal = data.back();
	data.pop_back();
	minHeapify(1);
	return retVal;
}

void printPreOrder(tree* ptr, string str, vector<string>& result) {
	if (!(ptr->getLchild())) {
		result[ptr->getData().alpha - 'a'] = str;
	}
	else {
		printPreOrder(ptr->getLchild(), str + "0", result);
		printPreOrder(ptr->getRchild(), str + "1", result);
	}
}