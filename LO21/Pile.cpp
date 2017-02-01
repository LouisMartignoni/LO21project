#include "Pile.hpp"
#include "Entiere.h"

Pile* Pile::instance = nullptr;
Pile::Pile(): message(""), maxAffiche(0) {

}

Pile::~Pile() {
    libererInstance();
}

Pile& Pile::getInstance() {
    if (!instance)
        instance = new Pile();

    return *instance;
}

void Pile::libererInstance() {
    if (instance)
        delete instance;
}

QString Pile::getMessage() const {
    return message;
}

void Pile::setMessage(const QString& msg) {
    message = msg;
}

void Pile::setMaxAffiche(unsigned int i) {
    maxAffiche = i;
}

unsigned int Pile::getMaxAffiche() const {
    return maxAffiche;
}

void Pile::push(Litterale& l) {
    pile.push_back(&l);
    emit modificationEtat();
}


void Pile::pop() {
    if(getSize() == 0)
        throw ComputerException("Erreur : Pile vide");
     pile.pop_back();
}

Litterale& Pile::top() const {
    if(getSize() == 0)
        throw ComputerException("Erreur : Pile vide");
    else
        return *pile.back();
}

unsigned int Pile::getSize() const {
    return pile.size();
}

bool Pile::isEmpty() {
    return pile.empty();
}

deque<Litterale*>::const_iterator Pile::getIteratorBegin() const {
    deque<Litterale*>::const_iterator i = pile.cbegin();
    return i;
}

deque<Litterale*>::const_iterator Pile::getIteratorEnd() const {
    deque<Litterale*>::const_iterator i = pile.cend();
    return i;
}

void Pile::dup() {
    pile.push_back(pile.back());
}

void Pile::swap(){
    Litterale& it1=this->top();
    setlastArg1(&it1);
    this->pop();
    Litterale& it2=this->top();
    setlastArg2(&it2);
    this->pop();
    this->push(it1);
    this->push(it2);
}

void Pile::reinstateMemento(Memento* mem) {
    pile = mem->state_pile;
    //emit modificationEtat();
}

void Pile::lastop(){
    auto search1 = fonctions_unaires.find(lastOp);
    auto search2 = fonctions_binaires.find(lastOp);
    if(search1 != fonctions_unaires.end()){//C'est une fonction unaire
        cout <<  lastOp << " est une fonction unaire" << endl;
        if(this->getSize() < 1){
            //throw ComputerException("Pas assez d'arguments dans la pile");
            cout<<"Pas assez d'arguments dans la pile";
        }
        Litterale& l1 = this->top();
        this->pop();
        this->push(fonctions_unaires[lastOp](l1));

    }

    else if(search2 != fonctions_binaires.end()){//C'est une fonction binaire
        cout <<  lastOp << " est une fonction binaire" << endl;
        if(this->getSize() < 2){
            //throw ComputerException("Pas assez d'arguments dans la pile");
            cout<<"Pas assez d'arguments dans la pile";
        }
        Litterale& l2 = this->top();
        this->pop();
        Litterale& l1 = this->top();
        this->pop();
        this->push(fonctions_binaires[lastOp](l1,l2));

    }
}


void Pile::lastargs(){

    auto search1 = fonctions_unaires.find(lastOp);
    auto search2 = fonctions_binaires.find(lastOp);
    if(search1 != fonctions_unaires.end()){
                push(*lastArg1);
    }
    else if(search2 != fonctions_binaires.end()){
            push(*lastArg1);
            push(*lastArg2);
    }
    else if (lastOp=="SWAP"){
            push(*lastArg1);
            push(*lastArg2);
            cout<<"c chiant";}
}

void Pile::IFT(){//TODO demander exécution
    if(this->getSize() < 2){
            throw ComputerException("Pas assez d'arguments dans la pile");
        }
    Litterale& arg1=this->top();
    lastArg1=&arg1;
    pop();
    Litterale& arg2=this->top();
    lastArg2=&arg2;
    pop();
    if(arg2.estVrai()){
        //push(EVAL(arg1));
    }
}

void Pile::IFTE(){//TODO demander exécution
    if(this->getSize() < 3){
            throw ComputerException("Pas assez d'arguments dans la pile");
        }
    Litterale& arg1=top();
    lastArg1=&arg1;
    pop();
    Litterale& arg2=top();
    lastArg2=&arg2;
    pop();
    Litterale& arg3=top();
    lastArg3=&arg3;
    pop();
     /*if(arg3.estVrai())
        //push(EVAL(arg2));
    //push(EVAL(arg1));*/
}

void Pile::While(){//TODO demander exécution
    Litterale& arg1=top();
    lastArg1=&arg1;
    pop();
    Litterale& arg2=top();
    lastArg2=&arg2;
    pop();
    /*while(arg2.estVrai())
        push(EVAL(arg1));*/
    }
