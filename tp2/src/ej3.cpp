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

vector<bool> puente;          // e es puente
vector<long> sizeComponente;  // tamaño de la componente i
vector<vector<long>>
    componentes;              // componentes biconexas a las que pertenece v
vector<long> sumComponentes;  // cantidad de nodos que comparten un ciclo con v

void dfsBC(vertex v, long d, vertex padre, stack<edge> &q,
           stack<vertex> &componentVertices, vector<long> &depth,
           vector<long> &low) {
    depth[v] = d;
    low[v] = d;

    for (pair<vertex, edge> p : adj[v]) {
        vertex u = p.first;
        edge e = p.second;

        if (u == padre)
            continue;

        if (depth[u] < depth[v]) {
            // Solo agregamos (u,v) si es un tree edge o un back edge nuevo
            q.push(e);
        }

        if (depth[u] == -1) {
            // Tree edge
            componentVertices.push(u);

            dfsBC(u, d + 1, v, q, componentVertices, depth, low);
            low[v] = min(low[v], low[u]);

            if (low[u] >= depth[v]) {
                // Completamos la componente,
                // actualizamos los valores de las aristas
                long comp = sizeComponente.size();
                sizeComponente.push_back(0);
                long top;

                do {
                    top = q.top();
                    sizeComponente[comp]++;
                    q.pop();
                } while (top != e);

                // Actualizamos la cantidad de nodos biconectados de cada u
                do {
                    top = componentVertices.top();
                    sumComponentes[top] += sizeComponente[comp] - 1;
                    componentes[top].push_back(comp);
                    componentVertices.pop();
                } while (top != u);
                sumComponentes[v] += sizeComponente[comp] - 1;
            }
            if (low[u] >= depth[u]) {
                // (v,u) es puente
                puente[e] = true;
            }
        } else {
            // Back edge
            low[v] = min(low[v], depth[u]);
        }
    }
}

void findBiconnected() {
    // Computar componentes y puentes
    puente = vector<bool>(m, false);
    componentes = vector<vector<long>>(n);
    sizeComponente.resize(0);
    sumComponentes = vector<long>(n, 0);

    stack<long> q;
    stack<long> componentVertices;
    vector<long> depth(n, -1);
    vector<long> low(n);

    dfsBC(0, 0, -1, q, componentVertices, depth, low);
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
    // Contar la cantidad de puentes en el camino de u a v,
    // o 0 si están en una misma componente de tamaño > 1
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

    // Precomputar componentes biconexas y puentes
    findBiconnected();

    cin >> queries;
    for (long q = 0; q < queries; q++) {
        char type;
        cin >> type;

        if (type == 'A') {
            // # puentes en un camino simple de v a u (0 si hay caminos
            // disjuntos)
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
            // cantidad de vertices que comparten ciclo con u
            vertex u;
            cin >> u;
            u--;

            cout << sumComponentes[u] << endl;
        }
    }

    return 0;
}
