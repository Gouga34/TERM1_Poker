/*========================================================================
Nom: main.cpp              Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant le main de l'application graphique.
=========================================================================*/

#include <QApplication>
#include "../../include/Interface/Window.h"
#include "../../include/Game/Game.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Window window;
    window.show();

    return app.exec();
}
