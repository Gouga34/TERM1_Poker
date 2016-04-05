/*========================================================================
Nom: ContenuFenetre.cpp       Auteur: Manuel Chataigner
Maj: 21/05/2015            Creation: 21/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ContenuFenetre.
=========================================================================*/

#include "../../include/Interface/ContenuFenetre.h"
#include "../../include/Interface/Logger.h"

#include <QScrollBar>

ContenuFenetre::ContenuFenetre(game::Game *j) : QWidget(), jeu(j)
{
    Logger::createInstance(this);
}

ContenuFenetre::~ContenuFenetre()
{
    Logger::deleteInstance();
}

void ContenuFenetre::ajoutLogs(QString contenu)
{
    logs.setText(logs.toPlainText() + contenu + "\n");

    // Scroll automatique vers la fin
    QScrollBar *sb=logs.verticalScrollBar();
    sb->setValue(sb->maximum());
}
