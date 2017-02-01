#include "projet.h"
#include "Expression.hpp"
#include "Programme.hpp"
#include "Numerique.h"
#include "Entiere.h"
#include "Reelle.h"
#include "Rationelle.h"
#include "Complexe.h"
#include "Atome.hpp"
#include "Pile.hpp"
#include "litteralemanager.h"


std::map<string,Litterale*> Map;
const regex correctSyntaxAtome("[[:upper:]][[:upper:][:digit:]]*");


bool hasSpace(std::string str){
    int i = 0;
    char c;
    while (str[i])
    {
        c=str[i];
        if (isspace(c)) return true;
        i++;
    }
    return false;

}

void STO(Litterale& l,Litterale& ep){//l'expression déjà présente dans la pile est sans guillemets
    Expression* e = dynamic_cast<Expression*>(&ep);
    if(e == 0) throw ComputerException("Le deuxième argument doit être une expression");
    else{
        //****EXPRESSION NE DOIT PAS ETRE OPERATEUR PREDEFINI****////TODO vérifiez avec opérateurs logiques

        string atome = e->getExp();
        string lit = l.getExpression();
        cout << "lit associée : " << lit << endl;
        auto search1 = fonctions_unaires.find(lit);
        auto search2 = fonctions_binaires.find(lit);
        auto search3 = find (fonctions_pre_0aires.begin(), fonctions_pre_0aires.end(), lit);
        auto search4 = find (fonctions_pre_unaires.begin(), fonctions_pre_unaires.end(), lit);
        auto search5 = find (fonctions_pre_binaires.begin(), fonctions_pre_binaires.end(), lit);
        auto search6 = find (fonctions_pre_ternaires.begin(), fonctions_pre_ternaires.end(), lit);

        if(search1 != fonctions_unaires.end() || search2 != fonctions_binaires.end() ||
                search3 != fonctions_pre_0aires.end() || search4 != fonctions_pre_unaires.end() ||
                search5 != fonctions_pre_binaires.end() || search6 != fonctions_pre_ternaires.end()){
            //TODO verifier pas reservé
            throw ComputerException("Erreur : Identificateur réservé");

        }
        else{

            if(regex_match(atome, correctSyntaxAtome)){
                Numerique* p = dynamic_cast<Numerique*>(&l);
                Expression* p1 = dynamic_cast<Expression*>(&l);
                Programme* p2 = dynamic_cast<Programme*>(&l);
                if (p != 0 || p1 != 0 || p2 != 0){
                    extern std::map<string,Litterale*> Map;

                    Map[atome] = &l;//on écrase ou on créé une nouvelle valeur
                    cout << "Affichage de la Map" << endl;
                    afficheMap();

                }
                else {
                    Pile::getInstance().push(LitteraleManager::getInstance().addLitterale(&l));
                    throw ComputerException("Littérale impossible à stocker (Complexe, Atome...)");
                }


            }
            else {
                Pile::getInstance().push(LitteraleManager::getInstance().addLitterale(&l));
                throw  ComputerException("Erreur Atome");
            }
     }
}
}

void EVAL(Litterale& l){
    Expression* lp = dynamic_cast<Expression*>(&l);
    if(lp != 0) {
        cout << "traitement d'une experession" << endl;
        std::string chaineResult="";
        std::queue<string> Result = lp->shuntingYard();
        while(!Result.empty()) {chaineResult+=Result.front() + " ";Result.pop();}
        //Equivalent de traitementLigne(chaineResult);
        lp->traitementRPN(chaineResult);

    }
    else{
            Programme* lp = dynamic_cast<Programme*>(&l);
            if(lp!=0){
                lp->traitementRPN(lp->getContenu(),true);
            }
            else {
                Pile& Pile = Pile::getInstance();
                LitteraleManager& litMng = LitteraleManager::getInstance();
                Pile.push(litMng.addLitterale(&l));
                throw ComputerException("EVAL ne peut pas être appliquée");
            }
    }


}

