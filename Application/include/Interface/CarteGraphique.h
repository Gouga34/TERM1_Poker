/*========================================================================
Nom: CarteGraphique.h       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015          Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant une carte graphique.
=========================================================================*/

#ifndef CARTEGRAPHIQUE_H
#define CARTEGRAPHIQUE_H

#include <QLabel>
#include "../Jeu/Carte.h"

class CarteGraphique : public QLabel
{
    private:

        // Constantes définissant la taille d'une carte
        static const unsigned int LARGEUR_CARTE;
        static const unsigned int HAUTEUR_CARTE;

        /**
         * @param rang Rang de la carte
         * @param couleur Couleur de la carte
         * @return Identifiant de la carte à partir de son rang et sa couleur
         */
        int makeCard(int rang, int couleur) const;

    protected:

        int id;

    public:

        static const QString couleurs[4];

        /**
         * @action Constructeur de la carte graphique avec son rang et sa couleur
         * @param rang Rang de la carte
         * @param couleur Couleur de la carte
         */
        CarteGraphique(int rang, int couleur);

        /**
         * @action Constructeur de la carte graphique avec un objet de type Carte
         * @param c Carte à créer
         */
        CarteGraphique(Carte c);

        ~CarteGraphique();
};

#endif // CARTEGRAPHIQUE_H
