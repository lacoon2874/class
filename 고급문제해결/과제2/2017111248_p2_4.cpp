#include <iostream>
using std::cin;
using std::cout;

int pascal(int, int);

int main(void) {
    int row;

    cout << "rowindex : ";
    cin >> row;

    cout << "output : ";
    for (int i = 1; i <= row + 1; i++) {
        cout << pascal(row + 1, i) << " ";
    }

    return 0;
}

int pascal(int row, int col) {
    if (col == 1 || col == row) {
        return 1;
    }
    else {
        return pascal(row - 1, col - 1) + pascal(row - 1, col);
    }
}