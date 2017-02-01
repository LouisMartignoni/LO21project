#ifndef LITTERALE_H_INCLUDED
#define LITTERALE_H_INCLUDED

#include<iostream>
#include<string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>

class Litterale {

public:
    Litterale(){}
    virtual ~Litterale(){}
    void afficher(std::ostream& f= std::cout) const{f<<toString()<<"\n";}
    virtual std::string toString() const=0;
    virtual std::string getExpression() const {return toString();}
    bool isType(const char * t) const;
    virtual double getPartieReelleNum() const{return 0;}
    virtual int getPartieReelleDen() const{return 1;}
    virtual double getPartieImNum() const{return 0;}
    virtual int getPartieImDen() const{return 1;}
    double getPartieReelle() const{return (getPartieReelleNum()/getPartieReelleDen());}
    double getPartieIm() const{return (getPartieImNum()/getPartieImDen());}
    virtual bool estVrai()const=0;
};

#endif // LITTERALE_H_INCLUDED
