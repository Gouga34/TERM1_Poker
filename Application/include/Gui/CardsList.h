/*========================================================================
Nom: ListeCartes.h       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015          Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe correspondant à un ensemble de cartes contenues
               dans un layout.
=========================================================================*/

#ifndef CARDSLIST_H
#define CARDSLIST_H

#include <QHBoxLayout>
#include <QList>
#include <QLabel>
#include "../Game/Card.h"

namespace gui
{
    class CardsList : public QHBoxLayout
    {
        public:

            CardsList();
            virtual ~CardsList();

            /**
             * @brief Ajoute les cartes de la liste passées en paramètres à l'objet courant
             * @param cards Ensemble de cartes à ajouter
             */
            virtual void addCards(std::vector<game::Card> cards);

            /**
             * @brief Retire l'ensemble des cartes de la liste
             */
            virtual void clear();
    };
}

#endif // CARDSLIST_H
