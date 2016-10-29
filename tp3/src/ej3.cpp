#include <iostream>

using namespace std;

int main(){

	int cargos, antiguedad, preguntas;

	cin >> cargos >> antiguedad >> preguntas;

	int matriz[cargos + 1][antiguedad + 1];

	for (int i = 0; i < antiguedad + 1; i++) {
		matriz[0][i] = 0;
	}

	for (int i = 0; i < cargos + 1; i++) {
		matriz[i][0] = 0;
	}

	for (int i = 1; i <= cargos; i++) {
		for (int j = 1; j <= antiguedad; j++) {
			cin >> matriz[i][j];
			matriz[i][j] = matriz[i][j] + matriz [i - 1][j] + matriz[i][j - 1] - matriz[i - 1][j - 1];
		}
	}

	for (int i = 0; i < preguntas; i++) {
		int c1, a1, c2, a2;
		cin >> c1 >> a1 >> c2 >> a2;
		cout << matriz[c2][a2] - matriz[c2][a1] - matriz[c1][a2] + matriz[c1][a1] << "\n";
	}


	return 0;
}