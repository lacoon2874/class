#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cin;
using std::cout;
using std::vector;
using std::string;

class list{
    int data;
    list*next;

public:
    list(int);

    int getData() const;
    list* getNext() const;
    void setNext(list*);
};

class graph{
    const int size;
    vector<list*> head, tail;

public:
    graph(int);

    int getSize() const;
    list* getHead(int) const;
    list* getTail(int) const;
    void addEdge(int, int);
};

vector<int> solution(int, vector<vector<int>>&);

int main(void){
    vector<vector<int>> edge;
    int n;

    cout << "numCourses = ";
    cin >> n;

    cout << "prerequisites = ";
    char ch;
    cin>>ch;
    int openCount = 0;
    vector<int> temp;
    string str;
    while(true){
        if(ch == '['){
            openCount++;
        }
        else if(ch == ']'){
            openCount--;
            if(openCount){
                if(str.length()){
                    temp.push_back(std::stoi(str));
                    str.clear();
                }
                edge.push_back(temp);
                temp.clear();
            }
            else{
                break;
            }
        }
        else if(ch >= '0' && ch <= '9'){
            str.push_back(ch);
        }
        else{
            if(str.length()){
                temp.push_back(std::stoi(str));
                str.clear();
            }
        }
        cin >> ch;
    }

    vector<int> retVal = solution(n, edge);
    cout << "order : [" << retVal[0];
    for(int i = 1; i < retVal.size(); i++){
        cout << ',' << retVal[i];
    }
    cout << ']' << std::endl;

    return 0;
}

list::list(int n) : data(n), next(NULL) {

}

int list::getData() const {
    return data;
}

list* list::getNext() const {
    return next;
}

void list::setNext(list* ptr) {
    next = ptr;
}

graph::graph(int n) : size(n), head(vector<list*>(size, NULL)), tail(vector<list*>(size, NULL)) {

}

int graph::getSize() const {
    return size;
}

list* graph::getHead(int idx) const {
    return head[idx];
}

list* graph::getTail(int idx) const {
    return tail[idx];
}

void graph::addEdge(int a, int b) {
    list* temp = new list(b);

    if(!head[a]){
        head[a] = temp;
    }
    else{
        tail[a]->setNext(temp);
    }
    tail[a] = temp;
}

vector<int> solution(int n, vector<vector<int>>& edge){
    vector<int> result;

    graph graphObj(n);

    int *inDegree = new int[graphObj.getSize()];
    for(int i = 0; i < graphObj.getSize(); i++){
        inDegree[i] = 0;
    }

    for(int i = 0; i < edge.size(); i++){
        graphObj.addEdge(edge[i][1], edge[i][0]);
        inDegree[edge[i][0]]++;
    }

    std::queue<int> Q;
    for(int i = 0; i < n; i++){
        if(inDegree[i] == 0){
            Q.push(i);
        }
    }
    int cur;
    while(!Q.empty()){
        cur = Q.front();
        result.push_back(cur);
        Q.pop();
        for(list* search = graphObj.getHead(cur); search; search = search->getNext()){
            inDegree[search->getData()]--;
            if(inDegree[search->getData()] == 0){
                Q.push(search->getData());
            }
        }
    }

    return result;
}