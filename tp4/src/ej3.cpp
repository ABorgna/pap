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
	double x, y;
    int original_idx;
    
	Punto() {}
	
	Punto (double x, double y) : x(x), y(y) {}

	double producto_vectorial(const Punto& b) const{
		return x * b.y - y * b.x;
	}

    double operator^ (const Punto& b) const {
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

double producto(Punto p1, Punto p2, Punto p3){
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
        return atan2(p1.y, p1.x) > atan2(p2.y, p2.x);
    }
};

bool esta_contenido(Punto a, Punto b, Punto c, Punto x){
    // Para no tener problemas de precisión almacenamos el doble para cada uno
    // No divido por 2
    
    double abc_area = abs((b-a).producto_vectorial(b-c)); 
    double axb_area = abs((x-a).producto_vectorial(x-b));
    double axc_area = abs((x-a).producto_vectorial(x-c));
    double bxc_area = abs((x-b).producto_vectorial(x-c));
    
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
double historicos_contenidos(Punto p1, Punto p2, Punto p3, const vector<Punto>& historicos, const vector<Punto>& enemigos ) {
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
    
    double res = 0;
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
        if (p == p_base) {
            continue;
        }
        
        if (p.y > p_base.y or (p.y == p_base.y and p.x >= p_base.x) ){
            res.push_back(p);
        }
    }
    
    return res;
}

double mejor_con_ultimo( vector<Punto>& historicos, const vector<Punto>& enemigos, Matriz2& memo, Punto p_base, int ultimo_a, int ultimo_b){
    if (memo[ultimo_a][ultimo_b] != -kInf){
        return memo[ultimo_a][ultimo_b];
    }
    
    double res = -kInf; 
    double base_contenidos = historicos_contenidos(p_base, historicos[ultimo_a], historicos[ultimo_b], historicos, enemigos);
    
    if (base_contenidos != -1) {
        res = base_contenidos + 3; // base + ultimo_a + ultimo_b forman un triangulo
		
        for(int i = 0; i < ultimo_a; i++) {
        
            double contenidos = historicos_contenidos(p_base, historicos[ultimo_a], historicos[i], historicos, enemigos); 
			if (contenidos != -1 and producto(historicos[ultimo_b], historicos[ultimo_a], historicos[i]) > 0 ) {
                res = max(res, mejor_con_ultimo(historicos, enemigos, memo, p_base, i, ultimo_a) + 1 + base_contenidos);
            }
        }
    }
    
    memo[ultimo_a][ultimo_b] = res;

    return res;
}

double mejor_con_base(const vector<Punto>& historicos, const vector<Punto>& enemigos, int base) {
    Punto p_base = historicos[base];
   
    vector<Punto> historicos_arriba_der = filtro_arriba_der(historicos, base);
    Comparator comp = Comparator(p_base);
    sort(historicos_arriba_der.begin(), historicos_arriba_der.end(), comp); 
    
    int h = historicos_arriba_der.size();
    if (h <= 1){
        return h + 1;
    }

    double res = -kInf;
    Matriz2 memo = nueva_matriz2_menos_inf(h);    
    
    for (int i = 0; i < h; i++){
        for (int j = i+1; j < h; j++){
            res = max(res, mejor_con_ultimo(historicos_arriba_der, enemigos, memo, p_base, i, j));
        }
    }
    
    return res;
}

double solucion(const vector<Punto>& historicos, const vector<Punto>& enemigos) {
    m_historicos_contenidos = nueva_matriz3_menos_inf( (int) historicos.size());

    double res = -kInf; // no hay ptos alineados

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

    double res = solucion(historicos, enemigos);
    if (res > (int) historicos.size()) {
        cout << "Perdon Leo/Melanie por tanto, la proxima te damos algo que ande\n";
        cout << res << endl;
    } else {
        cout << res << endl; 
    }
}
