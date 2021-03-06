//
//  Expression.cpp
//  ProjetLO21
//
//  Created by Nicolas on 2016-05-25.
//  Copyright © 2016 Nicolas. All rights reserved.
//

#include <regex>

#include "Expression.hpp"
#include "projet.h"

//REGEX Expression
regex nombreProbleme(".*(([[:digit:]][[:alpha:]]+)|([)][[:alnum:]])).*");
regex operateursProbleme("^.*(([-/+*][+/*])+|([-+/*$][)])+|([(][+/*$])+)+.*$");
regex complexeProbleme(".*[$]([+/*]|[[:digit:]]+[[:alpha:]]+|[[:alpha:]]+)+.*");
regex syntaxeCorrecteExpression("^([[:upper:]]|[[:digit:]]|[-,.$ ()+*/\\[\\]])+$");
regex vide("^[ ]$");
regex petitesLettres(".*([[:lower:]]|[^,.$ ()+*/-\\[\\]])+.*");
regex decimalProbleme("^.*[.]([[:digit:]]*[.])+.*$");

bool Expression::isType(const char * t) const{
    string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}



string Expression::toString() const{//TO DO remplacer toutes les tabs et newline en espace
    stringstream f;
    f<< "'" << this->exp << "'" << endl;;
    return f.str();
}

void Expression::elimineSpace(){
    exp.erase(std::remove_if(exp.begin(),exp.end(),[](char x){return std::isspace(x);}),exp.end());
}

bool Expression::checkSyntax(){
    this->elimineSpace();
    if(regex_match (exp, nombreProbleme)){
        throw ComputerException("Expression illégale (nombre invalide)");
        return false;
    }

    if(regex_match (exp, operateursProbleme)){
        throw ComputerException("Expression illégale (problème sur opérateurs)");
        return false;
    }

    else if(regex_match (exp, complexeProbleme)){
        throw ComputerException("Expression illégale (nombre complxe invalide)");
        return false;
    }

    else if(regex_match (exp, vide)){
        throw ComputerException("Expression illégale (expression vide)");
        return false;

    }
    else if(regex_match (exp, decimalProbleme)){
        throw ComputerException("Expression illégale (problème sur décimal)");
        return false;
    }
    else if(regex_match (exp, syntaxeCorrecteExpression)){
        return true;
    }

    else if(regex_match (exp, petitesLettres)){
        throw ComputerException("Expression illégale (petites lettres ou ponctuation interdite)");
        return false;
    }
    else {
        throw ComputerException("Expression illégale (caractère(s) inconnu(s))");
        return false;
    }
}