/// <summary>
/// Indique si un nombre est numerique
/// </summary>
/// <param name="s">Chaine en entrée</param>
/// <returns>Vrai si la chaine est un numérique avec un séparateur '.' </returns>
bool EstNumerique(string s)
{

    for ( int c = 0; c < (int) s.length(); ++c ) {
        if(!isdigit(s[c]) && s[c] != '.') return false;

    }
    return true;
}
/// <summary>
/// Indique si un caractere fait parti des caractère acceptés pour construire un numerique
/// </summary>
/// <param name="s">Caractère en entrée</param>
/// <returns>Vrai si le caractère est un numérique ou un séparateur '.' </returns>
bool EstNumerique(char c)
{
    if (isdigit(c) || c == '.')
    {
        return true;
    }
    return false;
}
/// <summary>
/// Indique si un nombre est un entier
/// </summary>
/// <param name="s">Chaine en entrée</param>
/// <returns>Vrai si le caractère est un entier</returns>
bool EstEntier(string s)
{
    for ( int i = 0; i < (int) s.size(); ++i )
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}


/// <summary>
/// Permet de déterminer si la chaine correspond à un opérateur
/// </summary>
/// <param name="chaine">Chaine à tester</param>
/// <returns>Vrai s'il s'agit d'un opérateur référencé, Faux sinon</returns>
bool EstOperateur(string chaine)
{
    if(chaine.length() > 1) return false;
    else{
        auto search = operateurs_arithmetiques.find(chaine);
        return search != operateurs_arithmetiques.end();
    }
}
/// <summary>
/// Permet de déterminer si la chaine correspond à une fonction UNIQUEMENT SI LA CHAINE VIENT DU STACK
/// Cette FCT Factorise simplement du code, rien de plus
/// </summary>
/// <param name="chaine">Chaine en entrée à identifier</param>
/// <returns>Vrai s'il s'agit d'une fonction, Faux sinon</returns>
/// <remarks>Cette fonction permet uniquement de factoriser du code</remarks>
bool EstFonction(string caractere)//TODO ajouter fonctions manipulation e pile
{

    auto search1 = fonctions_unaires.find(caractere);
    auto search2 = fonctions_binaires.find(caractere);
    auto search3 = find (fonctions_pre_0aires.begin(), fonctions_pre_0aires.end(), caractere);
    auto search4 = find (fonctions_pre_unaires.begin(), fonctions_pre_unaires.end(), caractere);
    auto search5 = find (fonctions_pre_binaires.begin(), fonctions_pre_binaires.end(), caractere);
    auto search6 = find (fonctions_pre_ternaires.begin(), fonctions_pre_ternaires.end(), caractere);
    return (!EstOperateur(caractere) && caractere != "(" && caractere != ")" && caractere != "," &&
            caractere != "." && (search1 != fonctions_unaires.end() || search2 != fonctions_binaires.end() ||
            search3 != fonctions_pre_0aires.end() || search4 != fonctions_pre_unaires.end() ||
            search5 != fonctions_pre_binaires.end() || search6 != fonctions_pre_ternaires.end()));
}



bool Precedence(string operateur1, string operateur2)
{
    //La fonction "Precedence" ne doit être appellé que pour comparer le stack avec un opérateur
    //Cette fonction sert également, uniquement à externalier du code, pour la lisibilité
    int valPrec1, valPrec2;

    //Une fonction à toujours la priorité max, (puisque parenthèsée)
    if (EstFonction(operateur2)){
        return true;
    }

    if (EstFonction(operateur1)){
        return false;
    }

    //S'il ne s'agit pas d'un opérateur référencé (ex : parenthèse) il n'y a pas de précédence
    auto search1 = operateurs_arithmetiques.find(operateur1);
    auto search2 = operateurs_arithmetiques.find(operateur2);
    valPrec1 = search1->second;
    valPrec2 = search2->second;

    if (search1 != operateurs_arithmetiques.end() && search2 != operateurs_arithmetiques.end() )
    {
        return valPrec1 <= valPrec2;
    }
    else
    {
        //Il n'y a précédence que si l'operateur1 possède une priorité plus faible ou égal que l'opérateur2
        return false;
    }
}


