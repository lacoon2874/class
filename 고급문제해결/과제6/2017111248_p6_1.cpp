#include <iostream>
using std::cin;
using std::cout;

int main(void) {
	int n;
	cout << "n = ";
	cin >> n;

	int* parent = new int[n];
	for (int i = 0; i < n; i++) {
		parent[i] = i;
	}
	char input;
	cin >> input;
	while (input != '[') {
		cin >> input;
	}
	bool isEnd = false;
	int a, b;
	cin >> input;
	while (!isEnd) {
		if (input == '[') {
			cin >> a;
			cin >> input;
			cin >> b;
			cin >> input;

			if (a < b) {
				parent[b] = parent[a];
			}
			else {
				parent[a] = parent[b];
			}
		}
		else if (input == ']') {
			isEnd = true;
		}
		else {
			// ',' skip
		}
		cin >> input;
	}

	int before = -1;
	int answer = 0;
	for (int i = 0; i < n; i++) {
		if (before != parent[i]) {
			before = parent[i];
			answer++;
		}
	}

	cout << answer << std::endl;
	
	return 0;
}