#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>

using namespace std;

struct Edge {
    int next; // 边终点
    int weight; // 边权

    Edge() = default; // 默认构造函数

    Edge(int next, int weight) : next(next), weight(weight) {} // 重载构造函数
};

vector<vector<Edge>> G; // G为AOE网络
vector<int> inDegree; // 入度值表
map<int, int> hashIndex, inverseHashIndex; // hashIndex为下标编号，inverseHashIndex为编号下标
int n, m; // n点数，m边数
vector<int> topo; // 拓扑序
vector<int> vertexEarly, vertexLate;

void init() {
    cin >> n >> m; // 读入点数、边数
    inDegree.clear();
    inDegree.resize(n, 0); // 初始化入读值表长为n，初始值为0
    G.clear();
    G.resize(n); // 初始化图为n个节点
    hashIndex.clear();
    inverseHashIndex.clear();
    for (int i = 0, t; i < n; i++) {
        cin >> t;
        hashIndex[t] = i; // 编号索引下标
        inverseHashIndex[i] = t; // 下标索引编号
    }
    for (int i = 0, a, b, s, e, w; i < m; i++) {
        cin >> a >> b >> w; // cin>>起点编号>>终点编号>>边权
        s = hashIndex[a]; // 索引起点下标
        e = hashIndex[b]; // 索引终点下标
        inDegree[e]++; // 终点入读+1
        G[s].push_back(Edge(e, w)); // 构图
    }
}

bool topSort() {
    stack<int> s; // 辅助栈
    for (int i = 0; i < n; i++) if (!inDegree[i]) s.push(i); // 入读为0压入栈
    while (!s.empty()) {
        int temp = s.top();
        s.pop();
        topo.push_back(temp); // 栈顶元素出栈且加入拓扑序尾
        // 遍历该点为起点的边，将各边对应终点入度-1
        for (int i = 0, next; i < G[temp].size(); i++) {
            next = G[temp][i].next;
            inDegree[next]--;
            if (inDegree[G[temp][i].next] == 0) s.push(next); // 入度减为0，压入栈
        }
    }
    return topo.size() >= n; // 如果拓扑序长度小于点数，说明存在环
}

bool criticalPath() {
    if (!topSort()) return false; // 如果存在环直接返回false
    for (auto &it:topo) cout << it << " ";
    cout << endl;
    // 按拓扑序求每个事件的最早发生时间vertexEarly
    vertexEarly.resize(n, 0);
    for (auto &it:topo) {
        for (int i = 0, next, weight; i < G[it].size(); i++) {
            next = G[it][i].next;
            weight = G[it][i].weight;
            vertexEarly[next] = max(vertexEarly[it] + weight, vertexEarly[next]);
        }
    }
    for (auto &it:vertexEarly) cout << it << " ";
    cout << endl;
    // 按拓扑序求每个事件的最晚发生时间vertexLate
    vertexLate.resize(n, vertexEarly[n - 1]);
    for (int i = n - 1, temp; i >= 0; i--) {
        temp = topo[i];
        for (int j = 0, next, weight; j < G[temp].size(); j++) {
            next = G[temp][j].next;
            weight = G[temp][j].weight;
            vertexLate[temp] = min(vertexLate[next] - weight, vertexLate[temp]);
        }
    }
    for (auto &it:vertexLate) cout << it << " ";
    cout << endl;
    // 判断每一活动是否为关键活动
    for (int i = 0; i < n; i++) {
        for (int j = 0, next, weight, early, late; j < G[i].size(); j++) {
            next = G[i][j].next;
            weight = G[i][j].weight;
            early = vertexEarly[i];
            late = vertexLate[next] - weight;
            if (early == late) cout << "<" << inverseHashIndex[i] << ", " << inverseHashIndex[next] << ">" << endl;
        }
    }
    return true;
}

int main() {
    init();
    criticalPath();
    return 0;
}
