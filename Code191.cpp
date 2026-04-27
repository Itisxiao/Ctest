#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution
{
public:
    string toBinary(int n)
    {
        if (n == 0)
            return "0";

        string binary = "";
        bool isNegative = n < 0;
        unsigned int num = isNegative ? static_cast<unsigned int>(n) : n;

        while (num > 0)
        {
            binary += (num % 2) ? '1' : '0';
            num /= 2;
        }

        reverse(binary.begin(), binary.end());
        return binary;
    }
    int hammingWeight(int n)
    {
        
        int count = 0;
        for (char c : toBinary(n))
        {
            if (c == '1')
                count++;
        }
        return count;
    };
    // Brian Kernighan 算法：每次将 n 与 n-1 做位与运算，直到 n 变为 0
    int hammingWeight2(int n)
    {
        unsigned int x = static_cast<unsigned int>(n);
        int count = 0;
        while (x)
        {
            x &= (x - 1);
            count++;
        }
        return count;
    }
};

int main(){
    Solution test;
    int result = test.hammingWeight(11);
    cout << result << endl; // 输出: 3 (二进制 1011 中有三个 '1')
    return 0;
}