#ifndef COMPLEXE_H_INCLUDED
#define COMPLEXE_H_INCLUDED

#include "projet.h"
#include "Numerique.h"

class Complexe : public Litterale{
    Numerique& reel;
    Numerique& im;
public:
    Complexe(Numerique& r, Numerique& i) : reel(r),im(i){}
    ~Complexe(){}
    std::string toString()const;
    double getPartieReelleNum() const{ return reel.getPartieReelleNum();}
    int getPartieReelleDen() const{return reel.getPartieReelleDen();}
    double getPartieImNum() const{return im.getPartieReelleNum();}
    int getPartieImDen() const{return im.getPartieReelleDen();}
    bool isType(const char * t) const;
    bool estVrai()const;
};

#endif // COMPLEXE_H_INCLUDED
