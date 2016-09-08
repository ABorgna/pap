#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

const int INF = (1 << 30);

//devuelve la diversion de poner a todas las amigas de la máscara que estén en 1 en una fiesta
int calcularDiversion(int mask, int* cacheDiversion, int** diversion){
	
	if(cacheDiversion[mask] != -INF) return cacheDiversion[mask]; // ya calculado	

	int fun = 0;

	for(int x = 0;x < ceil(log2(mask));x++){ // por cada bit de la máscara
		for(int y = x+1;y < ceil(log2(mask));y++){
			if(mask & (1 << x) and mask & (1 << y)){ // si los dos estaban en uno
				fun += diversion[x][y];
			}
		}
	}	


	cacheDiversion[mask] = fun;

	return fun;
}

//en la mascara, cada bit representa si falta o no poner a la i-esima amiga en alguna fiesta 
int ponerEnFiestas(int mask, int* cacheMaximaDiversion, int* cacheDiversion, int** diversion){
	if(cacheMaximaDiversion[mask] != -INF) return cacheMaximaDiversion[mask]; //ya calculado, cómo poner a un subconjunto de amigas en fiestas de forma óptima es independiente de cómo agrupé las demás anteriormente
	if(mask == 0) return 0; // ya puse a todas en fiestas
	
	int best = calcularDiversion(mask, cacheDiversion, diversion);

	for (int i = mask; i != 0;i = mask & (i - 1)){
		best = max(best, ponerEnFiestas(mask ^ i, cacheMaximaDiversion, cacheDiversion, diversion) + calcularDiversion(i, cacheDiversion, diversion));
	}

	cacheMaximaDiversion[mask] = best;

	return best;

}

int main () {
	int amigas;
	cin >> amigas;
	
	int **diversion;
	diversion = new int *[amigas];

	for(int i = 0; i < amigas; i++){
		diversion[i] = new int [amigas];
	}

	for(int i = 0;i < amigas * amigas;i++){
		cin >> diversion[i / amigas][i % amigas];
	}

	int cacheMaximaDiversion[1 << amigas]; // la maxima diversion que da un conjunto de amigas en fiestas
	int cacheDiversion[1 << amigas]; // la diversion que da un conjunto de amigas en una fiesta

	for(int i = 0;i < (1 << amigas);i++){
		cacheMaximaDiversion[i] = -INF;
		cacheDiversion[i] = -INF;
	}

	ponerEnFiestas((1 << amigas) - 1, cacheMaximaDiversion, cacheDiversion, diversion);

	for(int i = 0; i < amigas; i++){
		free(diversion[i]);
	}

	free(diversion);	

	cout << cacheMaximaDiversion[(1 << amigas) - 1] << endl;

	return 0;
}

