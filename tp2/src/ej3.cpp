#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>

using namespace std;

typedef long vertex;
typedef long edge;
typedef vector<vector<pair<vertex, edge>>> graph;

long n, m;
graph adj;

vector<bool> puente;   // e es puente
vector<long> size2EC;  // tamaño de la componente 2-edge conexa de v

void dfsBC(vertex v, long d, vertex padre, stack<vertex> &vertices2EC,
           vector<long> &depth, vector<long> &low) {
    depth[v] = d;
    low[v] = d;

    vertices2EC.push(v);

    for (pair<vertex, edge> p : adj[v]) {
        vertex u = p.first;
        edge e = p.second;

        if (u == padre)
            continue;

        if (depth[u] == -1) {
            // Tree edge
            long position2EC = vertices2EC.size();
            dfsBC(u, d + 1, v, vertices2EC, depth, low);
            low[v] = min(low[v], low[u]);

            if (low[u] >= depth[u]) {
                // (v,u) es puente
                puente[e] = true;

                // Actualizamos la cantidad de nodos 2-edge conectados a cada u
                vertex top;
                long size = vertices2EC.size() - position2EC;
                do {
                    top = vertices2EC.top();
                    size2EC[top] = size;
                    vertices2EC.pop();
                } while (top != u);
            }
        } else {
            // Back edge
            low[v] = min(low[v], depth[u]);
        }
    }

    if (padre == -1) {
        long size = vertices2EC.size();
        while (not vertices2EC.empty()) {
            size2EC[vertices2EC.top()] = size;
            vertices2EC.pop();
        }
    }
}

void findBiconnected() {
    // Computar componentes y puentes
    puente = vector<bool>(m, false);
    size2EC = vector<long>(n, 0);

    stack<long> vertices2EC;
    vector<long> depth(n, -1);
    vector<long> low(n);

    dfsBC(0, 0, -1, vertices2EC, depth, low);
}

pair<bool, long> dfsPuentes(vertex u, vertex dest, vector<bool> &vis) {
    vis[u] = true;
    if (u == dest)
        return {true, 0};

    long res = 0;
    bool found = false;

    for (auto p : adj[u]) {
        vertex v = p.first;
        edge e = p.second;

        if (vis[v])
            continue;

        auto pp = dfsPuentes(v, dest, vis);
        bool thisWay = pp.first;
        long puentes = pp.second;

        if (thisWay) {
            found = true;
            res = puentes + (puente[e] ? 1 : 0);
            break;
        }
    }
    return {found, res};
}

long contarPuentes(vertex u, vertex v) {
    // Contar la cantidad de puentes en algun camino simple de u a v
    // (todo camino simple pasa por la misma cantidad de puentes)
    vector<bool> visitado(n, false);
    return dfsPuentes(u, v, visitado).second;
}

int main() {
    long queries;

    cin >> n >> m;

    adj = graph(n);

    for (long i = 0; i < m; i++) {
        vertex u, v;
        cin >> u >> v;
        u--;
        v--;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    // Precomputar componentes 2-edge conexas y puentes
    findBiconnected();

    cin >> queries;
    for (long q = 0; q < queries; q++) {
        char type;
        cin >> type;

        if (type == 'A') {
            // # puentes en un camino simple de v a u
            vertex u, v;
            cin >> u >> v;
            u--;
            v--;

            cout << contarPuentes(u, v) << endl;

        } else if (type == 'B') {
            // e es puente
            edge e;
            cin >> e;
            e--;

            cout << (puente[e] ? 1 : 0) << endl;

        } else {
            // tamaño de la componente 2-edge conexa de u - 1
            vertex u;
            cin >> u;
            u--;

            cout << size2EC[u] - 1 << endl;
        }
    }

    return 0;
}
