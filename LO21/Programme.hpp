#ifndef Programme_hpp
#define Programme_hpp

#include "Litterale.hpp"
#include "Strategy.hpp"

//TODO tout est à faire

 class Operande{
 public:
 Operande(){}
 virtual ~Operande(){}
 };



class Programme : public Litterale, public Strategy{//Strategy -> traitementRPN
    std::string contenu;
public:
    explicit Programme(std::string s):contenu(s){cout << "Programme cree" << endl;}
    ~Programme(){}
    std::string getContenu()const {return contenu;}
    bool checkSyntax()const ;
    bool checkSemantic() const;
    bool isType(const char * t) const;
    std::string toString() const;
    inline bool estVrai()const{return true;}
};


#endif /* Programme_hpp */
