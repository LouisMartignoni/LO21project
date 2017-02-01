#ifndef Pile_hpp
#define Pile_hpp

#include "projet.h"
#include <QObject>

class Memento
{
    friend class Pile;
     deque<Litterale*> state_pile;
public:
    Memento(deque<Litterale*> p):state_pile(p){}


};

class Pile :  public QObject {
  Q_OBJECT
    friend class Controleur;
    static Pile* instance;
    deque<Litterale*> pile;
    QString message;
    unsigned int maxAffiche;
    string lastOp;
    Litterale* lastArg1;
    Litterale* lastArg2;
    Litterale* lastArg3;
    Pile();
    ~Pile();
    Pile& operator=(const Pile&) = delete;
    Pile (const Pile&) = delete;
  public:
    static Pile& getInstance();
    static void libererInstance();
    void setlastOp(const string& s){lastOp=s;}
    string getlastOp()const{return lastOp;}
    void setlastArg1(Litterale* l){lastArg1=l;}
    void setlastArg2(Litterale* l){lastArg2=l;}
    void setlastArg3(Litterale* l){lastArg3=l;}
    Litterale* getlastArg1()const{return lastArg1;}
    Litterale* getlastArg2()const{return lastArg2;}
    Litterale* getlastArg3()const{return lastArg3;}
    QString getMessage() const;
    void setMessage(const QString& msg);
    unsigned int getMaxAffiche() const;
    void setMaxAffiche(unsigned int);
    unsigned int getSize() const;
    bool isEmpty();
    void push(Litterale& l);
    void pop();
    Litterale& top() const;
    std::deque<Litterale*>::const_iterator getIteratorBegin() const;
    std::deque<Litterale*>::const_iterator getIteratorEnd() const;
    Memento* SavePile(){return new Memento(pile);}
    void dup();
    void drop(){pile.pop_back();}
    void swap();
    void lastop();
    void lastargs();
    void clear(){pile.clear();}
    void reinstateMemento(Memento* mem);
    void IFT();
    void IFTE();
    void While();
  signals:
    void modificationEtat();
};


#endif
