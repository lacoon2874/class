#include <iostream>
#include <vector>
using std::cin;
using std::cout;

int getPath(std::vector<std::vector<int>>&);

int main(){
    bool inputEnd = false;
    int openCount = 0;
    char ch;
    std::vector<std::vector<int>> arr;
    std::vector<int> temp;

    cout << "obstacleGrid = ";

    do{
        cin >> ch;
        switch(ch){
            case '[':
                openCount++;
                break;
            case ']':
                openCount--;
                if(!openCount){
                    inputEnd = true;
                }
                else{
                    arr.push_back(temp);
                    temp.clear();
                }
                break;
            case '0':
                temp.push_back(0);
                break;
            case '1':
                temp.push_back(1);
                break;
            default:
                // ',' 입력
                break;
        }
    }while(!inputEnd);

    cout << "path의 수 : " << getPath(arr) << std::endl;

    return 0;
}

int getPath(std::vector<std::vector<int>>& arr){
    int m = arr.size();
    int n = arr[0].size();
    int **grid;
    grid = new int*[m];
    for(int i = 0; i < m; i++){
        grid[i] = new int[n]{};
    }
    grid[0][0] = 1;
    
    for(int i = 0; i < m - 1; i++){
        for(int j = 0; j < n - 1; j++){
            if(!arr[i+1][j]){
                grid[i+1][j] += grid[i][j];
            }
            if(!arr[i][j+1]){
                grid[i][j+1] += grid[i][j];
            }
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