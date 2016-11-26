#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

int mcd (int a, int b) {
	return a == 0 ? b : mcd (b % a, a);
}

long long modPow (long long a, long long b, long long mod) {
	if (b == 0LL) return 1LL;
	long long temp = modPow(a, b / 2, mod) % mod;
	return (temp * temp * (b % 2 == 0LL ? 1LL : a)) % mod;
}

int main () { 
    int N;
    cin >> N;

    int F[N + 1];
    int visitados[N + 1];
    long long cantidades[N + 1];
    long long dependencias = 0; // para que n^2 entre, en int no entra
    vector<long long> longitudes;

    for (int i = 0; i < N; i++) {
    	cin >> F[i + 1];
    	visitados[i + 1] = false;
    	cantidades[i + 1] = 0;
    }

    for (int i = 1; i < N + 1; i++) {
    	int largo = 0;
    	int actual = i;

    	while (visitados[actual] == false) {
    		visitados[actual] = true;
    		largo++;
    		actual = F[actual];
    	}
    	cantidades[largo]++; //las cantidades de largo 0 no me importan asi que por mas que aca se lo sumo a cantidades no lo uso
    }

    for (int i = 1; i < N + 1; i++) {
    	if (cantidades[i] > 0 && i % 2 == 0) {
    		cout << 0 << endl;
    		return 0;
    	}
    	if (cantidades[i] > 0 && i % 2 == 1) longitudes.push_back(i);
    }

    for (int i = 0;i < longitudes.size(); i++) {
    	dependencias += longitudes[i] * cantidades[longitudes[i]] * (cantidades[longitudes[i]] - 1) / 2; // cantidad de dependencias entre ciclos de largo longitudes[i]
    	dependencias += (longitudes[i] - 1) / 2 * cantidades[longitudes[i]]; // cantidad de dependencias dentro de un ciclo de largo longitudes[i] * cantidad de ciclos de ese largo
    	
    	for (int j = i + 1; j < longitudes.size(); j++) {
    		dependencias += cantidades[longitudes[i]] * cantidades[longitudes[j]] * mcd(longitudes[i], longitudes[j]);
    	}
    }

    cout << modPow(2LL, dependencias, 1000000007LL) << endl;

}
