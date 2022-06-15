#include <iostream>
#include <string>
#include <queue>
using namespace std;

class element {
	int key, value;
	element* lchild, * rchild;

public:
	element(int, int);

	int getKey() const;
	int getValue() const;
	void setValue(int);
	element* getLeftChild() const;
	void setLeftChild(element*);
	element* getRightChild() const;
	void setRightChild(element*);
};

class bst {
	element* root;

public:
	bst();

	void addKey(int, int);
	int findKey(int) const;
	void removeKey(int);
};

class MyHashMap {
	int base;
	vector<bst*> hashMap;

public:
	MyHashMap(int);

	void put(int, int);
	int get(int);
	void remove(int);
};

int main(void) {
	queue<string> str;
	queue<int> number;

	string temp;
	char ch;
	cin >> ch;
	while (ch != ']') {
		if (ch == '"') {
			cin >> ch;
			while (ch != '"') {
				temp.push_back(ch);
				cin >> ch;
			}
			str.push(temp);
			temp.clear();
		}
		cin >> ch;
	}
	cin >> ch;
	int openCount = 0;
	while (true) {
		if (ch == '[') {
			openCount++;
		}
		if (ch >= '0' && ch <= '9') {
			temp.push_back(ch);
		}
		else {
			if (temp.size()) {
				int n = stoi(temp);
				number.push(n);
				temp.clear();
			}
			if (ch == ']') {
				openCount--;
				if (!openCount) {
					break;
				}
			}
		}
		cin >> ch;
	}

	cout << endl << "output" << endl << endl;
	MyHashMap* myHashMap = NULL;
	int k, v;
	cout << "[";
	while (!str.empty()) {
		temp = str.front();
		str.pop();
		switch (temp[0]) {
		case 'M':
			myHashMap = new MyHashMap(769);
			cout << "null";
			break;
		case 'p':
			k = number.front();
			number.pop();
			v = number.front();
			number.pop();
			myHashMap->put(k, v);
			cout << ", null";
			break;
		case 'g':
			k = number.front();
			number.pop();
			cout << ", ";
			if (myHashMap->get(k) != -1) {
				cout << "true";
			}
			else {
				cout << "false";
			}
			break;
		case 'r':
			k = number.front();
			number.pop();
			myHashMap->remove(k);
			cout << ", null";
			break;
		}
	}
	cout << "]" << endl;

	return 0;
}

element::element(int a, int b) : key(a), value(b), lchild(NULL), rchild(NULL) {

}

int element::getKey() const {
	return key;
}

int element::getValue() const {
	return value;
}

void element::setValue(int value) {
	this->value = value;
}

element* element::getLeftChild() const {
	return lchild;
}

void element::setLeftChild(element* ptr) {
	lchild = ptr;
}

element* element::getRightChild() const {
	return rchild;
}

void element::setRightChild(element* ptr) {
	rchild = ptr;
}

bst::bst() : root(NULL) {

}

void bst::addKey(int key, int value) {
	element* temp = new element(key, value);

	if (!root) {
		root = temp;
	}
	else {
		for (element* search = root; search;) {
			if (search->getKey() == key) {
				search->setValue(value);
				delete temp;
				search = NULL;
			}
			else if (search->getKey() > key) {
				if (!search->getLeftChild()) {
					search->setLeftChild(temp);
					search = NULL;
				}
				else {
					search = search->getLeftChild();
				}
			}
			else {
				if (!search->getRightChild()) {
					search->setRightChild(temp);
					search = NULL;
				}
				else {
					search = search->getRightChild();
				}
			}
		}
	}
}

int bst::findKey(int key) const {
	for (element* search = root; search;) {
		if (search->getKey() == key) {
			return search->getValue();
		}
		else if (search->getKey() > key) {
			search = search->getLeftChild();
		}
		else {
			search = search->getRightChild();
		}
	}

	return -1;
}

void bst::removeKey(int key) {
	element* parent = NULL;
	for (element* search = root; search;) {
		if (search->getKey() == key) {
			if (search == root) {
				if (!search->getLeftChild() && !search->getRightChild()) {
					root = NULL;
				}
				else if (!search->getLeftChild()) {
					root = root->getRightChild();
				}
				else if (!search->getRightChild()) {
					root = root->getLeftChild();
				}
				else {
					element* leaf, * leaf_parent = search;
					for (leaf = search->getRightChild(); leaf->getLeftChild(); leaf_parent = leaf, leaf = leaf->getLeftChild());
					if (leaf_parent != search) {
						leaf_parent->setLeftChild(leaf->getRightChild());
						leaf->setRightChild(search->getRightChild());
					}
					leaf->setLeftChild(search->getLeftChild());
					root = leaf;
				}
			}
			else {
				if (!search->getLeftChild() && !search->getRightChild()) {
					if (parent->getKey() > key) {
						parent->setLeftChild(NULL);
					}
					else {
						parent->setRightChild(NULL);
					}
				}
				else if (!search->getLeftChild()) {
					if (parent->getKey() > key) {
						parent->setLeftChild(search->getRightChild());
					}
					else {
						parent->setRightChild(search->getRightChild());
					}
				}
				else if (!search->getRightChild()) {
					if (parent->getKey() > key) {
						parent->setLeftChild(search->getLeftChild());
					}
					else {
						parent->setRightChild(search->getLeftChild());
					}
				}
				else {
					element* leaf, * leaf_parent = search;
					for (leaf = search->getRightChild(); leaf->getLeftChild(); leaf_parent = leaf, leaf = leaf->getLeftChild());
					if (leaf_parent != search) {
						leaf_parent->setLeftChild(leaf->getRightChild());
						leaf->setRightChild(search->getRightChild());
					}
					leaf->setLeftChild(search->getLeftChild());
					if (parent->getKey() > key) {
						parent->setLeftChild(leaf);
					}
					else {
						parent->setRightChild(leaf);
					}
				}
			}
			delete search;
			search = NULL;
		}
		else if (search->getKey() > key) {
			parent = search;
			search = search->getLeftChild();
		}
		else {
			parent = search;
			search = search->getRightChild();
		}
	}
}

MyHashMap::MyHashMap(int prime) : base(prime), hashMap(base, NULL) {

}

void MyHashMap::put(int key, int value) {
	int bucket = key % base;

	if (!hashMap[bucket]) {
		bst* newTree = new bst();
		hashMap[bucket] = newTree;
	}
	hashMap[bucket]->addKey(key, value);
}

int MyHashMap::get(int key) {
	int bucket = key % base;

	if (!hashMap[bucket]) {
		return -1;
	}
	return hashMap[bucket]->findKey(key);
}

void MyHashMap::remove(int key) {
	int bucket = key % base;
	if (hashMap[bucket]) {
		hashMap[bucket]->removeKey(key);
	}
}