#include <iostream>
#include <map>
#include <vector>
#include <algorithm>    // std::sort

using namespace std;

typedef vector<vector<int> > Matriz2;
typedef vector<vector<vector<int> > > Matriz3;

#define DEBUG(x) cout << x << endl;

const int kInf = 999999999;

Matriz3 m_historicos_contenidos;

struct Punto {
	int x, y;
    int original_idx;
    
	Punto() {}
	
	Punto (int x, int y) : x(x), y(y) {}

	int producto_vectorial(const Punto& b) const{
		return x * b.y - b.x * y;
	}

    int operator^ (const Punto& b) const {
        return producto_vectorial(b);
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
    
    Punto operator - (const Punto& p2) const {
        return Punto(x - p2.x, y - p2.y);
    }
};



int producto(Punto p1, Punto p2, Punto p3){
    Punto u = p2 - p1;
    Punto v = p3 - p2; 
    return u.producto_vectorial(v);    
}

struct Comparator {
    Punto p0;
    
    Comparator(Punto p0) {
        this->p0 = p0;
    }
    
    bool operator() (Punto p1, Punto p2) { 
        p1=p1-p0; p2=p2-p0; 
        if(p1.y == 0 and p1.x > 0) return true; //angle of p1 is 0, thus p2>p1
        if(p2.y == 0 and p2.x > 0) return false; //angle of p2 is 0 , thus p1>p2
        if(p1.y > 0 and p2.y < 0) return true; //p1 is between 0 and 180, p2 between 180 and 360
        if(p1.y < 0 and p2.y > 0) return false;
        return (p1.producto_vectorial(p2))>0; //return true if p1 is clockwise from p2
    }
};
bool esta_contenido(Punto a, Punto b, Punto c, Punto x){
    // Para no tener problemas de precisión almacenamos el doble para cada uno
    // No divido por 2
    
    int abc_area = abs((b-a).producto_vectorial(b-c)); 
    int axb_area = abs((x-a).producto_vectorial(x-b));
    int axc_area = abs((x-a).producto_vectorial(x-c));
    int bxc_area = abs((x-b).producto_vectorial(x-c));
    
    return abc_area == axb_area + axc_area + bxc_area;
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

/*
 * Si en el triangulo hay enemigos => -1
 * Si no => cantidad de historicos estrictamente contenidos 
 * NO en el borde (no hay), NO los tres vertices del triangulo
 * */
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
    
        for(int i = 0; i < ultimo_a; i++) {
        
            if (i == base or i == ultimo_a) {
                continue;
            }
            
            int contenidos = historicos_contenidos(historicos[base], historicos[ultimo_a], historicos[i], historicos, enemigos); 
            
            if (contenidos != -1 and producto(historicos[ultimo_b], historicos[ultimo_a], historicos[i]) > 0 ) { // BOLAS
                res = max(res, mejor_con_ultimo(historicos, enemigos, memo, base, i, ultimo_a) + 1 + base_contenidos);
            }
        }
    }
    
    memo[ultimo_a][ultimo_b] = res;
    
    return res;
}

int mejor_con_base(const vector<Punto>& historicos, const vector<Punto>& enemigos, int base) {
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

int solucion(const vector<Punto>& historicos, const vector<Punto>& enemigos) {
    m_historicos_contenidos = nueva_matriz3_menos_inf( (int) historicos.size());

    int res = -kInf; // no hay ptos alineados

    for (int i = 0; i < (int) historicos.size(); i++){
        res = max(res, mejor_con_base(historicos, enemigos, i));
    }
    
    return res;
}

int main(){
    int h, e;
    cin >> h >> e;
    
    vector<Punto> historicos(h);
    vector<Punto> enemigos(e);
    
    for(int i=0; i < h; i++){
        cin >> historicos[i];
        historicos[i].original_idx = i;
    }
    for(int i=0; i < e; i++){
        cin >> enemigos[i];
    }
    
    int res = solucion(historicos, enemigos);
    
    cout << res << endl; 
}
