#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


class Student{
public:
    int age;
    string name;
    double grade;
    Student (int a, string n, double g) : age(a), name(n), grade(g) {};
    ~Student ( ) {} ;
    
};

int main() {
    // TODO: Write your code here
    //stack allocation
    Student student1(20, "Alice", 85.5);
    cout << "Name: " << student1.name << ", Age: " << student1.age << ", Grade: " << student1.grade << endl;
    //heap allocation
    Student* student2 = new Student(22, "Bob", 90.0);
    cout << "Name: " << student2->name << ", Age: " << student2->age << ", Grade: " << student2->grade << endl;
    delete student2;
    return 0;
}

