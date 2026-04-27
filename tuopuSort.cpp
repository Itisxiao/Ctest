#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// ============================================================
// 拓扑排序 BFS (Kahn算法) 核心模板
// 输入：节点总数 n，邻接表 graph（graph[u] 包含 u 指向的所有后继节点）
// 输出：合法的拓扑序列。若存在环，则返回空数组。
// `const`是为了保证输入图不被修改，提升代码安全性和可读性。
// ============================================================
vector<int> topologicalSort(int n, const vector<vector<int>>& graph) {
    // 1️⃣ 统计每个节点的入度
    vector<int> indeg(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : graph[u]) {
            indeg[v]++;
        }
    }

    // 2️⃣ 初始化队列：将所有入度为 0 的节点加入队列
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) {
            q.push(i);
        }
    }

    // 3️⃣ BFS 逐层剥离
    vector<int> topo_order;
    topo_order.reserve(n); // 预分配内存，提升性能
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u); // 记录当前可学习的节点

        // 遍历 u 的所有后继节点 v
        for (int v : graph[u]) {
            if (--indeg[v] == 0) { // 移除 u->v 的依赖，若 v 入度归零则入队
                q.push(v);
            }
        }
    }

    // 4️⃣ 判环：若拓扑序列长度 != 节点总数，说明图中存在环
    if (topo_order.size() != n) {
        topo_order.clear(); // 按需可改为返回特定标记或抛出异常
    }
    return topo_order;
}
int main() {
    // 测试 1：正常 DAG
    int n1 = 4;
    vector<vector<int>> g1 = {{1}, {2}, {3}, {}}; // 0->1->2->3
    auto res1 = topologicalSort(n1, g1);
    cout << "Test1: "; for(int x:res1) cout << x << " "; // 输出: 0 1 2 3
    cout << "\n";

    // 测试 2：存在环
    int n2 = 3;
    vector<vector<int>> g2 = {{1}, {2}, {0}}; // 0->1->2->0 (环)
    auto res2 = topologicalSort(n2, g2);
    cout << "Test2: " << (res2.empty() ? "Has Cycle" : "Valid") << "\n"; // 输出: Has Cycle
    return 0;
}