#include <iostream>
#include <vector>
#include <stack>
#include <map>

using namespace std;

struct Edge {
    int next;
    int weight;

    Edge() = default;

    Edge(int next, int weight) : next(next), weight(weight) {}
};

vector<vector<Edge>> G, inverseG; // G为AOE网络，inverseG为G反图
vector<int> inDegree;
map<int, int> hashIndex, inverseHashIndex; // hashIndex为下标编号，inverseHashIndex为编号下标
int n, m; // n点数，m边数
vector<int> topo;

void init() {
    cin >> n >> m;
    inDegree.clear();
    inDegree.resize(n, 0);
    G.clear();
    G.resize(n);
    inverseG.clear();
    inverseG.resize(n);
    hashIndex.clear();
    inverseHashIndex.clear();
    for (int i = 0, t; i < n; i++) {
        cin >> t;
        hashIndex[t] = i;
        inverseHashIndex[i] = t;
    }
    for (int i = 0, a, b, s, e, w; i < m; i++) {
        cin >> a >> b >> w;
        s = hashIndex[a];
        e = hashIndex[b];
        inDegree[e]++;
        G[s].push_back(Edge(e, w));
        inverseG[e].push_back(Edge(s, w));
    }
}

bool topSort() {
    stack<int> s;
    for (int i = 0; i < n; i++) if (!inDegree[i]) s.push(i);
    while (!s.empty()) {
        int temp = s.top();
        s.pop();
        topo.push_back(temp);
        for (int i = 0, next; i < G[temp].size(); i++) {
            next = G[temp][i].next;
            inDegree[next]--;
            if (inDegree[G[temp][i].next] == 0) s.push(next);
        }
    }
    return topo.size() >= n;
}

int main() {
    init();
    if (topSort()) {
        for (auto &it:topo) cout << it << " ";
        cout << endl;
    }
    return 0;
}