// enlever stack parametre fonctions
std::queue<std::string> Expression::shuntingYard(){

    if(!this->checkSyntax()) throw ComputerException("Expression incorrecte");

    std::queue<std::string> listOutput; //Utilisé afin de stocker la chaine RPN
    std::stack<string> listStack; //Utilisé afin de stocker les opérateurs le temps de l'algorithme

    //Utilisé afin de stocker les valeur de parametrage en cas de fonction imbriqués, permet une gestion de la profondeur
    stack<int> listStack_parametrage;

    //Variable permettant de déterminer si l'on doit continuer de parcourir l'élément stack ou non lorsque l'on rencontre un opérateur de calcul
    bool parcourStack = false;

    //Utilisé pour detecter la précédence d'un nombre (Gestion des nombres négatifs)
    bool precNum = false;

    //Utilisé pour la constitution des token "variable prédéfini"
    string chaineLectureVariable = "";

    //Utilisé pour la constitution des token "nombre double"
    string chaineLecture = "";

    std::vector<std::string> tokens;

    for ( int i = 0; i < (int) exp.length(); ++i )//Parcours des caractères de la chaîne un par un
    {
        const std::string c( 1, exp[ i ] );
        //if(!listStack.empty())
            //afficheStack(listStack);

        /********************************************************************************
         ********** ETAPE 1 : Creation des tokens de nombres / variables / fonctions ****
         ********************************************************************************/
        //Permet de déterminer s'il s'agit d'un nombre
        if(chaineLectureVariable.empty() && EstNumerique(c))
        {
            chaineLecture += c;
            continue;

        }

        //Variable prédéfinie ou fonction

        if (chaineLecture == "" && !EstOperateur(c) && c != "(" && c != ")" && c != ",")
        {
            //Construction de la variable
            chaineLectureVariable += c;
            //On passe au caractère suivant, puisque le caractère courant a été traité
            continue;
        }
        //Une variable aprÈs une parenthèse est une fonction

        else if (chaineLectureVariable != "" && c == "(")
        {
            //Detection d'une fonction
            //On ajoute la fonction au stack comme un opérateur
            if(!EstFonction(chaineLectureVariable)){
                string exc = chaineLectureVariable + " est une fonction inconnue";
                throw ComputerException(QString::fromStdString(exc));
            }
            else{

                listStack.push(chaineLectureVariable);
                chaineLectureVariable = "";

                //Ajout de la parenthèse et passage à la lecture suivante
                listStack.push(c);
                //listStack_parametrage.push(1);
                continue;
            }
        }

        //Si un token de nombre a précédement été constitué on l'ajoute à la queue
        if (chaineLecture != "" && EstNumerique(chaineLecture)) // && chaineLecture[chaineLecture.length() - 1] != '.')//Remove dernière condition ?
        {
            listOutput.push(chaineLecture);
            //Si un nombre a été détecté, il y a précédence par un nombre, donc opération de calcul sur le symbole '-'
            precNum = true;
            chaineLecture = "";
        }
        else
        {
            cout << chaineLecture;
            //Erreur, un nombre a été mal renseigné (ex nombre decimal incomplet => 3. ou 3.h (avec un caractère)) //TODO 3. est correct, vérifier que ce ne soit pas une lettre
            if (!chaineLecture.empty())
            {
                throw ComputerException("L'un des nombres renseignés est incorrect.");
            }
        }

        //Si un token de variable prédéfinie à précédement été constitué on l'ajoute à la queue
        if (chaineLectureVariable != "")
        {
            listOutput.push(chaineLectureVariable);
            //Si une variable prédéfini a été détecté, il y a précédence par un nombre, donc opération de calcul sur le symbole '-'
            precNum = true;
            chaineLectureVariable = "";
        }

        if (c == ",") //séparateur de fonction
        {
            //L'ensemble des opérateurs doivent être dépilés avant le passage au parametres suivant
            while (listStack.size() > 0 && listStack.top() != "(")
            {
                listOutput.push(listStack.top());
                listStack.pop();
            }

            //Il reste forcement la parenthèse gauche pour identifier le début de fonction, sinon => erreur de parenthèsage
            if (listStack.size() == 0)
                throw ComputerException("Problème de parenthèsage");

            //Incrémentation du nombre de parametres à chaques identification du symbole ','
            int nb_parametre =listStack_parametrage.top() + 1 ;
            listStack_parametrage.pop();
            listStack_parametrage.push(nb_parametre);


            //Lorsque l'on dépile on retombe sur une parenthèse, il n'y a donc pas précédence par un nombre
           precNum = false;

            continue;
        }

        /***********************************************************************************************************
         ****** ETAPE 2 : Creation des tokens operateurs_arithmetiques et de la gestion des nombres negatifs *******
         ***********************************************************************************************************/

        //s'il s'agit d'un opérateur on ajoute au stack
        if (EstOperateur(c))
        {
            if (c == "-" && (!precNum || (listOutput.size() == 0|| (listStack.size() > 0 && listStack.top() == "("))))
            {
                //Gestion des nombres/variables negatifs :
                //Si le nombre est écrit au debut de la chaine (ex : -5 + 6) ou suite à une parenthèse (ex : -5*(-6))
                //Dans ce cas on ajoute un zero avant d'ajouter l'operateur (ex: 0 - 5 + 6 // 0-5*(0-6))
                listOutput.push("0");
                listStack.push(c);
                continue;
            }
            if (c == "$" && (listOutput.size() == 0|| (listStack.size() > 0 && !precNum)))
            {
                //Gestion des nombres/variables complexes :
                //Si le nombre est écrit au debut de la chaine (ex : -5 $ 6) ou suite à une parenthèse (ex : -5*($6))
                //Dans ce cas on ajoute un zero avant d'ajouter l'operateur (ex: 0 - 5 6 $// 0-5*(0-6))
                listOutput.push("0");
                listStack.push(c);
                continue;
            }

            else
            {
                parcourStack = true;
                //Verification de la précédence afin de déterminer si on ajoute l'operateur à la queue ou au stack
                while (parcourStack)
                {
                    if (listStack.size() > 0 && Precedence(c, listStack.top()))
                    {
                        //si la priorite : caractere <= listack.peek()
                        listOutput.push(listStack.top());
                        listStack.pop();
                    }
                    else
                    {
                        //s'il n'y a pas de précédence caractere > listack.peek OU caractere est autre chose qu'un opérateur
                        listStack.push(c); //On ajoute l'opérateur à la liste STack
                        parcourStack = false; //Arrêt de la boucle de lecture
                    }
                }
            }
            //Si un opérateur a été détecté, il n'y a pas précédence par un nombre
            precNum = false;

            //On passe au record suivant
            continue;
        }

        /**************************************************************************************
         ****** ETAPE 3 : Creation des tokens de parenthèse ET de gestion des fonctions *******
         **************************************************************************************/

        //Dès que l'on  rencontre une parenthèse gauche on la stack, elle servira au dépilement (plus tard dans l'algo)
        if (c == "(")
        {
            listStack.push(c);

            //Si une parenthèse a été détecté, il n'y a pas précédence par un nombre
            precNum = false;

            //On passe au record suivant
            continue;
        }

        //Lorsque l'on trouve une parenthèse fermante on traite l'ensemble du contenu des parenthèses
        if (c == ")")
        {

            //Tant que l'on ne trouve pas de parenthèse gauche on depile les operateurs_arithmetiques dans la file
            while (listStack.size() > 0 && listStack.top() != "(")
            {
                listOutput.push(listStack.top());
                listStack.pop();

            }

            //En sortie de boucle on controle le premier element de la pile
            if (listStack.size() > 0 && listStack.top() == "(")
            {

                listStack.pop(); //On retire la parenthèse ouvrante de la pile, car elle a été traitée
                //On recompte le nombre d'éléments du stack pour s'assurer qu'il ne s'agisse pas d'une parenthèse non attaché à une fonction
               if (listStack.size() > 0 && EstFonction(listStack.top()) == 1)
                {   //Une fois les parametres ajoutés, on peut ajouter la fonction qui utilisera le parametrage
                    listOutput.push(listStack.top());
                    listStack.pop();

                    precNum = true;
                }

                continue;
            }
            else
            {
                if(listStack.empty()) cout << "Vide" << endl;
                throw ComputerException("Problème de parenthèsage");
            }
        }
        //La boucle est pre-interrompue après tout traitement, cette ligne ne doit jamais s'executer sinon => erreur
        //ex : probleme de nombre decimaux incomplets
        throw ComputerException("Une erreur est survenue, problème technique, algorithme mal implémenté.");
    }



    //TRAITEMENT DE LA DERNIERE CHAINE LUE
    //En sortie de boucle il ne peut rester qu'un nombre(ou variable prédéfinie)
    //Si un token de nombre à précédement été constitué on l'ajoute à la queue
    if (chaineLecture != "" && EstNumerique(chaineLecture))// && chaineLecture.at(chaineLecture.length() - 1) != '.')
    {
        listOutput.push(chaineLecture); //Ajout à la queue du nombre constitué
    }
    else
    {
        //Erreur, un nombre a été mal renseigné (ex nombre decimal incomplet => 3. ou 3.h (avec un caractère))
        if (chaineLecture != "")
        {
            throw ComputerException("L'un des nombres renseignés est incorrect.");
        }
    }

    //Si un token de variable à précédement été constitué on l'ajoute à la queue
    if (chaineLectureVariable != "")
    {
        listOutput.push(chaineLectureVariable); //Ajout à la queue de la variable identifié
    }

    /**************************************************************************************
     ***** ETAPE 4 : vidage de la pile on ajoute l'ensemble des opérateurs à la file ******
     **************************************************************************************/
    int taille = (int)listStack.size();

    //Ajout de la pile a la file
    for (int index = 0; index < taille; index++)
    {
        if (listStack.top() == "(")
        {
            throw ComputerException("Problème de parenthèsage");
        }

        listOutput.push(listStack.top());
        listStack.pop();
    }

    return listOutput;
}



