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

void testStruct(const student& s){

    cout << "Name: " << s.name << ", Age: " << s.age << ", Grade: " << s.grade << endl;

}

int main() {
    // TODO: Write your code here
    student s1("Alice", 20, 85.5);
    testStruct(s1);
    student s2("Bob", 22, 90.0);
    testStruct(s2);

    return 0;
}