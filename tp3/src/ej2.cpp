#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Trie {
	map<char, Trie> hijos;
	
	int contador;
};

void agregar(Trie& nodo, string& s, int i){
	nodo.contador++;
	
	if (i == (int) s.size()){
		return;
	} else {
		agregar(nodo.hijos[s[i]], s, i+1);
	}
}

int contador(Trie& nodo, string s, int p, int i){
	if (i == p){
		return nodo.contador;
	} else {
		return contador(nodo.hijos[s[i]], s, p, i+1);
	}
}

int main(){
	Trie root;
	
	int n;
	
	cin >> n;
	
	vector<pair<string, int> > mails_prefijos(n);
	
	for(int i = 0; i < n; i++){
		string d;
		int p;
		
		cin >> d >> p;
		
		agregar(root, d, 0);
		
		mails_prefijos[i] = {d, p};
	}
	
	int t = 0;
	
	for(int i = 0; i < n; i++){
		int t_i = contador(root, mails_prefijos[i].first, mails_prefijos[i].second, 0);
		
		t = max(t, t_i); 
	}
	
	cout << t << endl;
	
	return 0;
}
