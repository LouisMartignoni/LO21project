#include "Controleur.hpp"
#include "projet.h"
// #include "Litterale.hpp"
#include "Pile.hpp"


/*//REGEX Atome
regex correctSyntaxAtome("[[:upper:]][[:upper:][:digit:]]*");
regex notCapital("[^[:upper:]].*");
regex smallLetters(".*[[:lower:]].*");

//REGEX
regex correcteReel("[[:digit:]]*[.]{1}[[:digit:]]*");*/

bool Controleur::isType(const char * t) const{
    string s(typeid(this).name());
    if (s.find(t) != std::string::npos) {
        return true;
    }
    return false;
}


Controleur* Controleur::instance = nullptr;
vector<Memento*> Controleur::mementoList;
unsigned int Controleur::IndexMemento = -1;

Controleur& Controleur::getInstance() {
    if(!instance) {
        instance = new Controleur();
    }

    return *instance;
}

void Controleur::libererInstance() {
    if (instance)
        delete instance;
}

Controleur::~Controleur() {
    libererInstance();
}

/*void Controleur::executer(){
    char c;
    string commande;
    bool lectureLitterale = false;

    do {
        cout<<"?-";
        cin>>c;
        commande+=c;
        if(c == 39){
            lectureLitterale = !lectureLitterale;
        }
        if (!lectureLitterale && EstOperateur(patch::to_string(c))) {

            traitementRPN(commande);
                commande = "";
            cout <<  "Pile :" <<  Pile::getInstance().top().toString();


        }

    }while(c!='Q');
}*/

void Controleur::undo() {
    if (IndexMemento == 0) {
        throw ComputerException("Undo impossible!!");
    }
    Pile& pile = Pile::getInstance();
    pile.reinstateMemento(mementoList[--IndexMemento]);
}

void Controleur::redo() {
    if (IndexMemento == mementoList.size() - 1) {
        throw ComputerException("Redo impossible!");
    }
    Pile& pile = Pile::getInstance();
    pile.reinstateMemento(mementoList[++IndexMemento]);
}

void Controleur::addMemento(Memento* mem){
    if (IndexMemento != mementoList.size() - 1) { //if changing something after undos
        for (unsigned int i=IndexMemento+1;i<mementoList.size();i++){
            delete mementoList[i];
        }
        //mementoList.erase(mementoList.begin()+IndexMemento+1,mementoList.end());
    }
    mementoList.push_back(mem);
    IndexMemento++;
    }

