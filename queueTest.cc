#include <iostream>
#include <queue>

int main() {
    // 创建一个整数队列
    std::queue<int> q;

    // 向队列中添加元素
    q.push(10);
    q.push(20);
    q.push(30);

    // 打印队列中的元素数量
    std::cout << "队列中的元素数量: " << q.size() << std::endl;

    // 打印队首元素
    std::cout << "队首元素: " << q.front() << std::endl;

    // 打印队尾元素
    std::cout << "队尾元素: " << q.back() << std::endl;

    // 移除队首元素
    q.pop();
    std::cout << "移除队首元素后，队首元素: " << q.front() << std::endl;

    // 再次打印队列中的元素数量
    std::cout << "队列中的元素数量: " << q.size() << std::endl;

    return 0;
}