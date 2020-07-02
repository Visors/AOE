#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <string>
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
string filename;

void init() {
    ifstream in("aoe.txt");
    in >> n >> m; // 读入点数、边数
//    cout<<n<<" "<<m<<endl;
    inDegree.clear();
    inDegree.resize(n, 0); // 初始化入读值表长为n，初始值为0
    G.clear();
    G.resize(n); // 初始化图为n个节点
    hashIndex.clear();
    inverseHashIndex.clear();
    for (int i = 0, t; i < n; i++) {
        in >> t;
        hashIndex[t] = i; // 编号索引下标
        inverseHashIndex[i] = t; // 下标索引编号
    }
    for (int i = 0, a, b, s, e, w; i < m; i++) {
        in >> a >> b >> w; // cin>>起点编号>>终点编号>>边权
        s = hashIndex[a]; // 索引起点下标
        e = hashIndex[b]; // 索引终点下标
        inDegree[e]++; // 终点入读+1
        G[s].push_back(Edge(e, w)); // 构图
    }
    in.close();
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
    cout << "拓扑序" << endl;
    bool first = true;
    for (auto &it:topo) {
        if (first) {
            cout << it;
            first = false;
        } else cout << "->" << it;
    }
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
    cout << "各事件最早发生时间" << endl;
    first = true;
    for (auto &it:vertexEarly) {
        if (first) {
            cout << it;
            first = false;
        } else cout << " " << it;
    }
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
    cout << "各事件最晚发生时间" << endl;
    first = true;
    for (auto &it:vertexLate) {
        if (first) {
            cout << it;
            first = false;
        } else cout << " " << it;
    }
    cout << endl;
    // 判断每一活动是否为关键活动
    cout << "关键活动[活动, 最早发生时间, 最晚发生时间]" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0, next, weight, early, late; j < G[i].size(); j++) {
            next = G[i][j].next;
            weight = G[i][j].weight;
            early = vertexEarly[i];
            late = vertexLate[next] - weight;
            if (early == late)
                cout << "[<" << inverseHashIndex[i] << ", " << inverseHashIndex[next] << ">], " << early << ", " << late
                     << "]" << endl;
        }
    }
    cout << "完成整项工程至少需要" << *max_element(vertexEarly.begin(), vertexEarly.end()) << endl;
    return true;
}

void menu() {
    cout << "欢迎使用关键路径求解系统" << endl;
    cout << "1. 从aoe.txt读入数据" << endl;
    cout << "0. 退出" << endl;
    cout << "请输入操作编号：";
}

int main() {
    while (true) {
        menu();
        int op;
        cin >> op;
        switch (op) {
            case 1:
                init();
                criticalPath();
                break;
            case 0:
                return 0;
            default:
                cout << "无此操作！" << endl;
                break;
        }
    }
}
