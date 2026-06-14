#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

// int main() {
//     // TODO: Write your code here
//     random_device rd;
//     mt19937 gen(rd());
//     int n=5;
//     vector<int> random_numbers(n);
//     for (int i = 0; i < n; ++i) {
//         random_numbers[i] = gen() % 100; // 生成0到99之间的随机数
//     }
//     cout << "生成的随机数为: ";
//     for (const auto& num : random_numbers) {
//         cout << num << " ";     
//     }
//     cout << endl;
//     return 0;
// }


int main() {
    // 创建随机数生成器
    std::mt19937 generator;

    // 创建一个均匀分布的随机数生成器，范围从 1 到 10
    std::uniform_int_distribution<int> distribution(1, 10);

    // 生成并打印 5 个随机数
    for (int i = 0; i < 5; ++i) {
        std::cout << "Random number: " << distribution(generator) << std::endl;
    }

    return 0;
}