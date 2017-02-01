#include <regex>
#include "Atome.hpp"

using namespace std;

//REGEX Atome
const regex correctSyntaxAtome("[[:upper:]][[:upper:][:digit:]]*");
const regex notCapital("[^[:upper:]].*");
const regex smallLetters(".*[[:lower:]].*");


bool Atome::checkSyntax(){

    if(regex_match (nom, correctSyntaxAtome)){
        return true;
    }

    else if(regex_match (nom, notCapital)){
        throw ComputerException("La première lettre d'un atome doit être une majuscule");
        return false;
    }
    else if(regex_match (nom, smallLetters)){
        throw ComputerException("Les minuscules sont interdites dans un atome");
        return false;
    }
    else {
        throw ComputerException("Vérifiez la syntaxe de l'atome");
        return false;
    }

}

/*pour checker si l'atome est déjà associé à une variable, un programme, ou un opérateur prédéfini*/
/*bool Atome::checkExist(){
 if (nom[0]<65 || nom[0]>90)return false;
 for (unsigned int i=1;i<nom.length();i++){
 if (nom[i]<48 || nom[i]>90)return false;
 if (nom[i]>=58 && nom[i]<65)return false;}
 return true;}

 */

bool Atome::isType(const char * t) const{
    string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}


string Atome::toString()const{
    stringstream f;
    f<<nom;
    return f.str();
}

AtomeManager::AtomeManager(const AtomeManager& atm):atms(new Atome*[10]),nb(0),cap(10){
    for (unsigned int i=0;i<nb;i++)
        *atms[i]=*atm.atms[i];}

AtomeManager& AtomeManager::operator=(const AtomeManager& atm){
    if (&atm==this) return *this;
    if(cap<atm.nb)reserve(atm.nb);
    nb=atm.nb;
    for (unsigned int i=0;i<nb;i++) *atms[i]=*atm.atms[i];
    return *this;
}

void AtomeManager::reserve(unsigned int n){
    if (n>cap){
        Atome** newTab=new Atome*[cap+10];
        for (unsigned int i=0;i<cap;i++)
            *newTab[i]=*atms[i];
        Atome** old;
        cap=n;
        old=atms;
        atms=newTab;
        delete[] old;}}

Atome& AtomeManager::AddAtome(Atome& n){
    if (nb==cap)
        reserve(cap+10);
    *atms[nb++]=n;
    return n;
}

void AtomeManager::removeAtome(Atome& a){
    unsigned int i=0;
    while(i<nb && atms[i] !=&a)i++;
    if (atms[i] != &a)
        throw "erreur Atome non existant";
    delete atms[i];
    for (unsigned int j=i;j<nb;j++) atms[j]=atms[j+1];
    nb--;
}
