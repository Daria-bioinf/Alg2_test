#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <map>

using namespace std;

const int MAXN = 20;

// --------------------------------------
// Простейшая структура DSU (без оптимизаций)
// --------------------------------------
struct DSU {
    vector<int> parent;

    DSU(int n = 0) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x) return x;
        parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        int pa = find(a);
        int pb = find(b);
        if (pa != pb) parent[pb] = pa;
    }
};

int main()
{
    string inputFile, outputFile;
    cout << "Input file: ";
    cin >> inputFile;

    cout << "Output file: ";
    cin >> outputFile;

    ifstream in(inputFile);
    if (!in) {
        cout << "Cannot open input file.\n";
        return 1;
    }

    int n, m;
    in >> n >> m;

    vector<pair<int, int>> edgesG;
    edgesG.reserve(m);

    for (int i = 0; i < m; i++) {
        int u, v;
        in >> u >> v;
        edgesG.push_back({ u - 1, v - 1 }); // to 0-based
    }
    in.close();

    // --------------------------------------
    // 1) Проверка 1-графа
    // --------------------------------------
    bool oneGraph = true;
    bool hasEdge[MAXN][MAXN] = { false };
    vector< bitset<MAXN> > Nplus(n);

    for (auto e : edgesG) {
        int u = e.first;
        int v = e.second;
        if (hasEdge[u][v]) oneGraph = false;
        hasEdge[u][v] = true;
        Nplus[u].set(v);
    }

    cout << (oneGraph ? "G is 1-graph\n" : "G is NOT 1-graph\n");

    // --------------------------------------
    // 2) Проверка графа сопряжённого
    // --------------------------------------
    bool adjoint = oneGraph;

    if (adjoint) {
        for (int x = 0; x < n; x++) {
            for (int y = x + 1; y < n; y++) {
                bitset<MAXN> inter = (Nplus[x] & Nplus[y]);
                if (inter.any() && Nplus[x] != Nplus[y]) {
                    adjoint = false;
                    break;
                }
            }
            if (!adjoint) break;
        }
    }

    if (!adjoint) {
        cout << "G is NOT adjoint graph.\n";
        return 0;
    }

    cout << "G is adjoint graph.\n";

    // --------------------------------------
    // 3) Восстановление H
    // --------------------------------------
    int Hn = 2 * n;
    DSU dsu(Hn);

    auto tailId = [](int i) { return 2 * i; };
    auto headId = [](int i) { return 2 * i + 1; };

    for (auto e : edgesG) {
        int x = e.first;
        int y = e.second;
        dsu.unite(headId(x), tailId(y));
    }

    vector<pair<int, int>> edgesH;
    edgesH.reserve(n);

    for (int i = 0; i < n; i++) {
        int u = dsu.find(tailId(i));
        int v = dsu.find(headId(i));
        edgesH.push_back({ u, v });
    }

    // --------------------------------------
    // 4) Проверка линейности: нет кратных дуг в H
    // --------------------------------------
    bool lineGraph = true;
    map<int, int> compress;
    int idx = 0;

    for (auto& e : edgesH) {
        if (!compress.count(e.first)) compress[e.first] = idx++;
        if (!compress.count(e.second)) compress[e.second] = idx++;
    }

    int K = idx;
    vector<vector<bool>> seen(K, vector<bool>(K, false));

    for (auto& e : edgesH) {
        int u = compress[e.first];
        int v = compress[e.second];
        if (seen[u][v]) {
            lineGraph = false;
            break;
        }
        seen[u][v] = true;
    }

    if (lineGraph)
        cout << "G is ALSO line graph.\n";
    else
        cout << "G is NOT line graph.\n";

    // --------------------------------------
    // 5) Запись H в файл
    // --------------------------------------
    vector<pair<int, int>> edgesHref;

    for (auto& e : edgesH) {
        int u = compress[e.first] + 1;
        int v = compress[e.second] + 1;
        edgesHref.push_back({ u, v });
    }

    ofstream out(outputFile);
    out << K << " " << edgesHref.size() << "\n";
    for (auto& e : edgesHref)
        out << e.first << " " << e.second << "\n";
    out.close();

    cout << "Graph H written to file.\n";

    return 0;
}


