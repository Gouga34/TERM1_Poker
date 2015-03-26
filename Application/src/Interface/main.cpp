/*========================================================================
Nom: main.cpp              Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant le main de l'application graphique.
=========================================================================*/

#include <QApplication>
#include "../../include/Interface/Fenetre.h"
#include "../../include/Jeu/Jeu.h"
#include "../../include/Interface/Logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Jeu *jeu = new Jeu(2, 20, 1000, 0.0, 0.0);

    Fenetre fenetre(jeu);
    fenetre.show();

    Logger::creerInstance(&fenetre);

    return app.exec();
}
