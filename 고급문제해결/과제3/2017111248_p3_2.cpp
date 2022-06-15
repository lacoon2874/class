#include <iostream>
#include <vector>
using std::cin;
using std::cout;

int getMinCost(std::vector<int>&);

int main(){
    std::vector<int> inputData;

    int num;
    cin >> num;
    while(!cin.eof()){   // 입력
        inputData.push_back(num);
        cin >> num;
    }

    cout << getMinCost(inputData) << std::endl;

    return 0;
}

int getMinCost(std::vector<int> &input){
    if(!input.size()){  // 입력 리스트의 사이즈가 0일 경우
        return 0;
    }
    else if(input.size() == 1){  // 입력 리스트의 사이즈가 1일 경우
        return input[0];
    }
    else{
        int *arr = new int[input.size()]{};
        arr[0] = input[0];
        arr[1] = input[1];
        for(int i = 0; i < input.size()-2; i++){
            if(arr[i]+input[i+1] < arr[i+1]){  // 1칸 올라가는 경우 갱신되어있는 2칸보다 낮을경우만 갱신
                arr[i+1] = arr[i]+input[i+1];
            }
            arr[i+2] = input[i+2] + arr[i];   // 2칸 올라가는 경우
        }

        if(arr[input.size()-2] < arr[input.size()-1]){    // 마지막 전칸이 최소
            return arr[input.size()-2];
        }
        else{
            return arr[input.size()-1];   // 마지막칸이 최소
        }
    }
}