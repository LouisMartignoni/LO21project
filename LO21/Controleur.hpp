#ifndef Controleur_hpp
#define Controleur_hpp

#include "projet.h"
#include "Pile.hpp"
#include "Strategy.hpp"

class Controleur : public Strategy{
    static Controleur* instance;
    Controleur(){}
    ~Controleur();
    static vector<Memento*> mementoList;
    static unsigned int IndexMemento;
    Controleur& operator=(const Controleur&);
    Controleur (const Controleur&);
public:
    static Controleur& getInstance();
    static void libererInstance();
    static void addMemento(Memento* mem);
    //void executer();
    //void traitementLigne(std::string str);
    void undo();
    void redo();
    bool isType(const char * t) const;

};



#endif /* Controleur_hpp */
