#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

template<typename T>
class MyVector {
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    void push_back(const T& value);
    void pop_back();
    T& operator[](size_t index);
};

int main() {
    // TODO: Write your code here
    MyVector<int> vec;
    vec.push_back(5);
    vec.push_back(10);
    cout << vec[0] << " " << vec[1] << endl;
    return 0;
}