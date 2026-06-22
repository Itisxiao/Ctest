#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Student {
    string name;
    int age;
    int grade;
};

// 1. 先在外面老老实实定义一个普通函数
bool compareByGrade(const Student &a, const Student &b) {
    return a.grade > b.grade; 
}

int main() {
    // TODO: Write your code here
    vector<Student> students = {
        {"Alice", 20, 85},
        {"Bob", 22, 90},
        {"Charlie", 21, 78}
    };
    sort(students.begin(), students.end(), compareByGrade);
    for (const auto &student : students) {
        cout << student.name << " - Age: " << student.age << ", Grade: " << student.grade << endl;
    }
    return 0;
}