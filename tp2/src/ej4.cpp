#include <iostream>
#include <vector>
#include <stack>
#include <cstring>
using namespace std;

stack <int > st;


void dfs_componentes(int nodo, vector<int> vecinos[], bool visitado[], int componentes[]){ // dfs para asignarle a cada nodo su componente
	visitado[nodo] = true;
	for(int &vecino : vecinos[nodo]){
		if(visitado[vecino] == false){
			componentes[vecino] = componentes[nodo]; // la componente de mi vecino es la misma que la mia
			dfs_componentes(vecino, vecinos, visitado, componentes);
		}
	}
}

void dfs(int nodo, vector<int> vecinos[], bool visitado[]){ // dfs, voy agregando al stack a medida que termino de procesar un nodo
	visitado[nodo] = true;
	for(int &vecino : vecinos[nodo]){
		if(visitado[vecino] == false){
			dfs(vecino, vecinos, visitado);
		}
	}
	st.push(nodo);
}


int main(){
	int nodos, aristas, queries;

	cin >> nodos >> aristas;

	vector<int> grafo[nodos + 1];
	vector<int> grafoInverso[nodos + 1]; // grafo con aristas invertidas

	for(int i = 0; i < aristas;i++){
		int a, b;
		cin >> a >> b;
		grafo[a].push_back(b);
		grafoInverso[b].push_back(a);
	}

	bool visitado[nodos + 1];
	int componentes[nodos + 1];

	memset(visitado,0,sizeof(visitado));

	for(int i = 1; i <= nodos; i++){
		if(visitado[i] == false){
			dfs(i, grafo, visitado);
		}
	}

	memset(visitado,0,sizeof(visitado));

	while(!st.empty()){ // calculo componentes en el orden en que me quedaron los nodos en el stack
		int nodo = st.top();
		st.pop();
		if(visitado[nodo] == false){
			componentes[nodo] = nodo; // inicializo mi componente como yo mismo
			dfs_componentes(nodo, grafoInverso, visitado, componentes);
		}
	}

	cin >> queries;

	for(int i = 0; i < queries; i++){
		int a, b;
		cin >> a >> b;
		cout << (componentes[a] == componentes[b] ? "S\n" : "N\n");
	}


}