int PGCD(int a, int b){
    while(b!=0){
        int c=a%b;
        a=b;
        b=c;
    }
    return a;
}

Litterale& bonType(double pReelleNum, double pReelleDen, double pImNum, double pImDen){
    Numerique* pReelle;

    int r  = PGCD(pReelleNum, pReelleDen);
        if (r!=1 && r!=0){
            pReelleNum /= r;
            pReelleDen /= r;
        }

    if((pReelleNum-floor(pReelleNum))==0){//num entier
            if(pReelleDen == pReelleNum || pReelleNum==0 || pReelleDen==1){//pReelle entier
                    if(pReelleNum==pReelleDen)
                        pReelle = new Entiere(1);
                    else
                        pReelle = new Entiere(pReelleNum);
            }
            else
                if((pReelleDen-floor(pReelleDen))==0)//den entier
                    pReelle = new Rationnelle(pReelleNum, pReelleDen);
                else
                    pReelle = new Reelle(pReelleNum/pReelleDen);//den reelle
    }
    else{
        if(pReelleDen == pReelleNum){//num = den
            pReelle = new Entiere(1);
        }
        else
            pReelle = new Reelle(pReelleNum/pReelleDen);
    }

    if (pImNum == 0)//pas complexe
        return *pReelle;
    else{//complexe
         Numerique* pIm;

         if(pReelle->getPartieReelle()<pow(10,-10))
            pReelle = new Entiere(0);

        int ri  = PGCD(pImNum, pImDen);
        if (ri!=1 && ri!=0){
            pImNum /= ri;
            pImDen /= ri;
        }

        if((pImNum-floor(pImNum))==0){//num entier
                if(pImDen == pImNum || pImNum==0 || pImDen==1){//pIm entier
                    if(pImNum==pImDen)
                        pIm = new Entiere(1);
                    else
                        pIm = new Entiere(pImNum);
                }
                else
                    if(pImDen-floor(pImDen)==0)//den entier
                        pIm = new Rationnelle(pImNum, pImDen);
                    else
                        pIm = new Reelle(pImNum/pImDen);//den reelle
        }
        else{ //num reel
            if (pImNum == pImDen)//num = den
                pIm = new Entiere(1);
            else
                pIm = new Reelle(pImNum/pImDen);
        }
        return *new Complexe(*pReelle, *pIm);
    }

}

Litterale& operator+(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme))
        throw ComputerException("Addition impossible entre ces operateurs !");
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
        return *new Expression('('+l1.getExpression()+")+("+l2.getExpression()+')');

    return bonType(l1.getPartieReelleNum()*l2.getPartieReelleDen()+l2.getPartieReelleNum()*l1.getPartieReelleDen(),
                   l1.getPartieReelleDen()*l2.getPartieReelleDen(),
                   l1.getPartieImNum()*l1.getPartieImDen()+l2.getPartieImNum()*l1.getPartieImDen(),
                   l1.getPartieImDen()*l2.getPartieImDen());
}

Litterale& operator-(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme))
        throw ComputerException("Soustraction impossible entre ces operateurs !");
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
        return *new Expression('('+l1.getExpression()+")-("+l2.getExpression()+')');

    return bonType(l1.getPartieReelleNum()*l2.getPartieReelleDen()-l2.getPartieReelleNum()*l1.getPartieReelleDen(),
                   l1.getPartieReelleDen()*l2.getPartieReelleDen(),
                   l1.getPartieImNum()*l1.getPartieImDen()-l2.getPartieImNum()*l1.getPartieImDen(),
                   l1.getPartieImDen()*l2.getPartieImDen());
}

