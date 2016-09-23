#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
const int maxn  = 100000;
vector<int> G[maxn], GT[maxn];
int C[maxn];
vector <int> st;
void dfs(int no){
	C[no]=1;
	for(int v : G[no]){
		if(!C[v])
			dfs(v);
	}
	st.push_back(no);
	return;
}

void dfs2(int no, int color){
	if(!C[no]){
		C[no]=color;
		for(int v : GT[no]){
			if(!C[v])
				dfs2(v,color);
		}
	}
}

int main(){
	int N,M,Q,a,b,colores = 0;
	cin>>N>>M;
	for(int i = 0 ; i< M; i++){
		cin>>a>>b; --a; --b;
		G[a].push_back(b);
		GT[b].push_back(a);		
	}
	for(int i = 0 ; i< N; i++){
		if(!C[i])
			dfs(i);
	}
	memset(C,0,sizeof(C));
	while(st.size()){ 
		int vec = st.back();
		st.pop_back(); 
		if( !C[vec]) 
			dfs2(vec,++colores); 
	} 

	cin>>Q;
	for(int i = 0 ; i < Q; i ++){
		cin>>a>>b; --a; --b;
		cout<<(C[a]==C[b]?"S\n":"N\n");
	}
	return 0;
}
