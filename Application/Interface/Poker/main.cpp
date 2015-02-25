/*========================================================================
Nom: main.cpp              Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant le main de l'application graphique.
=========================================================================*/

#include <QApplication>
#include "Fenetre.h"
#include "../../include/Jeu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Jeu *jeu = new Jeu(2, 20, 1000, 0);

    Fenetre fenetre(jeu);
    fenetre.show();

    return app.exec();
}
