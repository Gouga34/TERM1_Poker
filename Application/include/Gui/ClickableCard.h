/*========================================================================
Nom: CarteCliquable.h       Auteur: Manuel CHATAIGNER
Maj: 09/03/2015          Creation: 09/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant une carte cliquable.
=========================================================================*/

#ifndef CLICKABLECARD_H
#define CLICKABLECARD_H

#include "GraphicCard.h"

namespace gui
{
    class ClickableCard : public GraphicCard
    {
        Q_OBJECT

        protected:

            virtual void mousePressEvent(QMouseEvent *event);

        signals:

            void clicked(int m_id);

        public:

            ClickableCard(int rank, int color);
            virtual ~ClickableCard();
    };
}

#endif // CLICKABLECARD_H
