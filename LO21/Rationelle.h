#ifndef RATIONELLE_H_INCLUDED
#define RATIONELLE_H_INCLUDED

#include "Numerique.h"
#include "Entiere.h"

class Rationnelle: public Numerique{
    Entiere num;
    Entiere den;
public:
    Rationnelle(int n, int d): num(n),den(d){
        simplification();
        negative = (!num.getNeg() != !den.getNeg());
    }
    Rationnelle(const Entiere& e1, const Entiere& e2): num(e1),den(e2){
        simplification();
        negative = (!num.getNeg() != !den.getNeg());
    }
    /*
    Rationnelle(const Rationnelle& r): num(r.num), den(r.den){
        negative = (!num.getNeg() != !den.getNeg());
    }*/
    ~Rationnelle(){}
    std::string toString()const;
    double getValue() const{ return num.getValue();}
    int getPartieReelleDen() const{ return den.getValue();}
    void simplification(){
        /*int r  = PGCD(num.getValue(), den.getValue());
        if (r!=1 && r!=0){
            num.setValue(num.getValue()/r);
            den.setValue(den.getValue()/r);
        }*/ //Deja fait a la division
        if (den.getNeg()){
            num.setValue(-num.getValue());
            den.setValue(-den.getValue());
        }
    }
    bool isType(const char * t) const;
    bool estVrai()const{return false;}
};

#endif // RATIONELLE_H_INCLUDED
