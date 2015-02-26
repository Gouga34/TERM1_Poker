/*========================================================================
Nom: ListeCartes.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ListeCartes.
=========================================================================*/

#include "ListeCartes.h"
#include <CarteGraphique.h>


ListeCartes::ListeCartes() : QHBoxLayout()
{
    setSpacing(10);
    setAlignment(Qt::AlignLeft);
}

ListeCartes::~ListeCartes()
{

}

void ListeCartes::ajoutCartes(std::vector<Carte> cartes)
{
    for (int i = 0; i < cartes.size(); i++) {
        CarteGraphique *c = new CarteGraphique(cartes.at(i));
        this->addWidget(c);
    }
}

void ListeCartes::vider()
{
    QLayoutItem *item;

    while ((item = this->takeAt(0)) != 0) {
        delete item->widget();
        delete item;
    }
}
