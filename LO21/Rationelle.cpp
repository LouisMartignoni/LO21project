#include "Rationelle.h"

bool Rationnelle::isType(const char * t) const{
    string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}

std::string Rationnelle::toString()const{
    std::stringstream f;
    f<<num.getValue()<<"/"<<den.getValue();
    return f.str();
}
