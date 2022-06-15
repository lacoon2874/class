#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::string;

string printReverse(string);

int main(void) {
    string input;

    cin >> input;
    cout << printReverse(input) << std::endl;

    return 0;
}

string printReverse(string str) {
    if (!str.length()) {
        return str;
    }
    else {
        return (printReverse(str.substr(1)) + str.substr(0, 1));
    }
}