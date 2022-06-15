#include <iostream>
#include <vector>
#include <string>
using namespace std;

class heap {
	vector<int> sticks;

	void minHeapify(int);
	void exchange(int, int);

public:
	heap();

	void addStick(int);
	void buildMinHeap();
	int getSticksSize() const;
	void insertStick(int);
	int deleteStick();
};

int main(void) {
	heap minHeap;

	int n;
	char ch;
	string str;
	cout << "sticks = ";
	cin >> ch;
	while (true) {
		if (ch >= '0' && ch <= '9') {
			str.push_back(ch);
		}
		else {
			if (str.length()) {
				n = stoi(str);
				minHeap.addStick(n);
				str.clear();
			}
			if (ch == ']') {
				break;
			}
		}
		cin >> ch;
	}
	minHeap.buildMinHeap();
	
	int sum = 0;
	while (minHeap.getSticksSize() > 2) {
		int a = minHeap.deleteStick();
		int b = minHeap.deleteStick();
		int newStick = a + b;

		sum += newStick;
		minHeap.insertStick(newStick);
	}

	cout << "cost : " << sum << endl;

	return 0;
}

void heap::minHeapify(int idx) {
	int l = idx * 2;
	int r = l + 1;
	int min;
	if (l < sticks.size() && sticks[l] < sticks[idx]) {
		min = l;
	}
	else {
		min = idx;
	}
	if (r < sticks.size() && sticks[r] < sticks[min]) {
		min = r;
	}
	if (min != idx) {
		exchange(min, idx);
		minHeapify(min);
	}
}

void heap::exchange(int a, int b) {
	int temp = sticks[a];
	sticks[a] = sticks[b];
	sticks[b] = temp;
}

heap::heap() {
	sticks.push_back(0);
}

void heap::addStick(int s) {
	sticks.push_back(s);
}

void heap::buildMinHeap() {
	for (int i = (sticks.size() - 1) / 2; i >= 1; i--) {
		minHeapify(i);
	}
}

int heap::getSticksSize() const {
	return sticks.size();
}

void heap::insertStick(int s) {
	addStick(s);
	for (int i = sticks.size() - 1; i > 1; i /= 2) {
		if (sticks[i / 2] > sticks[i]) {
			exchange(i / 2, i);
		}
		else {
			break;
		}
	}
}

int heap::deleteStick() {
	exchange(1, sticks.size() - 1);
	int retVal = sticks.back();
	sticks.pop_back();
	minHeapify(1);
	return retVal;
}