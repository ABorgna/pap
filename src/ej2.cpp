#include <iostream>
#include <algorithm>

using namespace std;

const int INF = - (1 << 30);

//devuelve la diversion de poner a todas las amigas de la máscara que estén en 1 en una fiesta
int calcularDiversion(int mask, int** diversion){
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

	return fun;
}

//en la mascara, cada bit representa si falta o no poner a la i-esima amiga en alguna fiesta 
int ponerEnFiestas(int mask, int* cache, int** diversion){
	if(cache[mask] != INF) return cache[mask]; //ya calculado, cómo poner a un subconjunto de amigas en fiestas de forma óptima es independiente de cómo agrupé las demás anteriormente
	if(mask == 0) return 0; // ya puse a todas en fiestas
	
	int best = INF;

	for (int i = mask; i != 0;i = mask & (i - 1)){
		best = max(best, ponerEnFiestas(mask ^ i, cache, diversion) + calcularDiversion(i, diversion));
	}

	cache[mask] = best;

	return best;

}

int main () {
	int amigos;
	cin >> amigos;
	
	int **diversion;
	diversion = new int *[amigos];

	for(int i = 0; i < amigos; i++){
		diversion[i] = new int [amigos];
	}

	for(int i = 0;i < amigos * amigos;i++){
		cin >> diversion[i / amigos][i % amigos];
	}

	int cache[1 << amigos];

	for(int i = 0;i < (1 << amigos);i++){
		cache[i] = INF;
	}

	ponerEnFiestas((1 << amigos) - 1, cache, diversion);

	cout << cache[(1 << amigos) - 1] << endl;

	return 0;
}

