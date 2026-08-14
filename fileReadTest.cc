#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include<fstream>

using namespace std;

int main() {
   // 创建一个输入文件流
   string line;
   ifstream infile; 
   // 打开一个名为afile.txt的文件
   infile.open("afile.txt"); 
 
   cout << "Reading from the file" << endl; 
   while (getline(infile, line)) { 
        cout << line << endl;  // 每行原样输出
    }
 
   // 关闭打开的文件
   infile.close();
    return 0;
}