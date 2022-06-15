#include <iostream>
#include <vector>
using std::cin;
using std::cout;

int getMaxMoney(std::vector<int>&);

int main(){
    std::vector<int> inputData;

    int num;
    cin >> num;
    while(!cin.eof()){   // 입력
        inputData.push_back(num);
        cin >> num;
    }

    cout << getMaxMoney(inputData) << std::endl;

    return 0;
}

int getMaxMoney(std::vector<int> &input){
    if(!input.size()){   // 입력 리스트의 사이즈가 0일 경우
        return 0;
    }
    else if(input.size() == 1){   // 입력 리스트의 사이즈가 1일 경우
        return input[0];
    }
    else{
        int *arr = new int[input.size()]{};
        arr[0] = input[0];
        arr[1] = input[1];
        int index = 0;
        int max = arr[index];
        for(int i = 2; i < input.size(); i++){   // index가 i보다 2칸 앞이기 때문에 연속되지않음
            arr[i] = max+input[i];
            if(max < arr[++index]){
                max = arr[index];
            }
        }
        if(max < arr[++index]){
            max = arr[index];
        }
        return max;
    }
}