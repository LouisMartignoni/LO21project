#ifndef Expression_hpp
#define Expression_hpp

#include "Litterale.hpp"
#include "projet.h"
#include "Strategy.hpp"
#include <regex>
using namespace std;

class Atome;

class Expression : public Litterale,public Strategy {
    string exp;
public:
    explicit Expression(const string& val){
        if(regex_match(val, regex("^'.*'$"))) {
            string s1 = val.substr(1,val.length()-2);
            exp = val;
        }
        else exp = val;
        elimineSpace();
    }
    string toString() const;
    string getExp(){return exp;}
    string getExpression() const {return exp;}
    void setExp(string exp){this->exp = exp;}
    void elimineSpace();
    bool checkSyntax();
    bool checkSemantic();
    Litterale& eval();
    ~Expression(){}
    Atome& extract();// TODO : extrait l'atome de l'expression, traite l'atome (cherche s'il existe, le crée sinon) ou alors exception et message d'erreur
    std::queue<std::string> shuntingYard();
    bool isType(const char * t) const;
    bool estVrai()const{return true;}

};


#endif /* Expression_hpp */
