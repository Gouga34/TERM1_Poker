/*========================================================================
Nom: ListeCartes.h       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015          Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe correspondant à un ensemble de cartes contenues
               dans un layout.
=========================================================================*/

#ifndef LISTECARTES_H
#define LISTECARTES_H

#include <QHBoxLayout>
#include <QList>
#include <QLabel>
#include "../Jeu/Carte.h"

class ListeCartes : public QHBoxLayout
{
    public:

        ListeCartes();
        ~ListeCartes();

        /**
         * @action Ajoute les cartes de la liste passées en paramètres à l'objet courant
         * @param cartes Ensemble de cartes à ajouter
         */
        void ajoutCartes(std::vector<Carte> cartes);

        /**
         * @action Retire l'ensemble des cartes de la liste
         */
        void vider();
};

#endif // LISTECARTES_H
