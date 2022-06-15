#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
using namespace std;

struct Element {
	int key, capacity, flow;
	Element* pair, *next;

	Element(int, int);
};

class MaxFlow {
	int size;
	Element** head, ** tail;

	void minCut(const int&, const int&, vector<bool>&, const int&, int&, vector<bool>&);

public:
	MaxFlow(int);

	void addEdge(const int&, const int&, const int&);
	int getFlow(const int&, const int&);
	vector<pair<int, int>> getMinCut(const int&, const int&);
};

int main(void) {
	vector<vector<int>> input;
	vector<int> temp;
	int openCount = 0;
	string str;
	char ch;
	cout << "graph = ";
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
	int d = input.size() - 1;
	MaxFlow flowObj(input.size());
	for (int i = 0; i < input.size(); i++) {
		for (int j = 0; j < input[i].size(); j++) {
			if (input[i][j]) {
				flowObj.addEdge(i, j, input[i][j]);
			}
		}
	}

	cout << "\nMax flow : " << flowObj.getFlow(s, d) << endl;
	vector<pair<int, int>> cut = flowObj.getMinCut(s, d);

	cout << "Min Cut : ";
	if (cut.size()) {
		cout << "(" << cut[0].first << "," << cut[0].second << ")";
	}
	for (int i = 1; i < cut.size(); i++) {
		cout << ", " << "(" << cut[i].first << "," << cut[i].second << ")";
	}
	cout << endl;

	return 0;
}

Element::Element(int a, int b) : key(a), capacity(b), flow(0), pair(NULL), next(NULL) {

}

MaxFlow::MaxFlow(int n) : size(n) {
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

vector<pair<int, int>> MaxFlow::getMinCut(const int& s, const int& d) {
	vector<bool> startSet(size, false);
	startSet[s] = true;

	int min = 1000000000;
	vector<bool> result;
	minCut(s, d, startSet, 0, min, result);

	vector<pair<int, int>> minCutNodes;
	for (int i = 0; i < size; i++) {
		if (result[i]) {
			for (Element* search = head[i]; search; search = search->next) {
				if (!result[search->key] && search->capacity) {
					pair<int, int> temp = { i, search->key };
					minCutNodes.push_back(temp);
				}
			}
		}
	}

	return minCutNodes;
}

void MaxFlow::minCut(const int& s, const int& d, vector<bool>& startSet, const int& cur, int& min, vector<bool>& result) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		if (startSet[i]) {
			for (Element* search = head[i]; search; search = search->next) {
				if (!startSet[search->key]) {
					sum += search->capacity;
				}
			}
		}
	}
	if (sum < min) {
		min = sum;
		result = startSet;
	}

	for (int i = cur; i < size; i++) {
		if (i != s && i != d) {
			startSet[i] = true;
			minCut(s, d, startSet, i + 1, min, result);
			startSet[i] = false;
		}
	}
}