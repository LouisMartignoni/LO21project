#include "Litterale.hpp"

bool Litterale::isType(const char * t) const{
    std::string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}
