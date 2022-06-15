#include <iostream>
using std::cin;
using std::cout;

double pow(double, int, double);

int main(void) {
	double x;
	int n;

	cout << "input : x = ";
	cin >> x;
	cout << "n = ";
	cin >> n;

	if (n > 0) {
		cout << "output : " << pow(x, n, 1) << std::endl;
	}
	else {
		cout << "output : " << pow(1 / x, -n, 1) << std::endl;
	}
	return 0;
}

double pow(double x, int n, double result) {
	if (!n) {
		return result;
	}

	return pow(x, n - 1, result * x);
}