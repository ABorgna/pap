
#include "matriz.h"
#include <vector>
#include <cassert>

bool haySubsecuencia(int start, int end, vector<Matriz>& matrices, Matriz& m, int l){
  if (end - start < l){
    return false;
  }
  
  if (end - start == 1){
    // l == 1
    return matrices[start] == m;
  }
  
  bool res = false;
  
  int mid = (start + end) / 2;
  
  vector<Matriz> prefijo(l+1);
  prefijo[0] = id();
  
  int i = 1;
  while(i <= l and mid + i - 1 < end){
    prefijo[i] = prefijo[i - 1] * matrices[mid + i - 1];
    i++;
  }

  if (prefijo[l] == m){
    res = true;
  }
  
  Matriz sufijo = id();
  
  i = 1;
  while(i <= l and mid - i >= start){
    sufijo *= matrices[mid - i];
    if (sufijo * prefijo[l-i] == m){
      res = true;
    }
    i++;
  }

  
  res = res or haySubsecuencia(start, mid, matrices, m, l)
            or haySubsecuencia(mid, end, matrices, m, l);
  
  return res;
}

int main () {
  int n, l;
  std::cin >> n >> l;
  
  Matriz m;
  std::cin>>m;
  
  vector<Matriz> matrices(n);
  
  for(int i = 0; i < n; i++){
    std::cin>>matrices[i];
  }

  bool res = haySubsecuencia(0, n, matrices, m, l);
  
  if (res == true){
    std::cout << "SI\n";
  }
  else {
    std::cout << "NO\n";
  }
}

