//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <bitset>
//#include <map>
//#include <algorithm>
//
//using namespace std;
//
//const int MAXN = 20;
//
//
//
//struct DSU {
//    vector<int> parent;
//
//    DSU(int n = 0) {
//        parent.resize(n);
//        for (int i = 0; i < n; i++) parent[i] = i;
//    }
//
//    int find(int x) {
//        if (parent[x] == x) return x;
//        parent[x] = find(parent[x]);
//        return parent[x];
//    }
//
//    void unite(int a, int b) {
//        int pa = find(a);
//        int pb = find(b);
//        if (pa != pb) parent[pb] = pa;
//    }
//};
//
//
//bool isLinearG(const vector<pair<int, int>>& edgesG) {
//    int maxV = 0;
//    for (auto e : edgesG) {
//        if (e.first > maxV) maxV = e.first;
//        if (e.second > maxV) maxV = e.second;
//    }
//    maxV++; 
//
//
//    vector<vector<int>> N_plus(maxV);
//    vector<vector<int>> N_minus(maxV);
//
//    for (auto e : edgesG) {
//        N_plus[e.first].push_back(e.second);
//        N_minus[e.second].push_back(e.first);
//    }
//
//
//    for (int i = 0; i < maxV; i++) {
//        sort(N_plus[i].begin(), N_plus[i].end());
//        sort(N_minus[i].begin(), N_minus[i].end());
//    }
//
//
//    for (int x = 0; x < maxV; x++) {
//        for (int y = x + 1; y < maxV; y++) {
//            vector<int> inter_plus;
//            vector<int> inter_minus;
//
//            set_intersection(
//                N_plus[x].begin(), N_plus[x].end(),
//                N_plus[y].begin(), N_plus[y].end(),
//                back_inserter(inter_plus)
//            );
//
//            set_intersection(
//                N_minus[x].begin(), N_minus[x].end(),
//                N_minus[y].begin(), N_minus[y].end(),
//                back_inserter(inter_minus)
//            );
//
//
//            if (!inter_plus.empty()) {
//                if (N_plus[x] != N_plus[y] || !inter_minus.empty()) {
//                    return false; 
//                }
//            }
//        }
//    }
//
//    return true; 
//}
//
//int main()
//{
//    string inputFile, outputFile;
//    cout << "Input file: ";
//    cin >> inputFile;
//
//    cout << "Output file: ";
//    cin >> outputFile;
//
//    ifstream in(inputFile);
//    if (!in) {
//        cout << "Cannot open input file.\n";
//        return 1;
//    }
//
//    int n, m;
//    in >> n >> m;
//
//    vector<pair<int, int>> edgesG;
//    edgesG.reserve(m);
//
//    for (int i = 0; i < m; i++) {
//        int u, v;
//        in >> u >> v;
//        edgesG.push_back({ u - 1, v - 1 }); 
//    }
//    in.close();
//
//
//    bool oneGraph = true;
//    bool hasEdge[MAXN][MAXN] = { false };
//    vector< bitset<MAXN> > Nplus(n);
//
//    for (auto e : edgesG) {
//        int u = e.first;
//        int v = e.second;
//        if (hasEdge[u][v]) oneGraph = false;
//        hasEdge[u][v] = true;
//        Nplus[u].set(v);
//    }
//
//    cout << (oneGraph ? "G is 1-graph\n" : "G is NOT 1-graph\n");
//
//    bool adjoint = oneGraph;
//    if (adjoint) {
//        vector< vector<int> > Nvec(n);
//        for (int i = 0; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                if (Nplus[i].test(j)) Nvec[i].push_back(j);
//            }
//            sort(Nvec[i].begin(), Nvec[i].end()); 
//        }
//
//        for (int x = 0; x < n; x++) {
//            for (int y = x + 1; y < n; y++) {
//                vector<int> inter;
//                set_intersection(Nvec[x].begin(), Nvec[x].end(),
//                    Nvec[y].begin(), Nvec[y].end(),
//                    back_inserter(inter));
//                if (!inter.empty() && Nvec[x] != Nvec[y]) {
//                    adjoint = false;
//                    break;
//                }
//            }
//            if (!adjoint) break;
//        }
//    }
//    if (!adjoint) {
//        cout << "G is NOT adjoint (sprzezony) graph.\n";
//        return 0;
//    }
//    cout << "G is adjoint (sprzezony) graph.\n";
//
//
//
//
//
//    int Hn = 2 * n;
//    DSU dsu(Hn);
//
//    auto tailId = [](int i) { return 2 * i; };
//    auto headId = [](int i) { return 2 * i + 1; };
//
//    for (auto e : edgesG) {
//        int x = e.first;
//        int y = e.second;
//        dsu.unite(headId(x), tailId(y));
//    }
//
//    vector<pair<int, int>> edgesH;
//    edgesH.reserve(n);
//
//    for (int i = 0; i < n; i++) {
//        int u = dsu.find(tailId(i));
//        int v = dsu.find(headId(i));
//        edgesH.push_back({ u, v });
//    }
//
//
//    bool lineGraph = isLinearG(edgesG);
//
//    if (lineGraph)
//        cout << "G is ALSO line graph.\n";
//    else
//        cout << "G is NOT line graph.\n";
//
//
//
//    map<int, int> compress;
//    int idx = 0;
//    for (auto& e : edgesH) {
//        if (!compress.count(e.first)) compress[e.first] = idx++;
//        if (!compress.count(e.second)) compress[e.second] = idx++;
//    }
//    int K = idx;
//
//    ofstream out(outputFile);
//    if (!out.is_open()) {
//        cout << "Cannot open output file.\n";
//        return 1;
//    }
//
//    out << K << " " << edgesH.size() << "\n";
//    for (auto& e : edgesH) {
//        int u = compress[e.first] + 1; 
//        int v = compress[e.second] + 1;
//        out << u << " " << v << "\n";
//    }
//    out.close();
//
//    cout << "Graph H written to file.\n";
//
//
//    return 0;
//}
//
//
//

