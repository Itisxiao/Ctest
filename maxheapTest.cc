#include <iostream>
#include <queue>

int main() {
    // 创建一个整型优先队列
    std::priority_queue<int> pq;

    // 向优先队列中添加元素
    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);

    // 输出队列中的元素
    std::cout << "队列中的元素：" << std::endl;
    while (!pq.empty()) {
        std::cout << pq.top() << std::endl;
        pq.pop();
    }

    return 0;
}