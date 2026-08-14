#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    ofstream outfile("data.txt");
    
    string name = "张三";
    int age = 25;
    double salary = 8000.50;
    
    // 写入各种类型
    outfile << "姓名: " << name << endl;
    outfile << "年龄: " << age << endl;
    outfile << "工资: " << salary << endl;
    
    // 格式化输出
    outfile << fixed;  // 固定小数点格式
    outfile.precision(2);  // 保留两位小数
    outfile << "格式化工资: " << salary << endl;
    
    outfile.close();
    return 0;
}
