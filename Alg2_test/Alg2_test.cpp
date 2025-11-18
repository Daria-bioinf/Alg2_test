#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

// Проверка графа на сопряжённость
bool isGraf(const vector<vector<int>>& G) {
    int n = G.size();
    for (int x = 0; x < n; x++) {
        for (int y = x + 1; y < n; y++) {
            bool intersect = false;
            for (int a : G[x]) {
                for (int b : G[y]) {
                    if (a == b) { intersect = true; break; }
                }
                if (intersect) break;
            }
            if (intersect) {
                if (G[x].size() != G[y].size()) return false;
                for (int a : G[x]) {
                    bool found = false;
                    for (int b : G[y]) {
                        if (a == b) { found = true; break; }
                    }
                    if (!found) return false;
                }
            }
        }
    }
    return true;
}

// Проверка графа на линейность
bool isLinear(const vector<vector<int>>& G) {
    int n = G.size();
    vector<vector<int>> rev(n);
    for (int u = 0; u < n; u++) {
        for (int v : G[u]) rev[v].push_back(u);
    }
    vector<vector<int>> sortedG = G;
    for (int i = 0; i < n; i++) {
        sort(sortedG[i].begin(), sortedG[i].end());
        sort(rev[i].begin(), rev[i].end());
    }
    for (int x = 0; x < n; x++) {
        for (int y = x + 1; y < n; y++) {
            if (sortedG[x] == sortedG[y] && rev[x] == rev[y]) return false;
        }
    }
    return true;
}

// union-find (DSU)
int findRoot(vector<int>& parent, int x) {
    if (parent[x] == x) return x;
    parent[x] = findRoot(parent, parent[x]);
    return parent[x];
}
void unite(vector<int>& parent, int a, int b) {
    a = findRoot(parent, a);
    b = findRoot(parent, b);
    if (a != b) parent[b] = a;
}

// Построение H из сопряжённого G по описанному в задании способу.
// Возвращает список дуг H (после переиндексации вершин в компактный диапазон 0..k-1).
vector<pair<int, int>> buildH_from_G(const vector<vector<int>>& G) {
    int n = G.size();
    // H0: для каждой вершины i в G создаём дугу (2*i, 2*i+1)
    int total = 2 * n;
    // parent для union-find по вершинам H0 (0..total-1)
    vector<int> parent(total);
    for (int i = 0; i < total; ++i) parent[i] = i;

    // Для каждой дуги u->v в G: склеиваем конец дуги u (2*u+1) и начало дуги v (2*v)
    for (int u = 0; u < n; ++u) {
        for (int v : G[u]) {
            int a = 2 * u + 1;
            int b = 2 * v;
            // safety check (на случай некорректных номеров)
            if (a < 0 || a >= total || b < 0 || b >= total) continue;
            unite(parent, a, b);
        }
    }

    // Найдём репрезентанты (и переиндексируем их компактно)
    map<int, int> remap; // rep -> new index
    int nextIdx = 0;
    for (int i = 0; i < total; ++i) {
        int r = findRoot(parent, i);
        if (remap.find(r) == remap.end()) {
            remap[r] = nextIdx++;
        }
    }

    // Собираем дуги H: для каждой исходной дуги (2*i, 2*i+1) добавляем (rep(2*i), rep(2*i+1))
    // затем переиндексы по remap
    set<pair<int, int>> edgesSet; // используем set, чтобы не дублировать одинаковые дуги при выводе
    vector<pair<int, int>> edgesList; // если нужно оставить дубликаты — можно заполнять этот вектор

    for (int i = 0; i < n; ++i) {
        int a_rep = findRoot(parent, 2 * i);
        int b_rep = findRoot(parent, 2 * i + 1);
        int a = remap[a_rep];
        int b = remap[b_rep];
        // Если a == b — петля; по желанию можно её включать или не включать.
        // Я включаю только ненулевые дуги (a != b), как в примере задания.
        if (a != b) edgesSet.insert(make_pair(a, b));
    }

    // Перенесём из set в вектор (чтобы вернуть)
    for (auto it = edgesSet.begin(); it != edgesSet.end(); ++it) {
        edgesList.push_back(*it);
    }

    return edgesList;
}

int main() {
    ifstream fin("inp_sprze.txt"); // поменяй имя файла при необходимости
    if (!fin.is_open()) {
        cout << "Error: Cannot open file input4.txt\n";
        return 1;
    }

    string line;
    int n, m;

    // читаем заголовок
    if (!getline(fin, line)) {
        cout << "Error: empty input\n";
        return 1;
    }
    stringstream hdr(line);
    hdr >> n >> m;
    if (!hdr) {
        cout << "Error: invalid header\n";
        return 1;
    }

    // создаём G размером n (вершины 0..n-1)
    vector<vector<int>> G(n);
    int readEdges = 0;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int u, v;
        if (!(ss >> u >> v)) continue;
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cout << "Error: Arc " << u << "->" << v << " uses vertex outside range 0.." << n - 1 << "\n";
            return 1;
        }
        G[u].push_back(v);
        ++readEdges;
    }
    fin.close();

    // отладочный вывод
    cout << "Vertices = " << n << " Arcs(header) = " << m << " Arcs(read) = " << readEdges << "\n";
    cout << "List of arcs:\n";
    for (int i = 0; i < n; ++i) {
        cout << i << " -> ";
        for (size_t j = 0; j < G[i].size(); ++j) cout << G[i][j] << (j + 1 < G[i].size() ? " " : "");
        cout << "\n";
    }

    // проверка сопряжённости
    bool conj = isGraf(G);
    cout << (conj ? "Graf jest sprzezony.\n" : "Graf NIE jest sprzezony.\n");
    if (!conj) return 0;

    // проверка линейности
    bool lin = isLinear(G);
    cout << (lin ? "Graf jest liniowy.\n" : "Graf NIE jest liniowy.\n");

    // строим H
    vector<pair<int, int>> H = buildH_from_G(G);

    // вывод H в отладку
    cout << "Computed H (edges):\n";
    for (size_t i = 0; i < H.size(); ++i) {
        cout << H[i].first << " " << H[i].second << "\n";
    }

    // запись в output.txt — формат: num_vertices num_edges
    // нужно знать количество вершин в H: это количество уникальных репрезентантов, которое
    // равно максим индексу в ребрах + 1 (если мы использовали compact remap это nextIdx)
    int numVerticesH = 0;
    int numEdgesH = (int)H.size();
    if (numEdgesH > 0) {
        int maxv = 0;
        for (size_t i = 0; i < H.size(); ++i) {
            maxv = max(maxv, H[i].first);
            maxv = max(maxv, H[i].second);
        }
        numVerticesH = maxv + 1;
    }
    else {
        // если нет дуг — всё равно можно записать 0 вершин или 0 дуг;
        // по смыслу лучше записать количество компонент (можно 0)
        numVerticesH = 0;
    }

    ofstream fout("output_beta.txt");
    if (!fout.is_open()) {
        cout << "Error: cannot open output.txt for writing\n";
        return 1;
    }

    fout << numVerticesH << " " << numEdgesH << "\n";
    for (size_t i = 0; i < H.size(); ++i) {
        fout << H[i].first << " " << H[i].second << "\n";
    }
    fout.close();

    cout << "Graf H zapisano do output.txt\n";
    return 0;
}


