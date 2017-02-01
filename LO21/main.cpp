#include "projet.h"
#include "Expression.hpp"
#include "Controleur.hpp"
#include "Pile.hpp"
#include "Entiere.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    /*
    NICO : TODO sauvegarde de contexte
    LOUIS TODO vue secondaire + interaction clavier, beep et pile
    NICO TODO operateur de pile et de boucle + MAX retablir CTRL commande
    NICO TODO EVal expression avec atome

    MAX change EDIT and CTRL commande
    MAX TODO verifier toute les touches avec un ordinateur pave tactile
    ALL TODO suppr comment
    ALL TODO clean include
    TODO nombre maximum de caractere sur une ligne de la pile
    TODO personnaliser la caltos
    */
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
