//
//  Strategy.cpp
//  ProjetLO21
//
//  Created by Nicolas on 2016-06-08.
//  Copyright © 2016 Nicolas. All rights reserved.
//

#include "Strategy.hpp"
#include "Litterale.hpp"
#include "Litteralemanager.h"
#include "Controleur.hpp"
#include "Expression.hpp"
#include "Entiere.h"
#include "Programme.hpp"
#include "projet.h"

//REGEX Atome
regex correctSyntaxAtome("[[:upper:]][[:upper:][:digit:]]*");
regex notCapital("[^[:upper:]].*");
regex smallLetters(".*[[:lower:]].*");

//REGEX
regex correcteReel("[[:digit:]]*[.]{1}[[:digit:]]*");
regex correcteEntier("[[:digit:]]+");


std::string retablirEtatLigne(std::queue<std::string> fileTemp, std::string dernierEltraite){

    std::string ligne = dernierEltraite;
    while(!fileTemp.empty()){
        ligne += fileTemp.front();
        fileTemp.pop();
    }
    return ligne;
}

void Strategy::traitementRPN(std::string str, bool traiteProg){
    //std::vector<std::string> pileTemp = stringToVector(str);
    std::queue<std::string> fileTemp = stringToQueue(str);
    //std::queue<std::string> fileTemp = vectorToQueue(pileTemp);

    Pile& Pile = Pile::getInstance();
    
    LitteraleManager& litMng = LitteraleManager::getInstance();
    
    //afficheQueue(fileTemp);
    
    while(!fileTemp.empty()){
        
        std::string s = fileTemp.front();
        fileTemp.pop();

        cout << s << "est entier : " << EstEntier(s) << endl;

        if(s == " ") continue;
        if(EstNumerique(s) && regex_match(s, correcteReel)) {//Réel
            Pile.push(litMng.addLitterale(&bonType(stof(s))));
            
        }
        else if(EstEntier(s) && regex_match(s,correcteEntier)){//Entier
            Pile.push(litMng.addLitterale(&bonType(stoul(s))));
            
        }
        else if(regex_match(s,regex("^'.*'$"))){//match une expression
            string s1 = s.substr(1,s.length()-2);
            cout << "Expression : " << s1 << endl;
            Expression* exp = new Expression(s1);
            Pile.push(litMng.addLitterale(exp));
        }
        else if(regex_match (s, regex("^\\[.*\\]$"))){//match un programme
            string s1 = s.substr(1,s.length()-2);
            Programme* prog = new Programme(s1);
            prog->checkSyntax();
            //si on est déjà en train de traiter un programme
            if(traiteProg == true) {
                Pile.push(litMng.addLitterale(prog));
            }
            else{
                //sinon on traite le programme en question
                cout << "on traite" << endl;
                prog->traitementRPN(s1,true);
            }

        }
        else if(regex_match(s, correctSyntaxAtome)){//VAR, PROG, FONC ou rien
            
            //teste si fonction
            auto search1 = fonctions_unaires.find(s);
            auto search2 = fonctions_binaires.find(s);
            auto search3 = find (fonctions_pre_0aires.begin(), fonctions_pre_0aires.end(), s);
            auto search4 = find (fonctions_pre_unaires.begin(), fonctions_pre_unaires.end(), s);
            auto search5 = find (fonctions_pre_binaires.begin(), fonctions_pre_binaires.end(), s);
            auto search6 = find (fonctions_pre_ternaires.begin(), fonctions_pre_ternaires.end(), s);

            if(search1 != fonctions_unaires.end()){//C'est une fonction unaire
                cout <<  s << " est une fonction unaire" << endl;
                if(Pile.getSize() < 1){
                    throw ComputerException("Pas assez d'arguments dans la pile");
                }
                Litterale& l1 = Pile.top();
                Pile.pop();
                Pile.push(litMng.addLitterale(&fonctions_unaires[s](l1)));
            }
            
            else if(search2 != fonctions_binaires.end()){//C'est une fonction binaire
                cout <<  s << " est une fonction binaire" << endl;
                if(Pile.getSize() < 2){
                    throw ComputerException("Pas assez d'arguments dans la pile");
                }
                Litterale& l2 = Pile.top();
                Pile.pop();
                Litterale& l1 = Pile.top();
                Pile.pop();
                Pile.push(litMng.addLitterale(&fonctions_binaires[s](l1,l2)));
                
            }
            //C'est une fonction préféfinie ou un programme
            else if(search6 != fonctions_pre_ternaires.end()){//C'est une fonction ternaire prédéfinie
                cout <<  s << " est une fonction ternaire PREDEFINIE" << endl;
                if(Pile.getSize() < 3){
                    throw ComputerException("Pas assez d'arguments dans la pile");
                }
                if (s == "IFTE")
                    Pile.IFTE();
                else
                    throw ComputerException("Pas d'autre operateur ternaire");
            }
            else if(search5 != fonctions_pre_binaires.end()){//C'est une fonction binaire prédéfinie
                cout <<  s << " est une fonction binaire PREDEFINIE" << endl;
                if(Pile.getSize() < 2){
                    throw ComputerException("Pas assez d'arguments dans la pile");
                }
                if (s == "SWAP")
                    Pile.swap();
                else if(s == "IFT")
                        Pile.IFT();
                else if(s == "WHILE")
                        Pile.While();
                else
                    throw ComputerException("Pas d'autre operateur binaire");
                /*Litterale& l2 = Pile.top();
                Pile.pop();
                Litterale& l1 = Pile.top();
                Pile.pop();
                FnPtrBinPre f = fonctions_pre_binaires[s];
                Pile.(*f)(l1,l2);*/
            }
            else if(search4 != fonctions_pre_unaires.end()){//C'est une fonction unaire prédéfinie
                cout <<  s << " est une fonction unaire PREDEFINIE" << endl;
                if(Pile.getSize() < 1){
                    throw ComputerException("Pas assez d'arguments dans la pile");
                }
                if (s == "DUP")
                    Pile.dup();
                else if(s == "DROP")
                        Pile.drop();
                else if(s == "LASTOP")
                        Pile.lastop();
                else if(s == "LASTARGS")
                        Pile.lastargs();
                else
                    throw ComputerException("Pas d'autre operateur unaire");

                /*Litterale& l1 = Pile.top();
                Pile.pop();
                FnPtrUnPre f = fonctions_pre_unaires[s];
                (*f)(l1);*/
            }

            else if(search3 != fonctions_pre_0aires.end()){//C'est une fonction 0naire prédéfinie
                cout <<  s << " est une fonction 0naire PREDEFINIE" << endl;
                if (s == "REDO")
                    Controleur::getInstance().redo();
                else if(s == "UNDO")
                        Controleur::getInstance().undo();
                else if(s == "CLEAR")
                        Pile.clear();
                else
                    throw ComputerException("Pas d'autre operateur 0naire");
            }
            else{//c'est un programme, variable ou une expression
                
                cout <<  s << " est un programme, variable ou expression" << endl;
                
                
                extern std::map<string,Litterale*> Map;
                afficheMap();
                std::map<string,Litterale*>::iterator it;
                it = Map.find(s); //cherche si l'atome est déjà associé à une littérale
                if (it != Map.end()){
                    Numerique* p = dynamic_cast<Numerique*>(it->second);
                    if (p != 0)//la littérale associée est de type Numérique
                    {
                        Pile.push(litMng.addLitterale(it->second));
                        
                    }
                    else{
                        Expression* p = dynamic_cast<Expression*>(it->second);
                        if (p != 0)//la littérale associée est de type Expression-> évaluer l'expression
                        {
                                EVAL(*p);//évaluation de l'expression
                        }
                        else
                        {
                            Programme* p = dynamic_cast<Programme*>(it->second);
                            if (p != 0)//la littérale associée est de type Programme-> déclencher le programme
                            {
                                EVAL(*p);//évaluation du programme
                            }

                            else {
                                throw ComputerException("Erreur Map Atome Littérale");
                            }

                        }
                    }
                }
                else{//on créé une nouvelle expression avec la chaine de caractère
                    cout << s << " est incunnue, on crée une nouvelle expression" << endl;
                    Expression* exp = new Expression(s);
                    Pile.push(litMng.addLitterale(exp));
                }
                
            }
        }
        
        
        else if (EstOperateur(s)){
            //check size pile : on ne gère que des opérateurs binaires ici, il y a NEG pour le - unaire
            if(Pile.getSize() < 2){
                throw ComputerException("Pas assez d'arguments dans la pile");
            }
            Litterale& l2 = Pile.top();
            Pile.pop();
            Litterale& l1 = Pile.top();
            Pile.pop();

            Pile.push(litMng.addLitterale(CalculBinaire(l1,l2,s)));
            
            
        }
        else{//on replace le reste de la pile dans la ligne de commande et on affiche l'erreur
            throw ComputerException("Opérande n’est pas correcte syntaxiquement", QString::fromStdString(retablirEtatLigne(fileTemp, s)));
            
        }
    }
}
