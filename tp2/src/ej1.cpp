#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

typedef vector<std::vector<int> > adj_list;
const int inf = 9999999;

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

void conectar(adj_list& G , std::vector<std::vector<int> >&capacidad_red_residual, int a, int b){
	capacidad_red_residual[a][b] = 1;
	G[a].push_back(b);
	G[b].push_back(a);
}

int main(){
	//Sean a y N+a el nodo y su continuación
	//0 el inicial y 2*N+1 el final.
	int N,M;
	while(cin>>N>>M){
		char tipo;
		adj_list G; //Todo nodo "v" va a ser duplicado por "v_entrantes" y "v_salientes" con una conexión entre ellos direccional. 
		G.resize(2*N+2);
		
		std::vector<std::vector<int> > capacidad_red_residual(G.size());
		for(int i = 0; i < (int) G.size();i++){
			capacidad_red_residual[i].resize(G.size(),0);
		}

		int endpoint = 2*N+1;
		for(int i = 1 ; i < N+1; i ++){
			cin>>tipo;
			if(tipo=='E'){
				conectar(G,capacidad_red_residual,i,endpoint);//Las escuelas las conecto con el sumidero
			}
			else{
				if(tipo=='A'){
					conectar(G,capacidad_red_residual,0,i);//La fuente la conecto con los alumnos.
				}
			}
		}
		for(int i = 1 ; i < N+1; i ++){
			conectar(G,capacidad_red_residual,i,N+i); //Conecto a todo nodo con su duplicado
		}
		for(int i = 0 ; i < M ; i ++){
			int a,b;
			cin>>a>>b;	 //Calle bidireccional pero al duplicar los nodos, al saliente lo conecto con el entrante del nodo respectivo.
			conectar(G, capacidad_red_residual, N+a, b);
			conectar(G, capacidad_red_residual, b+N, a);
		}
		cout<<max_flujo(G,capacidad_red_residual,0,endpoint)<<endl;
	}
	return 0;
}
