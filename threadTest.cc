#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>

using namespace std;

void printHello(string name){
    cout << "Hello, " << name << "!" << endl;
}

int main() {
    // TODO: Write your code here
    std::thread t1(printHello, "Alice");
    std::thread t2(printHello, "Bob");
    t1.join();
    t2.join();
    
    return 0;
}