#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int secondHighest(const string &s)
{
    int first = -1, second = -1;
    for (auto c : s)
    {
        if (isdigit(c))
        {
            int num = c - '0';
            cout<<num<<" ";
            if (num > first)
            {
                second = first;
                first = num;
            }
            else if (num < first && num > second)
            {
                second = num;
            }
        }
    }
    return second;
}

int main()
{
    // TODO: Write your code here"
    string s="abc1113";
    cout<<secondHighest(s)<<endl;

    return 0;
}