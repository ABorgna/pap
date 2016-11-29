#include <iostream>
#include <map>
#include <vector>
#include <algorithm>    // std::sort

using namespace std;

typedef vector<vector<int> > Matriz2;
typedef vector<vector<vector<int> > > Matriz3;

#define DEBUG(x) cout << x << endl;

const int kInf = 999999999;
int H, E;

Matriz3 m_historicos_contenidos;

struct Punto {
	int x, y;
    int original_idx;
    
	Punto() {}
	
	Punto (int x, int y) : x(x), y(y) {}

	int producto_vectorial(Punto& b) {
		return x * b.y - b.x * y;
	}

	bool operator != (const Punto& b) const{
		return x != b.x or y != b.y or original_idx != b.original_idx;
	}
    
    bool operator == (const Punto& b) const{
        return !(*this != b);
    }
	
	friend std::ostream& operator << (std::ostream &o, Punto &p) {
		o << p.x << " " << p.y;
		return o;
	}

	friend std::istream& operator >> (std::istream &i, Punto &p) {
		i >> p.x >> p.y;
		return i;
	}
};



int sign (Punto p1, Punto p2, Punto p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

struct Comparator {
    Punto p0;
    
    Comparator(Punto p0) {
        this->p0 = p0;
    }
    
    bool operator() (const Punto p1, const Punto p2) const { 
        if (sign(p0, p1, p2) > 0) {
            return 1;
        } else if (sign(p0, p1, p2) < 0){
            return -1; 
        } else { // no hay colineales, no deberia pasar
            return 0;
        }
    }
};


bool esta_contenido(Punto p1, Punto p2, Punto p3, Punto x){
    // me robe esto, temporalmente
    
    bool b1, b2, b3;

    b1 = sign(x, p1, p2) < 0;
    b2 = sign(x, p2, p3) < 0;
    b3 = sign(x, p3, p1) < 0;

    return ((b1 == b2) && (b2 == b3));
}

Matriz2 nueva_matriz2_menos_inf(int n){
    Matriz2 m;
    m.resize(n);
    for(int i = 0; i < n; i++){
        m[i].assign(n, -kInf);
    }
    
    return m;
}

Matriz3 nueva_matriz3_menos_inf(int n){
    Matriz3 m;
    m.resize(n);
    for(int i = 0; i < n; i++){
        m[i].resize(n); 
        for (int j = 0; j < n; j++) {
            m[i][j].assign(n, -kInf); 
        }
    }
    
    return m;
}

int historicos_contenidos(Punto p1, Punto p2, Punto p3, const vector<Punto>& historicos, const vector<Punto>& enemigos ) {
    if (m_historicos_contenidos[p1.original_idx][p2.original_idx][p3.original_idx] != -kInf){
        return m_historicos_contenidos[p1.original_idx][p2.original_idx][p3.original_idx];
    }
    
    bool contiene_enemigos = false;
    for (const auto& x : enemigos) {
         contiene_enemigos |= esta_contenido(p1, p2, p3, x);
    }
    
    if(contiene_enemigos){        
        m_historicos_contenidos[p1.original_idx][p2.original_idx][p3.original_idx] = -1;
        return -1;
    }
    
    int res = 0;
    for (const auto& x : historicos) {
         if (x == p1 or x == p2 or x == p3){
             continue;
         }
         
         if (esta_contenido(p1, p2, p3, x)){
             res++;
         }
    }
    
    
    m_historicos_contenidos[p1.original_idx][p2.original_idx][p3.original_idx] = res;
    return res;
}

vector<Punto> filtro_arriba_der (const vector<Punto>& historicos, int base){
    vector<Punto> res;
    Punto p_base = historicos[base];
    
    for(const auto& p : historicos){
        if (p.y > p_base.y or (p.y == p_base.y and p.x >= p_base.x) ){
            res.push_back(p);
        }
    }
    
    return res;
}



int mejor_con_ultimo(const vector<Punto>& historicos, const vector<Punto>& enemigos, Matriz2& memo, int base, int ultimo_a, int ultimo_b){
    if (memo[ultimo_a][ultimo_b] != -kInf){
        return memo[ultimo_a][ultimo_b];
    }
    
    int res = -kInf; 
    int base_contenidos = historicos_contenidos(historicos[base], historicos[ultimo_a], historicos[ultimo_b], historicos, enemigos);
    
    if (base_contenidos != -1) {
        res = base_contenidos + 3; // base + ultimo_a + ultimo_b forman un triangulo
    }
    
    
    for(int i = 0; i < ultimo_a; i++) {
    
        if (i == base or i == ultimo_a) {
            continue;
        }
        
        int contenidos = historicos_contenidos(historicos[base], historicos[ultimo_a], historicos[i], historicos, enemigos); 
        
        if (contenidos != -1 and sign(historicos[ultimo_b], historicos[ultimo_a], historicos[i]) > 0 ) { // BOLAS
            res = max(res, mejor_con_ultimo(historicos, enemigos, memo, base, i, ultimo_a) + 1 + base_contenidos);
        }
    }
    
    memo[ultimo_a][ultimo_b] = res;
    
    return res;
}

int mejor_capsula_incluyendo(const vector<Punto>& historicos, vector<Punto>& enemigos, int base) {
    Punto p_base = historicos[base];
    vector<Punto> historicos_arriba_der = filtro_arriba_der(historicos, base);
    Comparator comp = Comparator(p_base);
    sort(historicos_arriba_der.begin(), historicos_arriba_der.end(), comp); 
    
    int base_filtrada = 0;
    while(p_base != historicos_arriba_der[base_filtrada]){
        base_filtrada++;
    }
    
    int h = historicos_arriba_der.size();
    if (h <= 2){
        return h;
    }

    int res = -kInf;
    Matriz2 memo = nueva_matriz2_menos_inf(h);    
    
    for (int i = 0; i < h; i++){
        for (int j = 0; j < h; j++){
            if (i != j and i != base_filtrada and j != base_filtrada){
                res = max(res, mejor_con_ultimo(historicos, enemigos, memo, base_filtrada, i, j));
            }
        }
    }
    
    return res;
}


int main(){
    cin >> H >> E;
    
    vector<Punto> historicos(H);
    vector<Punto> enemigos(E);
    
    for(int i=0; i < H; i++){
        cin >> historicos[i];
        historicos[i].original_idx = i;
    }
    for(int i=0; i < E; i++){
        cin >> enemigos[i];
    }
    
    int res;
    
    res = min(2, H); // no hay ptos alineados
    
    if (H > 2) {
        m_historicos_contenidos = nueva_matriz3_menos_inf(H);
        for (int i = 0; i < H; i++){
            res = max(res, mejor_capsula_incluyendo(historicos, enemigos, i));
        }
    }
    
    cout << res << endl; 
}
