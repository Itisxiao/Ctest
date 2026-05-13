#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findString(vector<string>& words, string s) {
        int i=-1;
        bool flag=false;
        for(auto v: words){
            i++;
            if(v.empty()) continue;
            if(v.compare(s) == 0) {
                flag=true ;
                break;
            }
        }

        return flag? i : -1;
        
    }
};

int main() {
    // TODO: Write your code here
    vector<string> words = {"at", "", "", "", "ball", "", "", "car", "", "", "dad", "", ""};
    string s = "ball";
    Solution sol;
    int result = sol.findString(words, s);
    cout << "The index of the string '" << s << "' is: " << result << endl;
    
    return 0;
}