Litterale& operator*(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme))
        throw ComputerException("Multiplication impossible entre ces operateurs !");
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
            return *new Expression('('+l1.getExpression()+")*("+l2.getExpression()+')');
    if(typeid(l1) == typeid(Complexe) || typeid(l2) == typeid(Complexe))
        return bonType( (l1.getPartieReelleNum()*l2.getPartieReelleNum()*l1.getPartieImDen()*l2.getPartieImDen()) - (l1.getPartieImNum()*l2.getPartieImNum()*l1.getPartieReelleDen()*l2.getPartieReelleDen()),
                   l1.getPartieReelleDen()*l2.getPartieReelleDen()*l1.getPartieImDen()*l2.getPartieImDen(),
                   (l1.getPartieReelleNum()*l2.getPartieImNum()*l1.getPartieImDen()*l2.getPartieReelleDen()) + (l2.getPartieReelleNum()*l1.getPartieImNum()*l2.getPartieImDen()*l1.getPartieReelleDen()),
                   l1.getPartieReelleDen()*l2.getPartieReelleDen()*l1.getPartieImDen()*l2.getPartieImDen());

    return bonType(l1.getPartieReelleNum()*l2.getPartieReelleNum(),
                   l1.getPartieReelleDen()*l2.getPartieReelleDen(),
                   l1.getPartieImNum()*l2.getPartieImNum(),
                   l1.getPartieImDen()*l2.getPartieImDen());

}

Litterale& operator/(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme))
        throw ComputerException("Division impossible entre ces operateurs !");
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
            return *new Expression('('+l1.getExpression()+")/("+l2.getExpression()+')');

    if(typeid(l2) == typeid(Complexe)){
        return bonType(l1.getPartieReelle()*l2.getPartieReelle() + l1.getPartieIm()*l2.getPartieIm(),
                        pow(l2.getPartieReelle(),2)+pow(l2.getPartieIm(),2),
                        l2.getPartieReelle()*l1.getPartieIm() - l2.getPartieIm()*l1.getPartieReelle(),
                        pow(l2.getPartieReelle(),2)+pow(l2.getPartieIm(),2));
    }

    if(l2.getPartieReelleNum()==0)
        throw ComputerException("Division par 0 !");

    return bonType(l1.getPartieReelleNum()*l2.getPartieReelleDen(),
                   l1.getPartieReelleDen()*l2.getPartieReelleNum(),
                   l1.getPartieImNum()*l2.getPartieReelleDen(),
                   l1.getPartieImDen()*l2.getPartieReelleNum());
}


Litterale& $(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) | typeid(l1) == typeid(Expression) ||
            typeid(l2) == typeid(Expression) || typeid(l1) == typeid(Complexe) ||
            typeid(l2) == typeid(Complexe)|| typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme))
        throw ComputerException("Creation de complexe impossible !");

    return bonType(l1.getPartieReelleNum(),
                   l1.getPartieReelleDen(),
                   l2.getPartieReelleNum(),
                   l2.getPartieReelleDen());
}

Litterale& NEG(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Negation impossible !");
    if (typeid(l) == typeid(Expression))
            return *new Expression("NEG("+l.getExpression()+')');
    return bonType(-l.getPartieReelleNum(),
                   l.getPartieReelleDen(),
                   -l.getPartieImNum(),
                   l.getPartieImDen());
}

