#include <iostream>
#include <algorithm>

using namespace std;

const int INF = (1 << 30);

//devuelve la diversion de poner a todas las amigas de la máscara que estén en 1 en una fiesta
int calcularDiversion(int mask, int* cacheDiversion, int** diversion){
	
	if(cacheDiversion[mask] != -INF) return cacheDiversion[mask]; // ya calculado	

	int fun = 0, 
		pos1 = 0;

	for(;mask != 0;mask >>= 1){
		if(mask & 1){
			int pos2 = pos1 + 1;
			for(int mask2 = mask >> 1; mask2 != 0; mask2 >>= 1){
				if(mask2 & 1){
					fun += diversion[pos1][pos2];
				}
				pos2++;
			}
		}
		pos1++;
	}	

	return cacheDiversion[mask] = fun;
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

	cout << cacheMaximaDiversion[(1 << amigas) - 1] << endl;

	return 0;
}

