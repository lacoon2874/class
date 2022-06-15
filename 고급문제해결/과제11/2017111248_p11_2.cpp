#include <iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

struct Element {
	int key, capacity, flow;
	Element* pair, *next;

	Element(int, int);
};

class MaxFlow {
	int size, boySize;
	Element** head, ** tail;
    bool first;

    void printPairs(const int&, vector<int>&, const int, const int);

public:
	MaxFlow(int, int);

	void addEdge(const int&, const int&, const int&);
	int getFlow(const int&, const int&);
    void printAllPairs(const int&);
};

int main(void) {
	vector<vector<int>> input;
	vector<int> temp;
	int openCount = 0;
	string str;
	char ch;
	cout << "grid = ";
	cin >> ch;
	while (true) {
		if (ch == '[') {
			openCount++;
		}
		else if (ch >= '0' && ch <= '9') {
			str.push_back(ch);
		}
		else {
			if (str.length()) {
				temp.push_back(stoi(str));
				str.clear();
			}
			if (ch == ']') {
				openCount--;
				if (temp.size()) {
					input.push_back(temp);
					temp.clear();
				}
				if (openCount == 0) {
					break;
				}
			}
		}
		cin >> ch;
	}

	int s = 0;
	int d = input.size()+input[0].size()+1;
	MaxFlow flowObj(d+1, input.size()+1);
    for(int i = 1; i <= input.size(); i++){
        flowObj.addEdge(0, i, 1);
    }
	for (int i = 0; i < input.size(); i++) {
		for (int j = 0; j < input[i].size(); j++) {
			if (input[i][j]) {
				flowObj.addEdge(i+1, j+input.size()+1, 1);
			}
		}
	}
    for(int i = input.size()+1; i < d; i++){
        flowObj.addEdge(i, d, 1);
    }

    int maxMatch = flowObj.getFlow(s, d);
	cout << "\nMax num of couples : " << maxMatch << endl;
    cout << "All the pairs : ";
    flowObj.printAllPairs(maxMatch);
    cout << endl;

	return 0;
}

Element::Element(int a, int b) : key(a), capacity(b), flow(0), pair(NULL), next(NULL) {

}

MaxFlow::MaxFlow(int n, int bn) : size(n), boySize(bn) {
	head = new Element * [size] {NULL};
	tail = new Element * [size] {NULL};
}

void MaxFlow::addEdge(const int& a, const int& b, const int& f) {
	for (Element* search = head[a]; search; search = search->next) {
		if (search->key == b) {
			search->capacity = f;
			return;
		}
	}

	Element* newA = new Element(b, f);
	if (!head[a]) {
		head[a] = newA;
	}
	else {
		tail[a]->next = newA;
	}
	tail[a] = newA;

	Element* newB = new Element(a, 0);
	if (!head[b]) {
		head[b] = newB;
	}
	else {
		tail[b]->next = newB;
	}
	tail[b] = newB;

	newA->pair = newB;
	newB->pair = newA;
}

int MaxFlow::getFlow(const int& s, const int& d) {
	bool* visit = new bool[size];
	int* parent = new int[size];
	int* min = new int[size];

	while (true) {
		for (int i = 0; i < size; i++) {
			visit[i] = false;
			parent[i] = -1;
			min[i] = 100000000;
		}

		queue<int> bfsQ;
		bfsQ.push(s);
		visit[s] = true;
		while (!bfsQ.empty()) {
			int cur = bfsQ.front();
			bfsQ.pop();
			for (Element* search = head[cur]; search; search = search->next) {
				if (!visit[search->key] && (search->capacity - search->flow) > 0) {
					visit[search->key] = true;
					parent[search->key] = cur;
					if (min[cur] > (search->capacity - search->flow)) {
						min[search->key] = search->capacity - search->flow;
					}
					else {
						min[search->key] = min[cur];
					}
					bfsQ.push(search->key);
				}
			}
		}

		if (parent[d] != -1) {
			int c = d;
			for (int p = parent[d]; p != -1; c = p, p = parent[p]) {
				for (Element* search = head[p]; search; search = search->next) {
					if (search->key == c) {
						search->flow += min[d];
						search->pair->flow -= min[d];
						break;
					}
				}
			}
		}
		else {
			break;
		}
	}

	int result = 0;
	for (Element* search = head[s]; search; search = search->next) {
		result += search->flow;
	}
	return result;
}

void MaxFlow::printPairs(const int& maxMatch, vector<int>& visit, const int cur, const int count){
    if(cur >= size-1){
        if(count == maxMatch){
            if(first){
                cout << "[";
                first = false;
            }
            else{
                cout << ",[";
            }
            bool firstIdx = true;
            for(int i = 1; i < boySize; i++){
                if(visit[i]){
                    if(firstIdx){
                        cout << "(" << i << "," << visit[i] - boySize + 1 << ")";
                        firstIdx = false;
                    }
                    else{
                        cout << ",(" << i << "," << visit[i] - boySize + 1 << ")";
                    }
                }
            }
            cout<<"]";
        }
        return;
    }

    for(Element* search = head[cur]; search; search = search->next){
        if(search->key > 0 && search->key < boySize && !visit[search->key]){
            visit[search->key] = cur;
            printPairs(maxMatch, visit, cur+1, count+1);
            visit[search->key] = 0;
        }
    }
    printPairs(maxMatch, visit, cur+1, count);
}

void MaxFlow::printAllPairs(const int& maxMatch){
    vector<int> visit(boySize, 0);

    first = true;
    printPairs(maxMatch, visit, boySize, 0);
}