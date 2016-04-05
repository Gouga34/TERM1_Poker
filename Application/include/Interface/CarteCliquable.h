/*========================================================================
Nom: CarteCliquable.h       Auteur: Manuel CHATAIGNER
Maj: 09/03/2015          Creation: 09/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant une carte cliquable.
=========================================================================*/

#ifndef CARTECLIQUABLE_H
#define CARTECLIQUABLE_H

#include "GraphicCard.h"

class CarteCliquable : public GraphicCard
{
    Q_OBJECT

    protected:

        void mousePressEvent(QMouseEvent *event);

    signals:

        void clicked(int m_id);

    public:

        CarteCliquable(int rang, int couleur);
        ~CarteCliquable();
};

#endif // CARTECLIQUABLE_H
