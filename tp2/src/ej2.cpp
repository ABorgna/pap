#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <queue>

using namespace std;

const int inf = 9999999;

typedef vector<vector<int> > adj_list;

int nodo_sale(int x, int a){
  return x;
}

int nodo_entra(int x, int a){
  return x + a;
}


int bfs(vector<vector<int> >& flujo_neto, adj_list& adj, int source, int sink){ 
  //Como en la red residual las aristas valen 1 o 0 el camino de aumento siempre puede ser 1 (si es que lo hay) o 0 en caso contrario.
	
  queue<int> cola;
	
  cola.push(source);
  
  vector<bool> visited(adj.size(), false);

  vector<int> p (adj.size(), -1);
  
  //BFS
  
	while(!cola.empty()){
		int actual = cola.front();
		cola.pop();
		
    visited[actual] = true;
		
    for(auto& v : adj[actual]){
			if(not visited[v] and flujo_neto[actual][v] < 1){
				p[v] = actual;
				cola.push(v);
			}
    }
	} 
  
	//Si no hubo camino de aumento (no llegue al nodo final)
	if(not visited[sink]){
		return 0;
  }
	
  //En caso de que sí, reconstruyo el camino y a todas esas aristas las marco como usadas y no pueden volver a usarse.
	int actual = sink;
	int padre = p[actual];
	
  while(padre!=-1){
		flujo_neto[padre][actual]++;
    flujo_neto[actual][padre]--;
    
    actual = padre;
		padre = p[actual];
	}
  
  // Toda arista tiene capacidad 1
	return 1;
}

int max_flujo(adj_list& adj, int source, int sink){
	int flujo_max = 0;
	int aumento;
  
  vector<vector<int> > flujo_neto(adj.size());
  for(int i = 0; i < (int) adj.size();i++){
    flujo_neto[i].resize(adj.size());
    for(int j = 0; j < (int) adj.size(); j++){
      flujo_neto[i][j] = 0;
    }
  }
  
  do {
    aumento = bfs(flujo_neto, adj, source, sink);
    flujo_max += aumento;
    
  } while (aumento > 0);
  
  
	return flujo_max;
}


bool estaArribaDe(int a, int b, vector<vector<int> >& precios){
  for(int i = 0; i < (int) precios[a].size(); i++){
    if (precios[a][i] <= precios[b][i]){ // Ojo, vale el igual?!
      return false;
    }
  }
  
  return true;
}

int main() {
  
  int a, d;
  
  std::cin >> a >> d;
  
  std::vector<std::vector<int> > precios(a+1);
  
  adj_list adj (2*a+ 2); //Todo nodo "v" va a ser duplicado por "v_entrantes" y "v_salientes" con una conexión entre ellos direccional. 
  
  int source = 0;
  int sink = 2*a+1;

  
  for (int i = 1; i < a + 1; i++){ // acciones en [1 .. a+1)
    precios[i].resize(d);
    
    for(int j = 0; j < d; j++){
      std::cin >> precios[i][j];
    }
  }
  
  for(int i = 1; i < a + 1; i++){
    adj[nodo_entra(i, a)].push_back(sink);  // con el sink
    adj[sink].push_back(nodo_entra(i, a));  // con el sink, la vuelta
    
    
    adj[source].push_back(nodo_sale(i, a)); // con el source
    adj[nodo_sale(i, a)].push_back(source); // con el source
    
  }
  
  for(int i = 1; i < a + 1; i++){
    for (int j = 1; j < a + 1; j++){
      if (estaArribaDe(i, j, precios)){
    
        adj[nodo_sale(i, a) ].push_back(nodo_entra(j, a));
        adj[nodo_entra(j, a)].push_back(nodo_sale(i, a));
    
      } 
    }
  }
  
  int f = max_flujo(adj, source, sink);
  
  std::cout << a - f << std::endl;
}
