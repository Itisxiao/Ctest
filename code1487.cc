#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<string> getFolderNames(vector<string>& names) {
        unordered_map<string,int> m;
        vector<string> res;
        for(const auto& name: names){
            if(m.find(name) == m.end()){
                res.push_back(name);
                m[name] = 1;
            } else {
                int i = m[name];
                while(m.find(name + "(" + to_string(i) + ")") != m.end()) {
                    i++;
                }
                string newName = name + "(" + to_string(i) + ")";
                res.push_back(newName);
                m[name] = i + 1;
                m[newName] = 1;
            }
        }

        return res;
    }
};

int main() {
    // TODO: Write your code here
    Solution sol;
     vector<string> names = {"gta","gta(1)","gta","avalon"};
    vector<string> result = sol.getFolderNames(names);
    cout << "The unique folder names are: ";
    for(const auto& name: result) {
        cout << name << " ";
    }
    cout << endl;
    return 0;
}