#ifndef REELLE_H_INCLUDED
#define REELLE_H_INCLUDED

#include "Numerique.h"

class Reelle: public Numerique{
    double valeur;
public:
    explicit Reelle(double v): valeur(v){
        if (v>=0)
            negative = false;
        else
            negative = true;
    }

    /*Reelle(const Reelle& r){
        valeur = r.getValue();
        negative = r.getNeg();
    }*/
    ~Reelle(){}
    double getValue()const{return valeur;}
    string toString()const;
    bool isNULL()const{return valeur == 0;}
    bool isType(const char * t) const;
    bool estVrai()const;
};

#endif // REELLE_H_INCLUDED
