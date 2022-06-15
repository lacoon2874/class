#include <iostream>
#include <string>
#include <queue>
using namespace std;

class element {
	int data;
	element* lchild, * rchild;

public:
	element(int);

	int getData() const;
	element* getLeftChild() const;
	void setLeftChild(element*);
	element* getRightChild() const;
	void setRightChild(element*);
};

class bst {
	element* root;

public:
	bst();

	void addKey(int);
	bool findKey(int) const;
	void removeKey(int);
};

class MyHashSet {
	int base;
	vector<bst*> hashSet;

public:
	MyHashSet(int);

	void add(int);
	bool contains(int);
	void remove(int);
};

bool answer(int);

int main(void) {
	int n;
	cout << "n = ";
	cin >> n;
	
	cout << "output : ";
	if (answer(n)) {
		cout << "true" << endl;
	}
	else {
		cout << "false" << endl;
	}

	return 0;
}

element::element(int key) : data(key), lchild(NULL), rchild(NULL) {

}

int element::getData() const {
	return data;
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

void bst::addKey(int key) {
	element* temp = new element(key);

	if (!root) {
		root = temp;
	}
	else {
		for (element* search = root; search;) {
			if (search->getData() == key) {
				delete temp;
				search = NULL;
			}
			else if (search->getData() > key) {
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

bool bst::findKey(int key) const {
	for (element* search = root; search;) {
		if (search->getData() == key) {
			return true;
		}
		else if (search->getData() > key) {
			search = search->getLeftChild();
		}
		else {
			search = search->getRightChild();
		}
	}

	return false;
}

void bst::removeKey(int key) {
	element* parent = NULL;
	for (element* search = root; search;) {
		if (search->getData() == key) {
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
					if (parent->getData() > key) {
						parent->setLeftChild(NULL);
					}
					else {
						parent->setRightChild(NULL);
					}
				}
				else if (!search->getLeftChild()) {
					if (parent->getData() > key) {
						parent->setLeftChild(search->getRightChild());
					}
					else {
						parent->setRightChild(search->getRightChild());
					}
				}
				else if (!search->getRightChild()) {
					if (parent->getData() > key) {
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
					if (parent->getData() > key) {
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
		else if (search->getData() > key) {
			parent = search;
			search = search->getLeftChild();
		}
		else {
			parent = search;
			search = search->getRightChild();
		}
	}
}

MyHashSet::MyHashSet(int prime) : base(prime), hashSet(base, NULL) {

}

void MyHashSet::add(int key) {
	int bucket = key % base;

	if (!hashSet[bucket]) {
		bst* newTree = new bst();
		hashSet[bucket] = newTree;
	}
	hashSet[bucket]->addKey(key);
}

bool MyHashSet::contains(int key) {
	int bucket = key % base;

	if (!hashSet[bucket]) {
		return false;
	}
	return hashSet[bucket]->findKey(key);
}

void MyHashSet::remove(int key) {
	int bucket = key % base;
	if (hashSet[bucket]) {
		hashSet[bucket]->removeKey(key);
	}
}

bool answer(int n) {
	MyHashSet myHashSet(769);
	int sum;
	while (n != 1) {
		if (myHashSet.contains(n)) {
			return false;
		}
		myHashSet.add(n);
		sum = 0;
		for (; n; n /= 10) {
			sum += ((n % 10) * (n % 10));
		}
		n = sum;
	}

	return true;
}