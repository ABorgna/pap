#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

const pair<int,int> elemento_neutro = {-1,-1};
const int valor_neutro = -1;


pair<int,int> get(pair<int,int> * arreglo, int inicio, int fin, int N , int minimo, int limite_superior){
	if(inicio <= minimo and fin >=limite_superior){ //Si estoy contenido totalmente
		return arreglo[N];
	}
	
	if(fin <= minimo or inicio >= limite_superior){ //Si estoy afuera 
		return elemento_neutro;
	}
	
	//Si tengo mitad adentro, mitad afuera.
	int medio = (limite_superior + minimo)/2;
	pair<int,int> parte_izquierda = get(arreglo,inicio,fin,2*N,minimo, medio);
	pair<int,int> parte_derecha = get(arreglo,inicio,fin,2*N+1,medio, limite_superior);
	
	if(parte_izquierda.first > parte_derecha.first){
		return parte_izquierda;
	}else{
		return parte_derecha;
	}
}

void set(pair<int,int> * arreglo, int index, int value){
	arreglo[index].first = value; //Le pongo el nuevo valor a la hoja
	int padre = index/2;
	bool mantuve = false;
	while(padre >=1 and not mantuve){
		if(arreglo[2*padre].first > arreglo[2*padre+1].first){ //Lo corrijo como corresponde.
			mantuve = (arreglo[padre] == arreglo[2*padre]);
			arreglo[padre] = arreglo[2*padre]; 	
		}else{
			mantuve = (arreglo[padre] == arreglo[2*padre+1]);
		 	arreglo[padre] = arreglo[2*padre+1];
		}
		padre = padre/2;
	}
}

int preguntar(pair<int,int> * arreglo, int limite_superior, int inicio, int fin){
	pair<int,int> mas_grande = get(arreglo,inicio,fin, 1, 0, limite_superior/2); //Busco el máximo
	
	int llevo = mas_grande.first; //Este es el valor del máximo
	
	int posicion_arreglo = limite_superior/2 + mas_grande.second;
	set(arreglo,posicion_arreglo,valor_neutro); //Pongo un elemento neutro en él.
	
	pair<int,int> segundo_mas_grande = get(arreglo,inicio,fin, 1, 0, limite_superior/2); //Busco el segundo más grande. 
	
	assert(mas_grande.first == llevo);
	set(arreglo,posicion_arreglo,mas_grande.first); //Le restauro el valor.
	//cout<<llevo<<" "<<segundo_mas_grande.first<<endl;
	llevo += segundo_mas_grande.first; //Tengo la suma de los dos más grandes.
	
	return llevo;
}


void armar_segment_tree(pair<int,int> * arreglo, int limite_superior){
	for (int i = limite_superior/2-1; i>0;i--){ //Para cada nodo interior, le pregunto cual de ambos de los hijos son más grandes y me lo guardo.
		if(arreglo[2*i].first > arreglo[2*i+1].first){
			arreglo[i] = arreglo[2*i]; 	
		}else{
		 	arreglo[i] = arreglo[2*i+1];
		}
	}
}

void llenar_hoja(pair<int,int> *arreglo, int valor, int posicion, int limite_superior){
	arreglo[limite_superior/2 + posicion] = {valor,posicion};
}

int main(){
	int D,R;
	cin>>D>>R;
	
	int limite_superior = 1; //Busco la potencia de dos que sea más grande a D. Ese será la cantidad de hojas.
	while(limite_superior<D){
		limite_superior*=2;
	}
	limite_superior*=2; //Multiplico por dos para tener la cantidad total de nodos del árbol + 1 (el 0 que no uso)
	
	pair<int,int> arreglo[limite_superior]; //Segment Tree, lo inicializo en neutros.
	for(int i = 0 ; i < D; i ++){
		arreglo[limite_superior] = elemento_neutro;
	}
	
	for(int i = 0 ; i < D; i ++){
		int diversion;
		cin>>diversion;
		llenar_hoja(arreglo,diversion,i,limite_superior); //Lleno los valores de las hojas
	}
		
	armar_segment_tree(arreglo,limite_superior); //Completo el árbol.

	for(int i = 0 ; i < R; i ++){
		int inicio,fin;
		cin>>inicio>>fin;
		cout<<preguntar(arreglo,limite_superior,inicio,fin)<<endl;	
	}
	return 0;
}
