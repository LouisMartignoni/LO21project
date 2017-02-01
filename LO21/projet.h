#ifndef PROJET_H_INCLUDED
#define PROJET_H_INCLUDED


#include <stack>
#include <regex>
#include <algorithm>
#include<math.h>
#include<queue>
#include <list>
#include <map>
#include <iterator>
#include <deque>
#include "ComputerException.h"
#include "Litterale.hpp"
#include "LitteraleManager.h"

class Litterale;
class Entiere;
class Reelle;
class Pile;


using namespace std;

namespace patch{
    template < typename T > std::string to_string( const T& n ){
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

int PGCD(int a, int b);
std::vector<string> stringToVector(std::string str);
std::queue<string> stringToQueue(std::string str);
void afficheStack(stack<string> &s);
void afficheQueue(queue<string> &s);
std::queue<std::string> vectorToQueue(std::vector<string> file);
bool Precedence(string operateur1, string operateur2);
bool EstFonction(string caractere);
bool EstOperateur(string chaine);
bool EstOperateur(char c);
bool EstEntier(string s);
bool EstNumerique(char c);
bool EstNumerique(string s);
bool hasSpace(std::string str);
std::string dernierOperateur(std::string);

Litterale& bonType(double pReelleNum, double pReelleDen=1, double pImNum=0, double pImDen=1);
Litterale& operator+(Litterale& l1, Litterale& l2);
Litterale& operator-(Litterale& l1, Litterale& l2);
Litterale& operator*(Litterale& l1, Litterale& l2);
Litterale& operator/(Litterale& l1, Litterale& l2);
Litterale& $(Litterale& l1, Litterale& l2);
Litterale& POW(Litterale& l1, Litterale& l2);
Litterale& NEG(Litterale& l);
Litterale& SIN(Litterale& l);
Litterale& COS(Litterale& l);
Litterale& TAN(Litterale& l);
Litterale& ARCSIN(Litterale& l);
Litterale& ARCTAN(Litterale& l);
Litterale& ARCCOS(Litterale& l);
Litterale& EXP(Litterale& l);
Litterale& LN(Litterale& l);
Litterale& SQRT(Litterale& l);
Litterale& NUM(Litterale& l);
Litterale& DEN(Litterale& l);
Litterale& RE(Litterale& l);
Litterale& IM(Litterale& l);
Litterale& NORM(Litterale& l);
Litterale& ARG(Litterale& l);

Litterale& DIV(Litterale& l1, Litterale& l2);
Litterale& MOD(Litterale& l1, Litterale& l2);
Litterale& operator!=(Litterale& l1, Litterale& l2);
Litterale& operator<(Litterale& l1, Litterale& l2);
Litterale& operator>(Litterale& l1, Litterale& l2);
Litterale& AND(Litterale& l1, Litterale& l2);
Litterale& OR(Litterale& l1, Litterale& l2);
Litterale& NOT(Litterale& l);

void EVAL(Litterale&);
void STO(Litterale&,Litterale&);

//Priorite operateurs TODO : Verifiez que les op?rateurs num?riques ont priorit? sup?rieures sur logiques
static std::map<string, int> operateurs_arithmetiques = {{ "+", 2 },{ "-", 2 },{ "*", 3 },{ "/", 3 }, {"$",4}};
//Fonctions, donne nb param?tre
typedef Litterale& (*FnPtrUn)(Litterale&);
static std::map<string, FnPtrUn> fonctions_unaires = {
    {"NEG", NEG},
    {"SIN", SIN},
    {"COS", COS},
    {"TAN", TAN},
    {"ARCSIN", ARCSIN},
    {"ARCTAN", ARCTAN},
    {"ARCCOS", ARCCOS},
    {"EXP", EXP},
    {"LN", LN},
    {"NUM", NUM},
    {"DEN", DEN},
    {"RE", RE},
    {"IM", IM},
    {"NORM", NORM},
    {"ARG", ARG},
    {"SQRT",SQRT},
    {"NOT",NOT}
};

typedef Litterale& (*FnPtrBin)(Litterale&, Litterale&);
static std::map<string, FnPtrBin> fonctions_binaires ={{"POW", POW}, {"AND", AND}, {"OR", OR}, {"MOD",MOD},{"DIV",DIV}};

//TODO FInir les op?rateurs pr?d?finis
static std::list<string> fonctions_pre_0aires ={"REDO", "UNDO","CLEAR"};
static std::list<string> fonctions_pre_unaires ={"EVAL", "DUP", "DROP", "LASTOP", "LASTARGS"};
static std::list<string> fonctions_pre_binaires ={"STO", "SWAP", "IFT", "WHILE"};
static std::list<string> fonctions_pre_ternaires ={"IFTE"};


void afficheQueue(queue<string> &s);
void afficheMap();
Litterale* CalculBinaire(Litterale& l1, Litterale& l2, string operateur);
bool estVrai(Entiere& l);
bool estVrai(Reelle& l);


#endif // PROJET_H_INCLUDED
