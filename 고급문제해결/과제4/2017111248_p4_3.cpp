#include <iostream>
using std::cin;
using std::cout;

int getPath(int&, int&);

int main(){
    int m, n;

    cout << "m = ";
    cin >> m;
    cout << "n = ";
    cin >> n;

    cout << "path의 수 : " << getPath(m, n) << std::endl;

    return 0;
}

int getPath(int &m, int &n){
    int **grid;

    grid = new int*[m];
    for(int i = 0; i < m; i++){
        grid[i] = new int[n]{};
    }
    grid[0][0] = 1;
    
    for(int i = 0; i < m - 1; i++){
        for(int j = 0; j < n - 1; j++){
            grid[i+1][j] += grid[i][j];
            grid[i][j+1] += grid[i][j];
        }
    }
    for(int i = 0; i < m-1; i++){
        grid[i+1][n-1] += grid[i][n-1];
    }
    for(int i = 0; i < n-1; i++){
        grid[m-1][i+1] += grid[m-1][i];
    }

    int retVal = grid[m-1][n-1];
    delete[] grid;
    return retVal;
}