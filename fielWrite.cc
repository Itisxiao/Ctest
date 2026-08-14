#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // 创建输出文件流
    ofstream outfile;
    
    // 打开文件（如果文件不存在会创建，存在则覆盖）
    outfile.open("output.txt");
    
    // 写入数据
    outfile << "Hello World" << endl;
    outfile << "C++ Programming" << endl;
    outfile << 123 << endl;
    outfile << 3.14 << endl;
    
    // 关闭文件
    outfile.close();
    
    cout << "写入完成！" << endl;
    return 0;
}
