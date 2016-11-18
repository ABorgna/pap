#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#define forn(i,n) for(int i = 0; i< (int) n; i++)
#define forns(i,s,n) for(int i = s; i<= (int) n; i++)

using namespace std;

struct punto {
	int x, y;
	punto() {
	}
	
	punto (int x, int y) : x(x), y(y) {
	}

	int producto_vectorial(punto& b) {
		return x * b.y - b.x * y;
	}

	bool operator != (punto& b) {
		return x != b.x or y != b.y;
	}
	
	friend std::ostream& operator << (std::ostream &o, punto &p) {
		o << p.x << " " << p.y;
		return o;
	}

	friend std::istream& operator >> (std::istream &i, punto &p) {
		i >> p.x >> p.y;
		return i;
	}
};

struct segmento {
	punto a, b;
	segmento() {
		
	}
	segmento (punto a, punto b) : a(a), b(b) {
	}
};

bool operator == (const punto& a, const punto& b) {
	return a.x == b.x and a.y == b.y;
}

bool operator < (const punto &p1,const punto &p2) {
	return p1.x < p2.x or (p1.x == p2.x and p1.y < p2.y);
}

bool operator < (const segmento& s1, const segmento& s2) {
	return s1.a < s2.a or (s1.a == s2.a and s1.b < s2.b);
}


void recorrer_y_mostrar(punto& actual, punto& padre, map<punto, vector<punto> >& segmentos_incidentes, punto& leftmost) {
	cout << " " << actual;
	if (segmentos_incidentes[actual][0] == padre) {
		if (segmentos_incidentes[actual][1] != leftmost) {
			recorrer_y_mostrar(segmentos_incidentes[actual][1], actual, segmentos_incidentes, leftmost);
		}
	}
	else{
		if (segmentos_incidentes[actual][0] != leftmost){
			recorrer_y_mostrar(segmentos_incidentes[actual][0], actual, segmentos_incidentes, leftmost);
		}
	}
}

int main() {
	int n;
	cin>>n;
	vector <segmento> segmentos;
	map <segmento, int> veces_usado;
	map<punto, vector<punto> > segmentos_incidentes;

	for (int i = 0; i < n-2; i++) {
		punto uno, dos, tres;
		cin >> uno >> dos >> tres;
		segmento s (uno,dos), s2(dos,tres);
		
		veces_usado[{min(uno, dos), max(uno, dos)}]++;
		veces_usado[{min(tres, dos), max(tres, dos)}]++;
		veces_usado[{min(tres, uno), max(tres, uno)}]++;
	}

	for (const pair<segmento,int>& segment : veces_usado) {
		if(segment.second==1) {
			segmentos.push_back(segment.first);
		}
	}

	punto leftmost = segmentos[0].a;
	for(const segmento& segment : segmentos)
	{
		leftmost = min(leftmost, segment.a);
		segmentos_incidentes[segment.a].push_back(segment.b);
		segmentos_incidentes[segment.b].push_back(segment.a);
	}

	cout << leftmost;
	
	punto der (segmentos_incidentes[leftmost][1].x - leftmost.x, segmentos_incidentes[leftmost][1].y - leftmost.y);
	punto izq (segmentos_incidentes[leftmost][0].x -leftmost.x ,segmentos_incidentes[leftmost][0].y - leftmost.y);
	
	if (izq.producto_vectorial(der) > 0) {
		recorrer_y_mostrar(segmentos_incidentes[leftmost][1], leftmost, segmentos_incidentes, leftmost);
	}
	else {
		recorrer_y_mostrar(segmentos_incidentes[leftmost][0], leftmost, segmentos_incidentes, leftmost);
	}

	return 0;
}