Litterale& POW(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme))
        throw ComputerException("Puissance impossible entre ces operateurs !");
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
            return *new Expression("POW("+l1.getExpression()+','+l2.getExpression()+')');
    if(typeid(l2) == typeid(Complexe))
        return EXP(l1*LN(l2));
    if(typeid(l1) == typeid(Complexe))
        return POW(NORM(l1),l2)*($(COS(l2*ARG(l1)), SIN(l2*ARG(l1))));
    if(l1.getPartieReelle()<0)
        return bonType(0,
                       1,
                       pow(-l1.getPartieReelle(),l2.getPartieReelle()));

    if(typeid(l2) == typeid(Entiere))
        return bonType(pow(l1.getPartieReelleNum(), l2.getPartieReelleNum()),
                   pow(l1.getPartieReelleDen(),l2.getPartieReelleNum()));

    return bonType(pow(l1.getPartieReelleNum(), l2.getPartieReelle())/pow(l1.getPartieReelleDen(), l2.getPartieReelle()));
}

Litterale& SIN(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Sinus impossible avec cet operateur !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("SIN("+l.getExpression()+')');
    return bonType(sin(l.getPartieReelle())*cosh(l.getPartieIm()),
               1,
               cos(l.getPartieReelle())*sinh(l.getPartieIm()));
}

Litterale& COS(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Cosinus impossible avec cet operateur  !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("COS("+l.getExpression()+')');

    return bonType(cos(l.getPartieReelle())*cosh(l.getPartieIm()),
               1,
               -(sin(l.getPartieReelle())*sinh(l.getPartieIm())));
}

Litterale& TAN(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Cosinus impossible avec cet operateur  !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("TAN("+l.getExpression()+')');

    return SIN(l)/COS(l);
}

Litterale& ARCSIN(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Arc Sinus impossible avec cet operateur !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("ARCSIN("+l.getExpression()+')');
    return $(*new Entiere(), *new Entiere(-1)) * LN($(*new Entiere(), *new Entiere(1))*l+POW(*new Entiere(1) + NEG(l * l),*new Reelle(0.5)));
}

Litterale& ARCCOS(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Arc Cosinus impossible avec cet operateur !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("ARCCOS("+l.getExpression()+')');
    return *new Reelle(3.1415926/2)+NEG(ARCSIN(l));
}

Litterale& ARCTAN(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Arc Tangente impossible avec cet operateur !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("ARCTAN("+l.getExpression()+')');
    if(typeid(l) == typeid(Complexe))
        return *new Reelle(0.5) * ($(*new Entiere(), *new Entiere(1))*(LN(*new Entiere(1) + NEG($(*new Entiere(), *new Entiere(1))*l))+NEG(LN(*new Entiere(1) + ($(*new Entiere(), *new Entiere(1))*l)))));
    return bonType(atan(l.getPartieReelle()));
}

Litterale& EXP(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Exponentielle impossible avec cet operateur !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("EXP("+l.getExpression()+')');
    if(typeid(l) == typeid(Complexe))
        return EXP(RE(l))* ($(COS(IM(l)), SIN(IM(l))));
    return bonType(exp(l.getPartieReelle()));
}

Litterale& LN(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Logarithme impossible avec cet operateur !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("LN("+l.getExpression()+')');
    if(typeid(l) == typeid(Complexe)){
        if(ARG(l).getPartieReelleNum()==0)
            return LN(NORM(l));
        return $(LN(NORM(l)), ARG(l));
    }

    if (l.getPartieReelle()<0 )
        throw ComputerException("Fonction ln non definie pour les nombres negatifs");

    return bonType(log(l.getPartieReelle()));
}

Litterale& SQRT(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
        throw ComputerException("Racine impossible avec cet operateur !");
    if (typeid(l) == typeid(Expression))
         return *new Expression("SQRT("+l.getExpression()+')');

    return POW(l,*new Reelle(0.5));
}

Litterale& NUM(Litterale& l){
    if (typeid(l) == typeid(Expression))
         return *new Expression("NUM("+l.getExpression()+')');
    if(typeid(l) == typeid(Entiere))
        return l;
    if(typeid(l) == typeid(Rationnelle)){
         return *new Entiere(l.getPartieReelleNum());
    }
    throw ComputerException("Recuperation du numérateur impossible !");
}

