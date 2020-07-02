#include <iostream>
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

vector<vector<Edge>> G, inverseG; // G为AOE网络，inverseG为G反图
vector<int> inDegree; // 入度值表
map<int, int> hashIndex, inverseHashIndex; // hashIndex为下标编号，inverseHashIndex为编号下标
int n, m; // n点数，m边数
vector<int> topo; // 拓扑序

void init() {
    cin >> n >> m; // 读入点数、边数
    inDegree.clear();
    inDegree.resize(n, 0); // 初始化入读值表长为n，初始值为0
    G.clear();
    G.resize(n); // 初始化图为n个节点
    inverseG.clear();
    inverseG.resize(n); // 初始化反图为n个节点
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
        inverseG[e].push_back(Edge(s, w)); // 构反图
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

int main() {
    init();
    if (topSort()) {
        for (auto &it:topo) cout << it << " ";
        cout << endl;
    }
    return 0;
}
