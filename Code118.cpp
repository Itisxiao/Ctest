#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class Solution {
public:
    vector<vector<int>> generate(int numRows) {
         vector<vector<int>> triangle;
    
    for (int i = 0; i < numRows; i++) {
        // 第 i 行有 i+1 个元素
        vector<int> row(i + 1, 1);  // 先全部初始化为 1
        
        // 只计算中间的元素（首尾已经是 1 了）
        for (int j = 1; j < i; j++) {
            row[j] = triangle[i-1][j-1] + triangle[i-1][j];
        }
        
        triangle.push_back(row);
    }
    
    return triangle;
    }
};

int main() {
    Solution sol;
    int numRows = 5;
    vector<vector<int>> result = sol.generate(numRows);
    
    for (const auto& row : result) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
    
    return 0;
}   