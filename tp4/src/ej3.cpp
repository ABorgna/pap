#include <iostream>
#include <map>
#include <vector>

using namespace std;

typedef vector<vector<int> > Matriz2;
typedef vector<vector<vector<int> > > Matriz3;


const int kInf = 999999999;
int H, E;

Matriz3 m_contiene_enemigo;

struct Punto {
	int x, y;
    int original_idx;
    
	Punto() {}
	
	Punto (int x, int y) : x(x), y(y) {}

	int producto_vectorial(Punto& b) {
		return x * b.y - b.x * y;
	}

	bool operator != (Punto& b) {
		return x != b.x or y != b.y;
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

bool contiene_enemigo(Punto p1, Punto p2, Punto p3, const vector<Punto>& enemigos) {
    if (m_contiene_enemigo[p1.original_idx][p2.original_idx][p3.original_idx] != -kInf){
        return m_contiene_enemigo[p1.original_idx][p2.original_idx][p3.original_idx];
    }
    
    bool res = false;
    for (const auto& x : enemigos) {
         res |= esta_contenido(p1, p2, p3, x);
    }
    
    m_contiene_enemigo[p1.original_idx][p2.original_idx][p3.original_idx] = res;
    return res;
}

vector<Punto> filtro_arriba_der (const vector<Punto>& historicos, int base){
    vector<Punto> res;
    
    for(const auto& p : historicos){
        if (true){
            res.push_back(p);
        }
    }
    
    return res;
}



int mejor_con_ultimo(const vector<Punto>& historicos, vector<Punto>& enemigos, Matriz2& memo, int base, int ultimo_a, int ultimo_b){
    if (memo[ultimo_a][ultimo_b] != -kInf){
        return memo[ultimo_a][ultimo_b];
    }
    
    int res = 3; // base + ultimo_a + ultimo_b forman un triangulo
    
    for(int i = 0; i < (int) historicos.size();i++) {
        if (true) {
            res = max(res, mejor_con_ultimo(historicos, enemigos, memo, base, i, ultimo_a) + 1);
        }
    }
    
    memo[ultimo_a][ultimo_b] = res;
    
    return res;
}

int mejor_capsula_incluyendo(const vector<Punto>& historicos, vector<Punto>& enemigos, int base) {
    Punto p_base = historicos[base];
    vector<Punto> historicos_arriba_der = filtro_arriba_der(historicos, base);
    
    int base_filtrada = 0;
    while(p_base != historicos_arriba_der[base_filtrada]){
        base_filtrada++;
    }
    
    int h = historicos_arriba_der.size();
    Matriz2 memo = nueva_matriz2_menos_inf(h);    
    int res = -kInf;
    
    for (int i = 0; i < h; i++){
        for (int j = 0; j < h; j++){
            int res = max(res, mejor_con_ultimo(historicos, enemigos, memo, base_filtrada, i, j));
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
    
    if (H <= 2){
        res = H; // no hay ptos alineados
    } else {
        m_contiene_enemigo = nueva_matriz3_menos_inf(H);
        for (int i = 0; i < H; i++){
            res = max(res, mejor_capsula_incluyendo(historicos, enemigos, i));
        }
    } 
}
