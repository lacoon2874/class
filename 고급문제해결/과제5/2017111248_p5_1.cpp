#include <iostream>
#include <vector>
using std::cin;
using std::cout;

class treeNode {
	int value;
	treeNode* left, * right;

public:
	treeNode(int);

	int getValue() const;
	void setValue(const int&);
	treeNode* getLeft() const;
	void setLeft(treeNode*&);
	treeNode* getRight() const;
	void setRight(treeNode*&);
};

class property {
	treeNode* data;
	property* next;

public:
	property(treeNode*&);

	treeNode* getData() const;
	void setData(treeNode*&);
	property* getNext() const;
	void setNext(property*&);
};

class queue {
	property *head, *tail;

public:
	queue();
	
	property* getHead() const;
	void setHead(property*&);
	property* getTail() const;
	void setTail(property*&);
	void enqueue(treeNode*&);
	treeNode* dequeue();
};

class stack {
	property *top;

public:
	stack();
	
	property* getTop() const;
	void setTop(property*&);
	void push(treeNode*);
	treeNode* pop();
};

void buildBinaryTree(const std::vector<int>&, treeNode*&);
void preOrder(treeNode*&);
void inOrder(treeNode*&);
void postOrder(treeNode*&);

int main(void) {
	treeNode *root;
	std::vector<int> inputArr;

	cout << "tree : ";

	int num;
	cin >> num;
	while(!cin.eof()){
		inputArr.push_back(num);
		cin >> num;
	}

	buildBinaryTree(inputArr, root);

	preOrder(root);
	inOrder(root);
	postOrder(root);

	return 0;
}

treeNode::treeNode(int key) : value(key), left(NULL), right(NULL) {

}

int treeNode::getValue() const {
	return value;
}

void treeNode::setValue(const int& key) {
	value = key;
}

treeNode* treeNode::getLeft() const {
	return left;
}

void treeNode::setLeft(treeNode*& ptr) {
	left = ptr;
}

treeNode* treeNode::getRight() const {
	return right;
}

void treeNode::setRight(treeNode*& ptr) {
	right = ptr;
}

property::property(treeNode*& ptr) : data(ptr), next(NULL) {

}

treeNode* property::getData() const {
	return data;
}

void property::setData(treeNode*& ptr) {
	data = ptr;
}

property* property::getNext() const {
	return next;
}

void property::setNext(property*& ptr) {
	next = ptr;
}

queue::queue() : head(NULL), tail(NULL) {

}

property* queue::getHead() const {
	return head;
}

void queue::setHead(property*& ptr) {
	head = ptr;
}

property* queue::getTail() const {
	return tail;
}

void queue::setTail(property*& ptr) {
	tail = ptr;
}

void queue::enqueue(treeNode*& ptr) {
	property* newData = new property(ptr);
	if (!head) {
		head = newData;
	}
	else {
		tail->setNext(newData);
	}
	tail = newData;
}

treeNode* queue::dequeue() {
	treeNode* retVal = head->getData();

	property* delData = head;

	head = head->getNext();

	delete delData;

	return retVal;
}

stack::stack() : top(NULL){

}

property* stack::getTop() const{
	return top;
}

void stack::setTop(property*& ptr){
	top = ptr;
}

void stack::push(treeNode* ptr){
	property* newData = new property(ptr);
	if(top){
		newData->setNext(top);
	}
	top = newData;
}

treeNode* stack::pop(){
	treeNode* retVal = top->getData();

	property* temp = top;
	top = top->getNext();

	delete temp;

	return retVal;
}

void buildBinaryTree(const std::vector<int>& input, treeNode*& root) {
	queue queueObj;
	treeNode* newNode = new treeNode(input[0]);
	root = newNode;
	queueObj.enqueue(root);
	treeNode* cur = NULL;
	for (int i = 1; i < input.size(); i++) {
		cur = queueObj.dequeue();
		if(input[i] >= 0){
			newNode = new treeNode(input[i]);
			cur->setLeft(newNode);
			queueObj.enqueue(newNode);
		}
		i++;

		if (i < input.size()) {
			if(input[i] >= 0){
				newNode = new treeNode(input[i]);
				cur->setRight(newNode);
				queueObj.enqueue(newNode);
			}
		}
	}

	while(queueObj.getHead()){
		queueObj.dequeue();
	}
}

void preOrder(treeNode*& root){
	stack stackObj;

	cout << "preorder : ";

	stackObj.push(root);
	treeNode* search;
	while(stackObj.getTop()){
		search = stackObj.pop();
		cout << search->getValue() << ' ';
		if(search->getRight()){
			stackObj.push(search->getRight());
		}
		if(search->getLeft()){
			stackObj.push(search->getLeft());
		}
	}
	cout << '\n';
}

void inOrder(treeNode*& root){
	stack stackObj;

	cout << "inorder : ";

	stackObj.push(root);
	treeNode* search = root->getLeft();
	while(true){
		while(search){
			stackObj.push(search);
			search = search->getLeft();
		}
		if(stackObj.getTop()){
			search = stackObj.pop();
			cout << search->getValue() << ' ';
			search = search->getRight();
		}
		else{
			break;
		}
	}
	cout << '\n';
}

void postOrder(treeNode*& root){
	stack stackObj, printObj;

	stackObj.push(root);
	treeNode* search;
	while(stackObj.getTop()){
		search = stackObj.pop();
		if(search->getLeft()){
			stackObj.push(search->getLeft());
		}
		if(search->getRight()){
			stackObj.push(search->getRight());
		}
		printObj.push(search);
	}

	cout << "postorder : ";

	while(printObj.getTop()){
		search = printObj.pop();
		cout << search->getValue() << ' ';
	}

	cout << '\n';
}