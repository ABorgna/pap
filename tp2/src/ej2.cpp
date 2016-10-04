#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <queue>

using namespace std;

typedef unordered_map<int,int> aristas;
const int inf = 9999999;

int nodo_sale(int x, int a){
  return x;
}

int nodo_entra(int x, int a){
  return x + a;
}


int bfs(vector< aristas > & G,int endpoint){ //Como en la red residual las aristas valen 1 o 0 el camino de aumento siempre puede ser 1 (si es que lo hay) o 0 en caso contrario.
	queue<int> cola;
	bool visited [G.size()];
	fill(visited,visited+endpoint+1,false);
	cola.push(0);
	int P[G.size()];
	fill(P,P + endpoint+1,-1);
	int aumento = 1;
	//BFS
	while(!cola.empty()){
		int actual = cola.front();
		cola.pop();
		visited[actual] = true;
		for(auto &a : G[actual])
			if(not visited[a.first] and a.second == 1){
				P[a.first] = actual;
				cola.push(a.first);
			}
	} 
	//Si no hubo camino de aumento (no llegue al nodo final)
	if(not visited[endpoint])
		return 0;
	//En caso de que sí, reconstruyo el camino y a todas esas aristas las marco como usadas y no pueden volver a usarse.
	int actual = endpoint;
	int padre = P[actual];
	while(padre!=-1){
		G[padre][actual] = 0;
		G[actual][padre] = 1;
		actual = padre;
		padre = P[actual];
	}
	return 1;
}

int flujo(vector< aristas > & G , int endpoint){
	int flujo_max = 0;
	int aumento;
	while((aumento = bfs(G,endpoint))>0){ //Mientras haya camino de aumento
		flujo_max+=aumento;
	}
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
  
  vector< aristas > G; //Todo nodo "v" va a ser duplicado por "v_entrantes" y "v_salientes" con una conexión entre ellos direccional. 
  G.resize(2*a+2);
  int endpoint = 2*a+1;

  
  for (int i = 1; i < a + 1; i++){ // acciones en [1 .. a+1)
    precios[i].resize(d);
    
    for(int j = 0; j < d; j++){
      std::cin >> precios[i][j];
    }
  }
  
  for(int i = 1; i < a + 1; i++){
    G[nodo_entra(i, a)][endpoint] = 1;  // con el sink
    G[0][nodo_sale(i, a)] = 1; // con el source
  }
  
  for(int i = 1; i < a + 1; i++){
    for (int j = 1; j < a + 1; j++){
      if (estaArribaDe(i, j, precios)){
        G[nodo_sale(i, a)][nodo_entra(j, a)] = 1;
      } 
    }
  }
  
  int f = flujo(G, endpoint);
  std::cout << a - f << std::endl;
}
