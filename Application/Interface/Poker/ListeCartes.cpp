/*========================================================================
Nom: ListeCartes.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ListeCartes.
=========================================================================*/

#include "ListeCartes.h"


ListeCartes::ListeCartes() : QHBoxLayout()
{
    setSpacing(10);
    setAlignment(Qt::AlignLeft);
}

ListeCartes::~ListeCartes()
{

}

void ListeCartes::ajoutCartes(QList<QLabel*> cartes)
{
    for (int i = 0; i < cartes.size(); ++i)
    {
        this->addWidget(cartes.at(i));
    }
}
