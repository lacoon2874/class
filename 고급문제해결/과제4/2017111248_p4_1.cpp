#include <iostream>
#include <vector>
using std::cin;
using std::cout;

int getMaxSqLength(std::vector<int>&);

int main() {
	std::vector<int> nums;
	int number;

	cin >> number;
	while (!cin.eof()) {
		nums.push_back(number);
		cin >> number;
	}

	cout << "가장 긴 수열의 길이 : " << getMaxSqLength(nums) << std::endl;
	
	return 0;
}

int getMaxSqLength(std::vector<int>& num) {
	int max = 1;
	int* arr = new int[num.size()];

	for (int i = 0; i < num.size(); i++) {
		arr[i] = 1;
	}

	for (int i = 1; i < num.size(); i++) {
		for (int j = 0; j < i; j++) {
			if (num[i] > num[j] && arr[j] + 1 > arr[i]) {
				arr[i] = arr[j] + 1;
			}
		}
		if (arr[i] > max) {
			max = arr[i];
		}
	}

	delete[] arr;

	return max;
}