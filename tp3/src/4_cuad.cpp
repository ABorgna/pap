#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int preguntar(int * arreglo, int inicio, int fin){
	auto it = max_element(arreglo+inicio,arreglo+fin);
	int maxi = *it;
	int index = it - arreglo;
	
	arreglo[index] = -1; // Ya no lo cuento
	
	it = max_element(arreglo+inicio,arreglo+fin);
	arreglo[index] = maxi; // Lo arreglo
	maxi += *it;
	
	return maxi;
}

int main(){
	int D,R;
	cin>>D>>R;
	
	int arreglo[D];
	
	for(int i = 0 ; i < D; i ++){
		cin>>arreglo[i];
	}
		
	for(int i = 0 ; i < R; i ++){
		int inicio,fin;
		cin>>inicio>>fin;
		cout<<preguntar(arreglo,inicio,fin)<<endl;	
	}
	return 0;
}
