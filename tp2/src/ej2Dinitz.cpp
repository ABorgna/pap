#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <cstring>
#include <queue>
#define forn(i,n) for (int i = 0 ; i < n; i++)
#define forsn(i,j,n) for (int i = j ; i < n; i++)

#define dforn(i,n) for (int i = n-1 ; i >= 0; i--)

using namespace std;
const int inf = 9999999;

typedef std::vector<std::vector<int> > adj_list;

int nin(int x, int a){
    return x;
}

int nout(int x, int a){
    return x + a;
}

bool estaArribaDe(int a, int b, std::vector<std::vector<int> >& precios){
    for(int i = 0; i < (int) precios[a].size(); i++){
        if (precios[a][i] <= precios[b][i]){
            return false;
        }
    }
    return true;
}

#define INF 1000000000 // Infinito de FLUJO
#define MAX_M 1000000  // Maximo de aristas
#define MAX_N 45000    // Maximo de nodos
int v[2*MAX_M], l[2*MAX_M];     // Vecino, link. link te tira el indice de la arista "al reves" asociada en la lista del vecino.
long c[2*MAX_M];        // Capacidad
int sz[MAX_N], po[MAX_N], r[MAX_N], n, S, T;
typedef map<int,long> Mii;
Mii CAP[MAX_N];
void iniG() {
        n = 0;
        memset(sz,0,sizeof(sz));
        forn(i,MAX_N) CAP[i].clear();
}
void aEje(int d,int h,long cap) {
        if (d == h) return; // Ignoramos completamente autoejes, obvio :D
        n = max(n,max(d,h));
        pair<Mii::iterator,bool> par = CAP[d].insert(make_pair(h,0));
        if (par.second) {
                CAP[h][d] = 0;
                sz[d]++;
                sz[h]++;
        }
        par.first->second += max(cap,(long)0);
}
void _aEje(int d,int h,long capDH, long capHD) {
        #define ASIG(d,h,cap) {v[po[d]] = h; c[po[d]] = cap; l[po[d]] = po[h];}
        ASIG(d,h,capDH);
        ASIG(h,d,capHD);
        po[d]++; po[h]++;
}
void _iniG() {
        po[0] = 0;
        forn(i,n-1) po[i+1] = po[i] + sz[i];
        forn(u,n) for(auto v : CAP[u])
                if (u < v.first) _aEje(u,v.first,v.second,CAP[v.first][u]);
}
long aumentar() {
        // bfs
        forn(i,n) r[i] = -1;
        r[S] = 0;
        queue<int> q;
        q.push(S);
        while (!q.empty()) {
                int x = q.front(); q.pop();
                int d = r[x] + 1, b = po[x];
                if (r[T] >= 0 && d > r[T]) break;
                forsn(j,b,b+sz[x])
                if (c[j]>0 && r[v[j]] < 0) {
                        r[v[j]] = d;
                        q.push(v[j]);
                }
        }
        // dfs que hace la magia :P
        long res = 0;
        static int path[MAX_N]; path[0] = S;
        static int p[MAX_N],ind[MAX_N];
        memset(p,-1,sizeof(p));
        int pp = 0; // Path pointer, es la longitud
        while (pp >= 0) {
                int x = path[pp];
                if (x == T)     {
                        long f = INF;
                        int pri = 0;
                        dforn(i,pp)     if (c[ind[i]]<=f) f = c[ind[i]], pri = i;
                        forn(i,pp) c[ind[i]] -= f, c[l[ind[i]]] += f;
                        res += f;
                        pp = pri;
                }
                else if (++p[x] < sz[x]) {
                        int j = po[x]+p[x];
                        if (p[v[j]] < 0 && c[j] > 0 && r[v[j]] == 1 + r[x])
                                ind[pp] = j, path[++pp] = v[j];
                }
                else pp--;
        }
        return res;
}
long flujo(int ss,int tt) {
        S = ss; T = tt;
        n = max(n,max(S,T)) + 1; // Aca, n ya tiene el valor posta
        _iniG();
        forn(i,n) po[i] -= sz[i];
       
        long res = 0,c;
        do {res += (c = aumentar());} while (c>0);
        return res;
}


int main() {
    
    int a, d;
    
    std::cin >> a >> d;
    
    std::vector<std::vector<int> > precios(a+1);
    
    //adj_list adj (2*a+ 2); //Todo nodo "v" va a ser duplicado por "v_entrantes" y "v_salientes" con una conexión entre ellos direccional. 
    iniG();
    int source = 0;
    int sink = 2*a+1;
    
    
    for (int i = 1; i < a + 1; i++){ // acciones en [1 .. a+1)
        precios[i].resize(d);
        
        for(int j = 0; j < d; j++){
            std::cin >> precios[i][j];
        }
    }
    

    for(int i = 1; i < a + 1; i++){
        aEje(nout(i, a),sink,1);
        aEje(source, nin(i,a),1);
    }
    
    for(int i = 1; i < a + 1; i++){
        for (int j = 1; j < a + 1; j++){
            if (estaArribaDe(i, j, precios)){
                aEje(nin(i,a), nout(j,a),1);
            } 
        }
    }
    
    int f = flujo(source, sink);
    
    std::cout << a - f << std::endl;
}


