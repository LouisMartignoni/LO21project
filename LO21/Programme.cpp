//
//  Programme.cpp
//  ProjetLO21
//
//  Created by Nicolas on 2016-06-02.
//  Copyright © 2016 Nicolas. All rights reserved.
//

#include "Programme.hpp"
#include "ComputerException.h"

//REGEX Programe
const regex smallLetters(".*[[:lower:]].*");
const regex legalCar(".*([[:alnum:]]|[,.$ ()+*\\[\\]]/-)+.*");

bool Programme::checkSyntax()const{

    if(regex_match (contenu, smallLetters)){
        throw ComputerException("Opérandes interdites dans programme : pas de minuscules");
        return false;
    }
    else if(regex_match (contenu, legalCar)){
        return true;

    }
    else {
        throw ComputerException("Opérandes interdites dans programme");
        return false;
    }

}


/*bool Programme::isType(const char * t) const{
    string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}*/

std::string Programme::toString()const{//TO DO remplacer toutes les tabs et newline en espace
    stringstream f;
    std::string str(this->contenu);
    str.erase( std::remove(str.begin(), str.end(), '\t' ), str.end() ) ;
    str.erase( std::remove(str.begin(), str.end(), '\n' ), str.end() ) ;
    f<< "[ " << str << " ]";
    return f.str();


}