Litterale& DEN(Litterale& l){
    if (typeid(l) == typeid(Expression))
         return *new Expression("DEN("+l.getExpression()+')');
    if(typeid(l) == typeid(Entiere))
        return *new Entiere(1);
    if(typeid(l) == typeid(Rationnelle)){
        return *new Entiere(l.getPartieReelleDen());
    }
    throw ComputerException("Recuperation du denominateur impossible !");
}

Litterale& RE(Litterale& l){
    if (typeid(l) == typeid(Expression))
         return *new Expression("RE("+l.getExpression()+')');
    if(typeid(l) == typeid(Entiere) || typeid(l) == typeid(Rationnelle) ||  typeid(l) == typeid(Reelle))
        return l;
    if(typeid(l) == typeid(Complexe))
        return bonType(l.getPartieReelleNum(),
                       l.getPartieReelleDen());
    throw ComputerException("Recuperation de la partie reelle impossible !");
}

Litterale& IM(Litterale& l){
    if (typeid(l) == typeid(Expression))
         return *new Expression("IM("+l.getExpression()+')');
    if(typeid(l) == typeid(Entiere) || typeid(l) == typeid(Rationnelle) ||  typeid(l) == typeid(Reelle))
        return *new Entiere();
    if(typeid(l) == typeid(Complexe))
        return bonType(l.getPartieImNum(),
                       l.getPartieImDen());
    throw ComputerException("Recuperation de la partie imaginaire impossible !");
}

Litterale& NORM(Litterale& l){
    if (typeid(l) == typeid(Expression))
         return *new Expression("NORM("+l.getExpression()+')');
    if(typeid(l) == typeid(Entiere) || typeid(l) == typeid(Rationnelle) ||  typeid(l) == typeid(Reelle) || typeid(l) == typeid(Complexe))
        return POW(POW(RE(l),*new Entiere(2))+POW(IM(l),*new Entiere(2)), *new Reelle(0.5));
    throw ComputerException("Calcul du module impossible !");
}

Litterale& ARG(Litterale& l){
    if (typeid(l) == typeid(Expression))
         return *new Expression("ARG("+l.getExpression()+')');
    if(typeid(l) == typeid(Entiere) || typeid(l) == typeid(Rationnelle) ||  typeid(l) == typeid(Reelle))
        return ARCTAN(*new Entiere());
    if(typeid(l) == typeid(Complexe))
        return ARCTAN(IM(l)/RE(l));
    throw ComputerException("Calcul de l argument impossible !");
}

Litterale& DIV(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
            return *new Expression("DIV("+l1.getExpression()+','+l2.getExpression()+')');
    if (typeid(l1) == typeid(Entiere) && typeid(l2) == typeid(Entiere)){
        int e1 = static_cast<int>(l1.getPartieReelleNum());
        int e2 = static_cast<int>(l2.getPartieReelleNum());
        return *new Entiere(e1/e2);
    }

    else
        throw ComputerException("Calcul de division entiere impossible !");
}

Litterale& MOD(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
            return *new Expression("MOD("+l1.getExpression()+','+l2.getExpression()+')');
    if (typeid(l1) == typeid(Entiere) && typeid(l2) == typeid(Entiere)){
        int e1 = static_cast<int>(l1.getPartieReelleNum());
        int e2 = static_cast<int>(l2.getPartieReelleNum());
        return *new Entiere(e1%e2);
    }

    else
        throw ComputerException("Calcul du modulo impossible !");
}

Litterale& operator!=(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme) || typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
        throw ComputerException("Operation logique impossible entre ces operateurs !");
    if(typeid(l1) == typeid(Complexe) || typeid(l2) == typeid(Complexe)){
        if(l1.getPartieReelle()!=l2.getPartieReelle() || l1.getPartieIm()!=l2.getPartieIm())
            return *new Entiere(1);
        else
            return *new Entiere(0);
    }

    if (l1.getPartieReelle() != l2.getPartieReelle())
        return *new Entiere(1);
    else
        return *new Entiere(0);
}

