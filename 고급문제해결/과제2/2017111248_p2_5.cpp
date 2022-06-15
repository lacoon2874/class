#include <iostream>
using std::cin;
using std::cout;

int main(void) {
	int stair[46] = { 0, };
	int input;

	cout << "input : ";
	cin >> input;

	stair[0] = 1;
	for (int i = 0; i < input; i++) {
		stair[i + 1] += stair[i];
		if (i + 2 <= input) {
			stair[i + 2] += stair[i];
		}
	}

	cout << "output : " << stair[input] << std::endl;

	return 0;
}