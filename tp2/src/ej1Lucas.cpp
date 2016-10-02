#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
using namespace std;

typedef map<int,int> aristas;
const int inf = 9999999;

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

int main(){
	//Sean a y N+a el nodo y su continuación
	//0 el inicial y 2*N+1 el final.
	int N,M;
	while(cin>>N>>M){
		char tipo;
		vector< aristas > G; //Todo nodo "v" va a ser duplicado por "v_entrantes" y "v_salientes" con una conexión entre ellos direccional. 
		G.resize(2*N+2);
		int endpoint = 2*N+1;
		for(int i = 1 ; i < N+1; i ++){
			cin>>tipo;
			if(tipo=='E'){
				G[i][endpoint] = 1; //Las escuelas las conecto con el sumidero
			}
			else{
				if(tipo=='A'){
					G[0][i] = 1; //La fuente la conecto con los alumnos.
				}
			}
		}
		for(int i = 1 ; i < N+1; i ++){
			G[i][N+i] = 1; //Conecto a todo nodo con su duplicado
		}
		for(int i = 0 ; i < M ; i ++){
			int a,b;
			cin>>a>>b;	 //Calle bidireccional pero al duplicar los nodos, al saliente lo conecto con el entrante del nodo respectivo.
			G[N+a][b] = 1;
			G[b+N][a] = 1;
		}
		cout<<flujo(G,endpoint)<<endl;
	}
	return 0;
}
