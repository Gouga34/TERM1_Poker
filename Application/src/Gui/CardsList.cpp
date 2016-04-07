/*========================================================================
Nom: ListeCartes.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ListeCartes.
=========================================================================*/

#include "../../include/Gui/CardsList.h"
#include "../../include/Gui/GraphicCard.h"


namespace gui
{
    CardsList::CardsList() : QHBoxLayout()
    {
        setSpacing(10);
        setAlignment(Qt::AlignLeft);
    }

    CardsList::~CardsList()
    {

    }

    void CardsList::addCards(std::vector<game::Card> cards)
    {
        for (unsigned int i = 0; i < cards.size(); i++) {
            GraphicCard *card = new GraphicCard(cards.at(i));
            addWidget(card);
        }
    }

    void CardsList::clear()
    {
        QLayoutItem *item;

        while ((item = this->takeAt(0)) != 0) {
            delete item->widget();
            delete item;
        }
    }
}
