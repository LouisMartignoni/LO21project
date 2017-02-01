#include "Complexe.h"

bool Complexe::isType(const char * t) const{
    string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}

string Complexe::toString()const{
    stringstream f;
    f<<reel.toString()<<"$"<<im.toString();
    return f.str();
}

bool Complexe::estVrai()const{
    if ((getPartieReelleNum()==0)&&(getPartieImNum()==0))
        return false;
    return true;
    }

