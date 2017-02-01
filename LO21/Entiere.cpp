#include "Entiere.h"

bool Entiere::isType(const char * t) const{
    string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}

string Entiere::toString()const{
    stringstream f;
    f<<valeur;
    return f.str();
}

bool Entiere::estVrai()const{
    if(valeur==0)
        return false;
    return true;
    }
