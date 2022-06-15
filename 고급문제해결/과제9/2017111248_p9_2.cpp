#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct element {
	int data;
	int frequent;

	bool operator<(const element&) const;
	bool operator>(const element&) const;
};

class heap {
	vector<element> nums;

	void maxHeapify(int);
	void exchange(int, int);

public:
	heap();

	void addElement(element);
	void buildMaxHeap();
	int getNumsSize() const;
	element deleteNum();
};

int main(void) {
	heap maxHeap;

	int n, before = -1, size = 0;
	char ch;
	string str;
	cout << "nums = ";
	cin >> ch;
	while (true) {
		if (ch >= '0' && ch <= '9') {
			str.push_back(ch);
		}
		else {
			if (str.length()) {
				n = stoi(str);
				if (n != before) {
					element temp = { before, size };
					maxHeap.addElement(temp);
					size = 1;
					before = n;
				}
				else {
					size++;
				}
				str.clear();
			}
			if (ch == ']') {
				break;
			}
		}
		cin >> ch;
	}
	element temp = { before, size };
	maxHeap.addElement(temp);

	maxHeap.buildMaxHeap();

	int k;
	cout << "k = ";
	cin >> k;

	cout << "result : ";
	for (int i = 0; i < k && maxHeap.getNumsSize() > 1; i++) {
		cout << maxHeap.deleteNum().data << ' ';
	}
	cout << endl;

	return 0;
}

bool element::operator<(const element& e) const {
	return this->frequent < e.frequent;
}
bool element::operator>(const element& e) const {
	return this->frequent > e.frequent;
}

void heap::maxHeapify(int idx) {
	int l = idx * 2;
	int r = l + 1;
	int max;
	if (l < nums.size() && nums[l] > nums[idx]) {
		max = l;
	}
	else {
		max = idx;
	}
	if (r < nums.size() && nums[r] > nums[max]) {
		max = r;
	}
	if (max != idx) {
		exchange(max, idx);
		maxHeapify(max);
	}
}

void heap::exchange(int a, int b) {
	element temp = nums[a];
	nums[a] = nums[b];
	nums[b] = temp;
}

heap::heap() {

}

void heap::addElement(element e) {
	nums.push_back(e);
}

void heap::buildMaxHeap() {
	for (int i = (nums.size() - 1) / 2; i >= 1; i--) {
		maxHeapify(i);
	}
}

int heap::getNumsSize() const {
	return nums.size();
}

element heap::deleteNum() {
	exchange(1, nums.size() - 1);
	element retVal = nums.back();
	nums.pop_back();
	maxHeapify(1);
	return retVal;
}