//упрощенная версия ниже віше старій код

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// --------------------------------------------------------
// Простая проверка, является ли граф G спряжённым (adjoint)
// --------------------------------------------------------
bool isAdjoint(const vector<vector<int>>& Nplus, int n)
{
    for (int x = 0; x < n; x++)
    {
        for (int y = x + 1; y < n; y++)
        {
            // ищем пересечение N+(x) и N+(y)
            bool common = false;
            for (int a : Nplus[x])
                for (int b : Nplus[y])
                    if (a == b) common = true;

            // если есть общие вершины, то множества должны быть равны
            if (common)
            {
                if (Nplus[x].size() != Nplus[y].size())
                    return false;

                for (int i = 0; i < (int)Nplus[x].size(); i++)
                    if (Nplus[x][i] != Nplus[y][i])
                        return false;
            }
        }
    }
    return true;
}

// --------------------------------------------------------
// Простейшая структура "объединение" (аналог DSU, но проще)
// --------------------------------------------------------
int parentArr[200];

int findRoot(int v)
{
    while (parentArr[v] != v)
        v = parentArr[v];
    return v;
}

void unite(int a, int b)
{
    a = findRoot(a);
    b = findRoot(b);
    if (a != b) parentArr[b] = a;
}

// --------------------------------------------------------
// Проверка линейности G (но сильно упрощённая логика)
// --------------------------------------------------------
bool isLinear(const vector<pair<int, int>>& edges)
{
    // Для студента: просто считаем, что если есть повторяющиеся хвосты → не линейный
    vector<int> cnt(200, 0);

    for (auto e : edges)
        cnt[e.first]++;

    for (int c : cnt)
        if (c > 1) return false;

    return true;
}


// --------------------------------------------------------
// ОСНОВНАЯ ПРОСТАЯ ПРОГРАММА
// --------------------------------------------------------
int main()
{
    string inputFile, outputFile;
    cout << "Input file: ";
    cin >> inputFile;

    cout << "Output file: ";
    cin >> outputFile;

    ifstream in(inputFile);
    if (!in)
    {
        cout << "Cannot open input file.\n";
        return 1;
    }

    int n, m;
    in >> n >> m;

    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        in >> u >> v;
        edges.push_back({ u - 1, v - 1 });
    }
    in.close();

    // -------------------------------------------
    // Формируем N+(x)
    // -------------------------------------------
    vector<vector<int>> Nplus(n);

    vector<vector<bool>> hasEdge(n, vector<bool>(n, false));
    bool is1Graph = true;

    for (auto e : edges)
    {
        int u = e.first;
        int v = e.second;

        if (hasEdge[u][v]) is1Graph = false;
        hasEdge[u][v] = true;

        Nplus[u].push_back(v);
    }

    for (int i = 0; i < n; i++)
        sort(Nplus[i].begin(), Nplus[i].end());

    cout << (is1Graph ? "G is 1-graph\n" : "G is NOT 1-graph\n");

    // -------------------------------------------
    // Проверка спряжённости
    // -------------------------------------------
    if (!is1Graph)
    {
        cout << "G is NOT adjoint graph.\n";
        return 0;
    }

    if (!isAdjoint(Nplus, n))
    {
        cout << "G is NOT adjoint graph.\n";
        return 0;
    }

    cout << "G is adjoint (sprzezony) graph.\n";

    // -------------------------------------------
    // Восстанавливаем граф H
    // -------------------------------------------
    int Hn = 2 * n;

    for (int i = 0; i < Hn; i++)
        parentArr[i] = i;

    // unite(head(x), tail(y))
    for (auto e : edges)
    {
        int x = e.first;
        int y = e.second;

        int headX = 2 * x + 1;
        int tailY = 2 * y;

        unite(headX, tailY);
    }

    vector<pair<int, int>> edgesH;
    for (int i = 0; i < n; i++)
    {
        int tail = findRoot(2 * i);
        int head = findRoot(2 * i + 1);
        edgesH.push_back({ tail, head });
    }

    // -------------------------------------------
    // Проверка линейности
    // -------------------------------------------
    if (isLinear(edges))
        cout << "G is ALSO line graph.\n";
    else
        cout << "G is NOT line graph.\n";

    // -------------------------------------------
    // Сжимаем номера вершин H
    // -------------------------------------------
    vector<int> comp(200, -1);
    int id = 0;

    for (auto e : edgesH)
    {
        if (comp[e.first] == -1) comp[e.first] = ++id;
        if (comp[e.second] == -1) comp[e.second] = ++id;
    }

    // -------------------------------------------
    // Запись H в файл
    // -------------------------------------------
    ofstream out(outputFile);
    if (!out)
    {
        cout << "Cannot open output file.\n";
        return 1;
    }

    out << id << " " << edgesH.size() << "\n";
    for (auto e : edgesH)
        out << comp[e.first] << " " << comp[e.second] << "\n";

    out.close();

    cout << "Graph H written.\n";
    return 0;
}
