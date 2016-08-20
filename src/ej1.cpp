#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

#define forn(i,n) for(int i =0; i < (int)(n); i++)

using namespace std;
const int maxn = 40;
int N,A,B;
long long P [maxn],Q[maxn],T,resp;
int main(){
	cin>>N>>T;
	A = N/2;
	B = N/2 + N%2;
	forn(i,A)
		cin>>P[i];
	forn(i,B)
		cin>>Q[i];
	set<long long> resultados;
	forn(i,1<<A){
		long long suma=0;
		forn(j,A){
			if((1<<j & i) == (1<<j))
				suma+=P[j];
		}
		if(suma<=T)
			resultados.insert(suma);
	}
	forn(i,1<<B){
		long long busco=0;
		forn(j,B){
			if((1<<j & i) == (1<<j))
				busco+=Q[j];
		}

		if(busco>T) continue;
		auto it = resultados.lower_bound(T - busco);
		if(it == resultados.end())
			it--;
		if(it==resultados.begin())
			resp= max(resp,busco);
		else{
			if(*it > T - busco) it--;
			if(it==resultados.begin())
				resp= max(resp,busco);
			else
				resp = max(resp,*it+busco);
		}
	}
	cout<<resp<<endl;
	return 0;
}
