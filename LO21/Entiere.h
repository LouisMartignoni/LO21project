#ifndef ENTIERE_H_INCLUDED
#define ENTIERE_H_INCLUDED

#include "Numerique.h"

class Entiere: public Numerique{
    int valeur;
public:
    Entiere(int v=0):valeur(v){
        if (v>=0)
            negative = false;
        else
            negative = true;
    }
    /*Entiere(const Entiere& e){
        valeur = e.getValue();
        negative = e.getNeg();
    }*/
    ~Entiere(){}
    void setValue(int v){
        if ( (valeur>=0 && v<0) || (valeur<0 && v>=0) )
            negative = !negative;
        valeur=v;
    }
    bool isType(const char * t) const;
    std::string toString()const;
    double getValue() const{ return valeur;}
    bool estVrai()const;
};

#endif // ENTIERE_H_INCLUDED
