#ifndef NUMERIQUE_H_INCLUDED
#define NUMERIQUE_H_INCLUDED

#include "projet.h"

class Numerique : public Litterale{
protected:
    bool negative;
    //Juste un boolean pour le signe global mais pas on change aussi la value
public:
    Numerique(){}
    virtual ~Numerique(){}
    //virtual Numerique* clone() const=0; pour savoir le type
    virtual std::string toString() const=0;
    double getPartieReelleNum() const{ return getValue();}
    virtual double getValue() const=0;
    bool getNeg() const {return negative;}
    bool isType(const char * t) const;//TODO a virer
    virtual bool estVrai()const=0;

};


#endif // NUMERIQUE_H_INCLUDED
