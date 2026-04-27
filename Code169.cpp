#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int, int> counts;
        int majority = 0, cnt = 0;
        for (int num: nums) {
            ++counts[num];
            if (counts[num] > cnt) {
                majority = num;
                cnt = counts[num];
            }
        }
        return majority;
        
    }
    int majorityElement2(vector<int>& nums){
        unordered_map<int, int> counts;
        for(auto num: nums){
            int count =0;
            if(counts.find(num) != counts.end()){
                count = counts[num];
            }
            counts[num] = count + 1;
            
        }
        int cnt=0;
        int majority = 0;
        for (const auto& pair : counts) {
            std::cout << pair.first << ": " << pair.second << std::endl;
            if(pair.second > cnt){
                cnt = pair.second;
                majority = pair.first;
            }
        }
        
        return majority;
    }
    int majorityElement3(vector<int>& nums) {
        int candidate = 0;
        int count = 0;
        
        for (int num : nums) {
            if (count == 0) {
                candidate = num;  // 更换候选人
            }
            
            // 相同则票数 +1，不同则票数 -1
            count += (num == candidate) ? 1 : -1;
        }
        
        return candidate;
    }
};

int main() {
    Solution sol;
    vector<int> nums = {2, 2, 1, 1, 1, 2, 2};
    cout << sol.majorityElement(nums) << endl; // 输出: 2
    return 0;
}