#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// ============================================================
// 拓扑排序 DFS 核心模板
// 输入：节点总数 n，邻接表 graph（graph[u] 存 u 的后继节点）
// 输出：{ 是否存在环, 拓扑序列（正序） }
// ============================================================
class TopoSortDFS {
private:
    // 状态定义：0-未访问, 1-访问中(当前DFS递归栈内), 2-已访问完成
    vector<int> state;
    vector<int> topo_order;
    bool has_cycle = false;

    void dfs(int u, const vector<vector<int>>& graph) {
        if (has_cycle) return; // 剪枝：全局已发现环，直接终止

        state[u] = 1; // 标记为“正在访问”
        for (int v : graph[u]) {
            if (state[v] == 0) {
                dfs(v, graph);
                if (has_cycle) return;
            } else if (state[v] == 1) {
                has_cycle = true; // 遇到同一路径上的节点 → 发现回边（环）
                return;
            }
            // state[v] == 2：其他分支已处理完毕，安全跳过
        }
        state[u] = 2;               // 标记为“已完成”
        topo_order.push_back(u);    // 后序遍历记录节点
    }

public:
    pair<bool, vector<int>> solve(int n, const vector<vector<int>>& graph) {
        state.assign(n, 0);
        topo_order.clear();
        has_cycle = false;

        // 图可能不连通，必须遍历所有节点
        for (int i = 0; i < n; ++i) {
            if (state[i] == 0) {
                dfs(i, graph);
                if (has_cycle) break;
            }
        }

        // DFS 产生的是逆拓扑序，按需反转即可得到正序
        if (!has_cycle) {
            reverse(topo_order.begin(), topo_order.end());
        }
        return {has_cycle, topo_order};
    }
};

int main() {
    // 测试 1：正常 DAG
    int n1 = 4;
    vector<vector<int>> g1 = {{1}, {2}, {3}, {}}; // 0->1->2->3
    TopoSortDFS solver;
    auto res1 = solver.solve(n1, g1);
    if (!res1.first) {
        cout << "Test1: "; for(int x:res1.second) cout << x << " "; // 输出: 0 1 2 3
        cout << "\n";
    } else {
        cout << "Test1: Graph has a cycle.\n";
    }

    // 测试 2：存在环
    int n2 = 3;
    vector<vector<int>> g2 = {{1}, {2}, {0}}; // 0->1->2->0 (环)
    auto res2 = solver.solve(n2, g2);
    if (!res2.first) {
        cout << "Test2: "; for(int x:res2.second) cout << x << " ";
        cout << "\n";
    } else {
        cout << "Test2: Graph has a cycle.\n"; // 输出: Graph has a cycle.
    }
}