Litterale& operator<(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme) || typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
         throw ComputerException("Operation logique impossible entre ces operateurs !");
    if(typeid(l1) == typeid(Complexe) || typeid(l2) == typeid(Complexe)){
            if(typeid(l1) == typeid(Complexe) && typeid(l2) == typeid(Complexe))
                if(l1.getPartieReelle()<l2.getPartieReelle() && l1.getPartieIm()==l2.getPartieIm())
                    return *new Entiere(1);
                else
                    return *new Entiere(0);
            else
                throw ComputerException("Operation impossible !");
    }

    if (l1.getPartieReelle() < l2.getPartieReelle())
        return *new Entiere(1);
    else
        return *new Entiere(0);
}

Litterale& operator>(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme) || typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
         throw ComputerException("Operation logique impossible entre ces operateurs !");
    if(typeid(l1) == typeid(Complexe) || typeid(l2) == typeid(Complexe)){
            if(typeid(l1) == typeid(Complexe) && typeid(l2) == typeid(Complexe))
                if(l1.getPartieReelle()>l2.getPartieReelle() && l1.getPartieIm()==l2.getPartieIm())
                    return *new Entiere(1);
                else
                    return *new Entiere(0);
            else
                throw ComputerException("Operation impossible !");
    }

    if (l1.getPartieReelle() > l2.getPartieReelle())
        return *new Entiere(1);
    else
        return *new Entiere(0);
}

Litterale& AND(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme))
         throw ComputerException("Operation logique impossible entre ces operateurs !");
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
            return *new Expression("AND("+l1.getExpression()+','+l2.getExpression()+')');
    if(l1.getPartieReelle()==0 || l2.getPartieReelle()==0)
        return *new Entiere(0);
    else
        return *new Entiere(1);
}

Litterale& OR(Litterale& l1, Litterale& l2){
    if (typeid(l1) == typeid(Atome) || typeid(l2) == typeid(Atome) || typeid(l1) == typeid(Programme) || typeid(l2) == typeid(Programme))
         throw ComputerException("Operation logique impossible entre ces operateurs !");
    if (typeid(l1) == typeid(Expression) || typeid(l2) == typeid(Expression))
            return *new Expression("OR("+l1.getExpression()+','+l2.getExpression()+')');
    if(l1.getPartieReelle()==0 && l2.getPartieReelle()==0)
        return *new Entiere(0);
    else
        return *new Entiere(1);
}

Litterale& NOT(Litterale& l){
    if (typeid(l) == typeid(Atome) || typeid(l) == typeid(Programme))
         throw ComputerException("Operation logique avec cet operateur impossible !");
    if (typeid(l) == typeid(Expression))
            return *new Expression("NOT("+l.getExpression()+')');
    if(l.getPartieReelle()==0)
        return *new Entiere(1);
    else
        return *new Entiere(0);
}




void afficheQueue(queue<string> &s)
{
    cout << "Affiche queue ";
    if(s.empty())
    {
        cout << endl;
        return;
    }
    unsigned int count = 0;
    while(count < s.size()){
        count++;
        string x = s.front();
        cout << x;
        s.pop();
        s.push(x);

    }
    cout << "fin affiche" << endl;

}


