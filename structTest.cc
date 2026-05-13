#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct student {
    string name;
    int age;
    double grade;
    student(string n, int a, double g) : name(n), age(a), grade(g) {}
    
};

int main() {
    // TODO: Write your code here
    student* s1 = new student("Alice", 20, 85.5);
    cout << "Name: " << s1->name << ", Age: " << s1->age << ", Grade: " << s1->grade << endl;
    student* s2 = new student("Bob", 22, 90.0);
    cout << "Name: " << s2->name << ", Age: " << s2->age << ", Grade: " << s2->grade << endl;

    delete s1;  
    delete s2;
    
    return 0;
}