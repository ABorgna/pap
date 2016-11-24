#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <queue>

const int inf = 9999999;

typedef std::vector<std::vector<int> > adj_list;

int nodo_sale(int x, int a){
	return x;
}

int nodo_entra(int x, int a){
	return x + a;
}


int bfs(std::vector<std::vector<int> >& capacidad_red_residual, adj_list& adj, int source, int sink){ 
//Como en la red residual las aristas valen 1 o 0 el camino de aumento siempre puede ser 1 (si es que lo hay) o 0 en caso contrario.
	
	std::queue<int> cola;
		
	cola.push(source);

	std::vector<bool> visited(adj.size(), false);

	std::vector<int> p (adj.size(), -1);

	//BFS

	while(!cola.empty()){
		int actual = cola.front();
		cola.pop();
		
		if (visited[actual]) {
			continue;
		}
		visited[actual] = true;
		
		for(auto& v : adj[actual]){
				if(not visited[v] and capacidad_red_residual[actual][v] == 1){
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
		capacidad_red_residual[padre][actual] = 0;
		capacidad_red_residual[actual][padre] = 1;
		
		actual = padre;
		padre = p[actual];
	}
	// Toda arista tiene capacidad 1
	return 1;
}

int max_flujo(adj_list& adj, std::vector<std::vector<int>>& capacidad_red_residual, int source, int sink){
	int flujo_max = 0;
	int aumento;
	
	do {
		aumento = bfs(capacidad_red_residual, adj, source, sink);
		flujo_max += aumento;
	} while (aumento > 0);
	
	return flujo_max;
}


bool estaArribaDe(int a, int b, std::vector<std::vector<int> >& precios){
	for(int i = 0; i < (int) precios[a].size(); i++){
		if (precios[a][i] <= precios[b][i]){
			return false;
		}
	}
	return true;
}

void conectar(adj_list& G , std::vector<std::vector<int> >&capacidad_red_residual, int a, int b){
	capacidad_red_residual[a][b] = 1;
	G[a].push_back(b);
	G[b].push_back(a);
}


int main() {
	
	int a, d;
	
	std::cin >> a >> d;
	
	std::vector<std::vector<int> > precios(a+1);
	
	adj_list adj (2*a+ 2); //Todo nodo "v" va a ser duplicado por "v_entrantes" y "v_salientes" con una conexión entre ellos direccional. 
	
	std::vector<std::vector<int> > capacidad_red_residual(adj.size());
	for(int i = 0; i < (int) adj.size();i++){
		capacidad_red_residual[i].resize(adj.size(),0);
	}
	
	int source = 0;
	int sink = 2*a+1;
	
	
	for (int i = 1; i < a + 1; i++){ // acciones en [1 .. a+1)
		precios[i].resize(d);
		
		for(int j = 0; j < d; j++){
			std::cin >> precios[i][j];
		}
	}
	

	for(int i = 1; i < a + 1; i++){
		conectar(adj,capacidad_red_residual,nodo_entra(i, a),sink);
		conectar(adj,capacidad_red_residual,source, nodo_sale(i,a));
	}
	
	for(int i = 1; i < a + 1; i++){
		for (int j = 1; j < a + 1; j++){
			if (estaArribaDe(i, j, precios)){
				conectar(adj,capacidad_red_residual,nodo_sale(i,a),nodo_entra(j,a));
			} 
		}
	}
	
	int f = max_flujo(adj, capacidad_red_residual, source, sink);
	
	std::cout << a - f << std::endl;
}
