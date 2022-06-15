#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <queue>
using std::cin;
using std::cout;
using std::vector;
using std::string;

struct element{
    int node;
    int delay;

    bool operator<(const element& e) const {
        return this->delay > e.delay;
    }
};

class list{
    int data, weight;
    list*next;

public:
    list(int, int);

    int getData() const;
    int getWeight() const;
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
    void addEdge(int, int, int);
};

int solution(vector<vector<int>>&, int, int);

int main(void){
    vector<vector<int>> edge;
    int n, k;

    cout << "times = ";
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

    cout << "n = ";
    cin >> n;
    cout << "k = ";
    cin >> k;

    cout << "delay time : " << solution(edge, n, k) << std::endl;

    return 0;
}

list::list(int n, int w) : data(n), weight(w), next(NULL) {

}

int list::getData() const {
    return data;
}

int list::getWeight() const{
    return weight;
}

list* list::getNext() const {
    return next;
}

void list::setNext(list* ptr) {
    next = ptr;
}

graph::graph(int n) : size(n+1), head(vector<list*>(size, NULL)), tail(vector<list*>(size, NULL)) {

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

void graph::addEdge(int a, int b, int w) {
    list* temp = new list(b, w);

    if(!head[a]){
        head[a] = temp;
    }
    else{
        tail[a]->setNext(temp);
    }
    tail[a] = temp;
}

int solution(vector<vector<int>>& edge, int n, int k){
    graph graphObj(n);

    for(int i = 0; i < edge.size(); i++){
        graphObj.addEdge(edge[i][0], edge[i][1], edge[i][2]);
    }

    std::numeric_limits<int> intLimit;
    int *delay = new int[graphObj.getSize()];
    bool *visit = new bool[graphObj.getSize()];
    for(int i = 1; i < graphObj.getSize(); i++){
        delay[i] = intLimit.max();
        visit[i] = false;
    }

    delay[k] = 0;
    std::priority_queue<element> pq;
    element temp = {k, delay[k]};
    pq.push(temp);
    while(!pq.empty()){
        do{
            temp = pq.top();
            pq.pop();
        }while(!visit[temp.node] && !pq.empty());
        if(!visit[temp.node]){
            visit[temp.node] = true;
            for(list* search = graphObj.getHead(temp.node); search; search = search->getNext()){
                if(delay[search->getData()] > delay[temp.node] + search->getWeight()){
                    delay[search->getData()] = delay[temp.node] + search->getWeight();
                    if(!visit[search->getData()]){
                        element qTemp = {search->getData(), delay[search->getData()]};
                        pq.push(qTemp);
                    }
                }
            }
        }
    }

    int result = -1;
    for(int i = 1; i < graphObj.getSize(); i++){
        if(delay[i] == intLimit.max()){
            return -1;
        }
        if(result < delay[i]){
            result = delay[i];
        }
    }

    return result;
}