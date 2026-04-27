#include <iostream>
#include <string>
#include <algorithm>
using namespace std;    

string toBinary(int n) {
    if (n == 0) return "0";
    
    string binary = "";
    bool isNegative = n < 0;
    unsigned int num = isNegative ? static_cast<unsigned int>(n) : n;
    
    while (num > 0) {
        binary += (num % 2) ? '1' : '0';
        num /= 2;
    }
    
    reverse(binary.begin(), binary.end());
    return binary;
}

int main() {
    cout << toBinary(42) << endl;   // 101010
    cout << toBinary(0) << endl;    // 0
    cout << toBinary(-1) << endl;   // 11111111111111111111111111111111 (补码)
    
    return 0;
}