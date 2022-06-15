#include <iostream>
using std::cin;
using std::cout;

class node {
    node* next;
    int val;

public:
    node(int num) : next(NULL), val(num) {

    }

    node* getNext() const {
        return next;
    }
    void setNext(node* newNext) {
        next = newNext;
    }
    int getVal() const {
        return val;
    }
    void setVal(int newVal) {
        val = newVal;
    }
};

class list {
    node* head;
    node* tail;

public:
    list() : head(NULL), tail(NULL) {

    }

    node* getHead() const {
        return head;
    }
    void setHead(node* newHead) {
        head = newHead;
    }
    node* getTail() const {
        return tail;
    }
    void setTail(node* newTail) {
        tail = newTail;
    }

    void addNode(int val) {
        node* newNode = new node(val);
        if (!head) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->setNext(newNode);
            tail = newNode;
        }
    }
};

void swapNode(list& lst) {
    if (!lst.getHead()) {
        return;
    }
    else if (!lst.getHead()->getNext()) {
        return;
    }
    else {
        node* ptr1 = lst.getHead();
        node* ptr2 = ptr1->getNext();
        node* before = ptr1;

        lst.setHead(ptr2);
        ptr1->setNext(ptr2->getNext());
        ptr2->setNext(ptr1);

        ptr1 = ptr1->getNext();
        while (ptr1) {
            ptr2 = ptr1->getNext();
            if (!ptr2) {
                return;
            }
            before->setNext(ptr2);
            ptr1->setNext(ptr2->getNext());
            ptr2->setNext(ptr1);
            before = ptr1;
            ptr1 = ptr1->getNext();
        }
    }
}

int main(void) {
    list inputList;

    cout << "input : ";
    int num;
    cin >> num;
    while (!cin.eof()) {
        inputList.addNode(num);
        cin >> num;
    }

    swapNode(inputList);

    cout << "output : ";
    for (node* ptr = inputList.getHead(); ptr; ptr = ptr->getNext()) {
        cout << ptr->getVal() << ' ';
    }
    cout << std::endl;

    return 0;
}