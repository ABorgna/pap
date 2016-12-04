#include <iostream>
#include <map>
#include <vector>
#include <algorithm>    // std::sort


struct Punto {
    int x, y;
    int original_idx;
    
    Punto() {}
    
    Punto (int x, int y) : x(x), y(y) {}

    int producto_vectorial(const Punto& b) const{
        return x * b.y - y * b.x;
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
        if(p1.y == 0 && p1.x > 0) 
        return true; //angle of p1 is 0, thus p2 > p1

    if(p2.y == 0 && p2.x > 0) 
        return false; //angle of p2 is 0 , thus p1 > p2

    if(p1.y > 0 && p2.y < 0) 
        return true; //p1 is between 0 and 180, p2 between 180 and 360

    if(p1.y <0 && p2.y > 0) 
         return false;

    return (p1 ^ p2) > 0; //return true if p1 is clockwise from p2
    }
};

using namespace std;

int main(){
    vector<Punto> ps;
    /*Punto p0 = Punto(0,0);
    int y = 5;
    ps.push_back(Punto(3,y));

    ps.push_back(Punto(5,y));

    ps.push_back(Punto(1,y));

    ps.push_back(Punto(-1,y));

    ps.push_back(Punto(10,y));

    ps.push_back(Punto(25,y));

    ps.push_back(Punto(12,y));

    ps.push_back(Punto(2,y));

    ps.push_back(Punto(13,y));
    Comparator comp = Comparator(p0);*/
    
    Punto p0 = Punto(0,0);
    
    ps.push_back(Punto(10,0));

    ps.push_back(Punto(0,0));

    ps.push_back(Punto(10, 10));

    ps.push_back(Punto(3, 4));

    ps.push_back(Punto(0, 10));

    Comparator comp = Comparator(p0);
    
    sort(ps.begin(), ps.end(), comp);
    
    for(auto p: ps) {
        cout << p << endl;
    }

}
