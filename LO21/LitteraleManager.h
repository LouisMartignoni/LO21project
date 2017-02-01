#ifndef LITTERALEMANAGER_H
#define LITTERALEMANAGER_H

#include "projet.h"

class LitteraleManager {// pattern singleton
    Litterale** lits;
    unsigned int nb;
    unsigned int nbMax;
    void agrandissementCapacite();
    LitteraleManager():lits(0),nb(0),nbMax(0){}
    ~LitteraleManager();
    LitteraleManager(const LitteraleManager& m);
    LitteraleManager& operator=(const LitteraleManager& m);

    struct Handler{
        LitteraleManager* instance;
        Handler():instance(0){}
        ~Handler(){ delete instance; }
    };
    static Handler handler;
public:
    Litterale& addLitterale(Litterale* lit);
    void removeLitterale(Litterale& e);
    static LitteraleManager& getInstance();
    static void libererInstance();
    class iterator {
        Litterale** current;
        iterator(Litterale** u):current(u){}
        friend class LitteraleManager;
    public:
        iterator():current(0){}
        Litterale& operator*() const { return **current; }
        bool operator!=(iterator it) const { return current!=it.current; }
        iterator& operator++(){ ++current; return *this; }
    };
    iterator begin() { return iterator(lits); }
    iterator end() { return iterator(lits+nb); }

    class const_iterator {
        Litterale** current;
        const_iterator(Litterale** u):current(u){}
        friend class LitteraleManager;
    public:
        const_iterator():current(0){}
        Litterale& operator*() const { return **current; }
        bool operator!=(const_iterator it) const { return current!=it.current; }
        const_iterator& operator++(){ ++current; return *this; }
    };
    const_iterator begin() const { return const_iterator(lits); }
    const_iterator end() const { return const_iterator(lits+nb); }
};



#endif // LITTERALEMANAGER_H
