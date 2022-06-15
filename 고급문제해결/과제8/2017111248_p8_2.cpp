#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <queue>
using std::cin;
using std::cout;
using std::vector;
using std::string;

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

int solution(int, vector<vector<int>>&, int, int, int);

int main(void){
    vector<vector<int>> edge;
    int n, s, d, k;

    cout << "n = ";
    cin >> n;

    cout << "flights = ";
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

    cout << "src = ";
    cin >> s;
    cout << "dst = ";
    cin >> d;
    cout << "k = ";
    cin >> k;

    cout << "price : " << solution(n, edge, s, d, k) << std::endl;

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

int solution(int n, vector<vector<int>>& edge, int src, int dst, int k){
    graph graphObj(n);

    for(int i = 0; i < edge.size(); i++){
        graphObj.addEdge(edge[i][0], edge[i][1], edge[i][2]);
    }

    std::numeric_limits<int> intLimit;
    int *cost = new int[graphObj.getSize()];
    int *visit = new int[graphObj.getSize()];
    for(int i = 0; i < graphObj.getSize(); i++){
        cost[i] = intLimit.max();
        visit[i] = -1;
    }

    cost[src] = 0;
    visit[src] = -1;
    std::priority_queue<int, vector<int>, std::greater<int>> pq;
    for(int i = 0; i < graphObj.getSize() - 1; i++){
        for(int j = 0; j < graphObj.getSize(); j++){
            if(cost[j] < intLimit.max()){
                for(list* search = graphObj.getHead(j); search; search = search->getNext()){
                    if(cost[search->getData()] > cost[j] + search->getWeight()){
                        cost[search->getData()] = cost[j] + search->getWeight();
                        visit[search->getData()] = visit[j] + 1;
                        if(search->getData() == dst && visit[search->getData()] <= k){
                            pq.push(cost[search->getData()]);
                        }
                    }
                }
            }
        }
    }
    for(int i = 0; i < graphObj.getSize(); i++){
        for(list* search = graphObj.getHead(i); search; search = search->getNext()){
            if(cost[search->getData()] > cost[i] + search->getWeight()){
                return -1;
            }
        }
    }

    return pq.top();
}