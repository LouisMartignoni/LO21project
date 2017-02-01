#include "Reelle.h"

bool Reelle::isType(const char * t) const{
    string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}

std::string Reelle::toString()const{
    std::stringstream f;
    f<<valeur;
    return f.str();
};

bool Reelle::estVrai()const{
    if(getValue()==0)
        return false;
    return true;
}
