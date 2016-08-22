#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

using namespace std;
const int maxn = 40;
int N, A, B;
long long P[maxn], Q[maxn], T, resp;//P = elementos del primer subconjunto, Q = elementos del segundo, T = target buscado, resp = Respuesta final

int main() {
    cin >> T >> N;
    A = N / 2;
    B = N / 2 + N % 2;
    forn(i, A) cin >> P[i];
    forn(i, B) cin >> Q[i];
    set<long long> resultados;
    forn(i, 1 << A) { //Armo todos los subconjuntos del primer subgrupo
        long long suma = 0;
        forn(j, A) { // Calculo la suma de los presentes
            if ((1 << j & i) == (1 << j))
                suma += P[j];
        }
        if (suma <= T) // si no me pasé del target lo agrego al conjunto
            resultados.insert(suma);
    }
    forn(i, 1 << B) { //Armo todos los subconjuntos del segundo subgrupo
        long long busco = 0;
        forn(j, B) { // Calculo la suma de los presentes
            if ((1 << j & i) == (1 << j))
                busco += Q[j];
        }

        if (busco > T)// si me pasé del target pruebo con otro
            continue;
        auto it = resultados.upper_bound(T - busco); //busco el primero que sea mayor
        if (it == resultados.begin()) //Si es el primero entonces no tengo ninguno con el cual sumar
            resp = max(resp, busco);
        else {
			it--; //retrocedo al primero que sea <= a lo que busco.
            resp = max(resp, *it + busco); //Uso el que buscaba y lo que tenia acumulado del subconjunto que calculé recién.
        }
    }
    cout << resp << endl; //Devuelvo la respuesta
    return 0;
}
