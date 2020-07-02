#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <string>
#include <map>

using namespace std;

struct Edge {
    int next; // ���յ�
    int weight; // ��Ȩ

    Edge() = default; // Ĭ�Ϲ��캯��

    Edge(int next, int weight) : next(next), weight(weight) {} // ���ع��캯��
};

vector<vector<Edge>> G; // GΪAOE����
vector<int> inDegree; // ���ֵ��
map<int, int> hashIndex, inverseHashIndex; // hashIndexΪ�±��ţ�inverseHashIndexΪ����±�
int n, m; // n������m����
vector<int> topo; // ������
vector<int> vertexEarly, vertexLate;
string filename;

void init() {
    ifstream in("aoe.txt");
    in >> n >> m; // �������������
//    cout<<n<<" "<<m<<endl;
    inDegree.clear();
    inDegree.resize(n, 0); // ��ʼ�����ֵ��Ϊn����ʼֵΪ0
    G.clear();
    G.resize(n); // ��ʼ��ͼΪn���ڵ�
    hashIndex.clear();
    inverseHashIndex.clear();
    for (int i = 0, t; i < n; i++) {
        in >> t;
        hashIndex[t] = i; // ��������±�
        inverseHashIndex[i] = t; // �±��������
    }
    for (int i = 0, a, b, s, e, w; i < m; i++) {
        in >> a >> b >> w; // cin>>�����>>�յ���>>��Ȩ
        s = hashIndex[a]; // ��������±�
        e = hashIndex[b]; // �����յ��±�
        inDegree[e]++; // �յ����+1
        G[s].push_back(Edge(e, w)); // ��ͼ
    }
    in.close();
}

bool topSort() {
    stack<int> s; // ����ջ
    for (int i = 0; i < n; i++) if (!inDegree[i]) s.push(i); // ���Ϊ0ѹ��ջ
    while (!s.empty()) {
        int temp = s.top();
        s.pop();
        topo.push_back(temp); // ջ��Ԫ�س�ջ�Ҽ���������β
        // �����õ�Ϊ���ıߣ������߶�Ӧ�յ����-1
        for (int i = 0, next; i < G[temp].size(); i++) {
            next = G[temp][i].next;
            inDegree[next]--;
            if (inDegree[G[temp][i].next] == 0) s.push(next); // ��ȼ�Ϊ0��ѹ��ջ
        }
    }
    return topo.size() >= n; // ��������򳤶�С�ڵ�����˵�����ڻ�
}

bool criticalPath() {
    if (!topSort()) return false; // ������ڻ�ֱ�ӷ���false
    cout << "������" << endl;
    bool first = true;
    for (auto &it:topo) {
        if (first) {
            cout << it;
            first = false;
        } else cout << "->" << it;
    }
    cout << endl;
    // ����������ÿ���¼������緢��ʱ��vertexEarly
    vertexEarly.resize(n, 0);
    for (auto &it:topo) {
        for (int i = 0, next, weight; i < G[it].size(); i++) {
            next = G[it][i].next;
            weight = G[it][i].weight;
            vertexEarly[next] = max(vertexEarly[it] + weight, vertexEarly[next]);
        }
    }
    cout << "���¼����緢��ʱ��" << endl;
    first = true;
    for (auto &it:vertexEarly) {
        if (first) {
            cout << it;
            first = false;
        } else cout << " " << it;
    }
    cout << endl;
    // ����������ÿ���¼���������ʱ��vertexLate
    vertexLate.resize(n, vertexEarly[n - 1]);
    for (int i = n - 1, temp; i >= 0; i--) {
        temp = topo[i];
        for (int j = 0, next, weight; j < G[temp].size(); j++) {
            next = G[temp][j].next;
            weight = G[temp][j].weight;
            vertexLate[temp] = min(vertexLate[next] - weight, vertexLate[temp]);
        }
    }
    cout << "���¼�������ʱ��" << endl;
    first = true;
    for (auto &it:vertexLate) {
        if (first) {
            cout << it;
            first = false;
        } else cout << " " << it;
    }
    cout << endl;
    // �ж�ÿһ��Ƿ�Ϊ�ؼ��
    cout << "�ؼ��[�, ���緢��ʱ��, ������ʱ��]" << endl;
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
    cout << "��������������Ҫ" << *max_element(vertexEarly.begin(), vertexEarly.end()) << endl;
    return true;
}

void menu() {
    cout << "��ӭʹ�ùؼ�·�����ϵͳ" << endl;
    cout << "1. ��aoe.txt��������" << endl;
    cout << "0. �˳�" << endl;
    cout << "�����������ţ�";
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
                cout << "�޴˲�����" << endl;
                break;
        }
    }
}
