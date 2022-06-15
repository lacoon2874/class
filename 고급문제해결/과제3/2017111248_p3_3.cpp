#include <iostream>
#include <string>
#include <algorithm>
using std::cin;
using std::cout;

int getLevenDist(std::string&, std::string&);
int getMin(int, int, int);   // 세 숫자중에 최소값 반환

int main(){
    std::string first, second;

    cin >> first >> second;

    cout << "레벤슈타인 거리는 " << getLevenDist(first, second) << std::endl;

    return 0;
}

int getLevenDist(std::string &str1, std::string &str2){
    int *arr[2];
    arr[0] = new int[str1.length()+1];
    arr[1] = new int[str1.length()+1];

    std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);   // 비교를 위해 소문자로 변환
    std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);   // 소문자로 변환
    
    for(int i = 0; i <= str1.length(); i++){   // 첫줄 0~n으로 갱신
        arr[0][i] = i;
    }
    for(int i = 0; i < str2.length(); i++){
        if(i%2){
            arr[0][0] = arr[1][0]+1;   // 첫칸 갱신
            for(int j = 1; j <= str1.length(); j++){
                if(str1[j-1] == str2[i]){    // 같은 문자일 경우 i-1, j-1값 가져오기
                    arr[0][j] = arr[1][j-1];
                }
                else{   // 같은 문자가 아닐 경우 앞의 세 숫자중에 최소값+1
                    arr[0][j] = getMin(arr[1][j-1], arr[1][j], arr[0][j-1])+1;
                }
            }
        }
        else{
            arr[1][0] = arr[0][0]+1;
            for(int j = 1; j <= str1.length(); j++){
                if(str1[j-1] == str2[i]){
                    arr[1][j] = arr[0][j-1];
                }
                else{
                    arr[1][j] = getMin(arr[0][j-1], arr[0][j], arr[1][j-1])+1;
                }
            }
        }
    }

    if(str2.length()%2){
        return arr[1][str1.length()];
    }
    else{
        return arr[0][str1.length()];
    }
}

int getMin(int a, int b, int c){
    if(a < b){
        if(a < c){
            return a;
        }
        else{
            return c;
        }
    }
    else{
        if(b < c){
            return b;
        }
        else{
            return c;
        }
    }
}