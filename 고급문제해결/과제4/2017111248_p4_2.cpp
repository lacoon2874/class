#include <iostream>
using std::cin;
using std::cout;

int getMaxWay(int&, int&);

int main(){
    int n, k;

    cout << "n = ";
    cin >> n;
    cout << "k = ";
    cin >> k;

    cout << "경우의 수 : " << getMaxWay(n, k) << std::endl;

    return 0;
}

int getMaxWay(int &n, int &k){
    int *post = new int[n]{};

    post[0] = k;
    if(n > 1){
        post[1] = k*k;
    }

    for(int i = 2; i < n; i++){
        post[i] = post[i-1]*(k-1)+post[i-2]*(k-1);
    }

    int retVal = post[n-1];
    delete[] post;
    return retVal;
}