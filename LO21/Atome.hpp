#ifndef Atome_hpp
#define Atome_hpp

#include "projet.h"

class Atome: public Litterale{
    string nom;
public:
    explicit Atome(const string& name):nom(name){}
    string getNom()const{return nom;}
    bool checkExist();
    bool checkSyntax();
    void traitement()const;
    string toString()const;
    bool isType(const char * t) const;
    bool estVrai()const{return true;}
};

class AtomeManager{
    Atome** atms;
    unsigned int nb;
    unsigned int cap;
public:
    AtomeManager():atms(new Atome*[10]),nb(0),cap(10){}
    ~AtomeManager(){delete[] atms;}
    AtomeManager(const AtomeManager& atm);
    AtomeManager& operator=(const AtomeManager& atm);
    void reserve(unsigned int n);
    Atome& AddAtome(Atome& n);
    void removeAtome(Atome& a);

};


#endif /* Atome_hpp */