std::queue<string> stringToQueue(std::string s){

    std::queue<std::string> tokens;
    string chaine = "";

    std::stack<string> pileTmp;//permet l'identification des programmes

    //*** On ne dépile pas si on lit une expression **//
    bool lectureExp = false;

    string str = dernierOperateur(s);
    cout << "chaine analysée : " << s << endl;


    //**PARCOURS DE LA CHAINE DE CARACTERES*//
    for ( int i = 0; i < (int) str.length(); ++i )
    {
        const std::string c( 1, str[ i ] );
        cout << "car :" << c << " ";


        //** DIFFERENCIATION DES CAS **//

        if(c == "'" && pileTmp.empty()){//si on lit une expression alors on ne push pas avant d'avoir atteint le 2e '
            chaine += c;
            //cout << "Lecture Expression" << endl;
            lectureExp = !lectureExp;
            if(lectureExp == false) {
                tokens.push(chaine);
                chaine = "";
                continue;
            }
        }

        else if(c == "[" && !lectureExp){//si on lit un programme alors on ne push pas avant d'avoir atteint le ] correspondant
            //on empile le [ et on continue de lire la chaine jusqu'à arriver à un ] qui ferait dépiler
            //cout << "Lecture Programme" << endl;
            if(chaine != "" && pileTmp.empty()) {
                tokens.push(chaine);
                chaine ="";
            }

            pileTmp.push(c);
            chaine += c;
            continue;
        }
        else if(c == "]" && !lectureExp){
            if(pileTmp.empty()) throw ComputerException("vérifiez la syntaxe !");
            pileTmp.pop();
            chaine += c;
            if(pileTmp.empty()) {
                tokens.push(chaine); //si la pile est vide alors on est arrivé à la fin du traitement du programme, on push
                chaine = "";
                continue;
            }
            else continue;
        }
        else if(c == " " && pileTmp.empty() && lectureExp == false){//quelque chose est prêt à être pushé, sauf si dans un programme ou expression
            cout << "Espace détecté" << chaine << endl;
            if(hasSpace(chaine) || chaine.empty()) {cout << "Suppression Spcae " << endl; continue;}
            tokens.push(chaine);
            chaine = "";
            continue;
        }
        else {
            cout << "Caractère normal" << endl;
            chaine += c;
            continue;

        }
    }


    if(!chaine.empty()) {tokens.push(chaine);}
    if(!pileTmp.empty()) throw ComputerException("Erreur syntaxe programme");

    afficheQueue(tokens);

    return tokens;

}
std::vector<string> stringToVector(std::string str){

    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> tokens(beg, end);
    return tokens;


}

std::queue<std::string> vectorToQueue(std::vector<string> pile){
    std::queue<string> file;

    for(auto& f: pile){
        file.push(f);
    }
    return file;
}


void afficheMap(){
    for(auto ite = Map.begin() ;ite != Map.end() ; ite++){
        cout << ite->first << " coorrespond à " << ite->second->toString() << endl;
    }
}

Litterale* CalculBinaire(Litterale& l1, Litterale& l2, string operateur)
{
    if(operateur == "+"){

        return &(l1 + l2);
    }
    else if(operateur == "-"){

        return &(l1 - l2);
    }
    else if(operateur == "*"){

        return &(l1 * l2);
    }
    else if(operateur == "/"){

        return &(l1 / l2);
    }
    else if(operateur == "$"){

        return &($(l1, l2));
    }
    else if(operateur == "="){

        return &(NOT(l1!=l2));
    }
    else if(operateur == "!="){

        return &(l1!=l2);
    }
    else if(operateur == "<"){

        return &(l1<l2);
    }
    else if(operateur == ">"){

        return &(l1>l2);
    }
    else if(operateur == ">="){

        return &(NOT(l1<l2));
    }
    else if(operateur == "=<"){

        return &(NOT(l1>l2));
    }
    else throw ComputerException("Opérateur incompris");
}

bool estVrai(Entiere& e){
    if(e.getValue()!=0)
        return true;
    return false;}

bool estVrai(Reelle& e){
    if(e.getValue()!=0)
        return true;
    return false;}

std::string dernierOperateur(std::string str){
    string str1 = str;
    string str2 = "";
    const std::string c( 1, str[ str.size() - 1 ] );
    cout << "operateur ?" << c << endl;
    if(EstOperateur(c)){
        cout << "dernier élément ope";
        str1 = str.substr(0,str.size()-1);
        str2 = str.substr(str.size()-1, str.size());
    }
    return str1 + " " + str2;


}
