/*========================================================================
Nom: CarteGraphique.h       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015          Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant une carte graphique.
=========================================================================*/

#ifndef GRAPHICCARD_H
#define GRAPHICCARD_H

#include <QLabel>
#include "../Game/Card.h"

namespace gui
{
    class GraphicCard : public QLabel
    {
        private:

            // Constantes définissant la taille d'une carte
            static const unsigned int CARD_WIDTH;
            static const unsigned int CARD_HEIGHT;

            /**
             * @param rank Rang de la carte
             * @param color Couleur de la carte
             * @return Identifiant de la carte à partir de son rang et sa couleur
             */
            virtual int makeCard(int rank, int couleur) const;

        protected:

            int m_id;

        public:

            static const QString colors[4];

            /**
             * @brief Constructeur de la carte graphique avec son rang et sa couleur
             * @param rank Rang de la carte
             * @param color Couleur de la carte
             */
            GraphicCard(int rank, int color);

            /**
             * @brief Constructeur de la carte graphique avec un objet de type Carte
             * @param c Carte à créer
             */
            GraphicCard(game::Card c);

            virtual ~GraphicCard();
    };
}

#endif // GRAPHICCARD_H
