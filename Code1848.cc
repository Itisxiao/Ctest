#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;



class Solution {
public:
    int getMinDistance(vector<int>& nums, int target, int start) {
        int size = nums.size();
        int minDistance = INT_MAX;
        for(int i=0;i<size;i++){
            if(nums[i] == target){
                minDistance = min(minDistance, abs(i - start));
            }
        }

        return minDistance;
    }
};

int main(int argc, char const *argv[])
{
    /* code */
    Solution test;
    vector<int> testAray={1,3,4,5,6,7,9,10,4,5};
    int result = test.getMinDistance(testAray,5,6);
    cout << result << endl;
    return 0;
}
