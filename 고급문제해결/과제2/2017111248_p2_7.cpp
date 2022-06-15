#include <iostream>
#include <vector>
using std::cin;
using std::cout;

void printPermu(std::vector<int>&, int*, int);

int main(void) {
	int num;
	std::vector<int> arr;

	cin >> num;
	while (!cin.eof()) {
		arr.push_back(num);

		cin >> num;
	}
	int* printList = new int[arr.size()]{};

	printPermu(arr, printList, arr.size());
	
	return 0;
}

void printPermu(std::vector<int>& arr, int* printList, int remainCount) {
	if (!remainCount) {
		for (int i = arr.size() - 1; i >= 0; i--) {
			cout << printList[i] << ' ';
		}
		cout << std::endl;
	}

	for (int i = 0; i < arr.size(); i++) {
		if (arr[i]) {
			printList[remainCount - 1] = arr[i];
			arr[i] = 0;
			printPermu(arr, printList, remainCount - 1);
			arr[i] = printList[remainCount - 1];
		}
	}
}