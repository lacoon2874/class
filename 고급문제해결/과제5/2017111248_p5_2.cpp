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
	void setLeft(treeNode*);
	treeNode* getRight() const;
	void setRight(treeNode*);
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
	void enqueue(treeNode*);
	treeNode* dequeue();
};

treeNode* buildBinaryTree(const std::vector<int>&, const std::vector<int>&, int, int, int);
void printTree(treeNode*, int);

int main(void){
    std::vector<int> input, inArr, postArr, tree;

    int num;
    cin >> num;
    while(!cin.eof()){
        input.push_back(num);
        cin >> num;
    }
    for(int i = 0; i < input.size()/2; i++){
        inArr.push_back(input[i]);
    }
    for(int i = input.size()/2; i < input.size(); i++){
        postArr.push_back(input[i]);
    }

    input.clear();

    treeNode* root = buildBinaryTree(inArr, postArr, 0, inArr.size()-1, inArr.size()-1);
    
    cout << "tree : ";
    printTree(root, inArr.size());

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

void treeNode::setLeft(treeNode* ptr) {
	left = ptr;
}

treeNode* treeNode::getRight() const {
	return right;
}

void treeNode::setRight(treeNode* ptr) {
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

void queue::enqueue(treeNode* ptr) {
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

treeNode* buildBinaryTree(const std::vector<int>& inorder, const std::vector<int>& postorder, int in_left, int in_right, int post_right){
    if(in_left <= in_right){
        treeNode* newNode = new treeNode(postorder[post_right]);
        for(int i = in_left; i <= in_right; i++){
            if(inorder[i] == postorder[post_right]){
                newNode->setLeft(buildBinaryTree(inorder, postorder, in_left, i-1, post_right-(in_right-i)-1));
                newNode->setRight(buildBinaryTree(inorder, postorder, i+1, in_right, post_right-1));
            }
        }
        return newNode;
    }

    return NULL;
}

void printTree(treeNode* root, int size) {
	queue queueObj;
    
	queueObj.enqueue(root);
	treeNode* cur;
	for (int i = 0; i < size;) {
		cur = queueObj.dequeue();
        if(cur){
            cout << cur->getValue() << ' ';
            queueObj.enqueue(cur->getLeft());
            queueObj.enqueue(cur->getRight());
            i++;
        }
        else{
            cout << "null ";
        }
	}

	while(queueObj.getHead()){
		queueObj.dequeue();
	}
}