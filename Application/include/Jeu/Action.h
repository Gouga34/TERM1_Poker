/*========================================================================
Nom: Action.h       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015          Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant une action de poker.
=========================================================================*/

#include "../Constantes.h"

#ifndef ACTION_H
#define ACTION_H

class Action
{
    private:

        ACTION action;

        int montant;

    public:

        Action();
        Action(ACTION ac, int m = -1);
        ~Action();

        ACTION getAction() const;
        int getMontant() const;

        void setAction(ACTION a);
        void setMontant(int m);
};

#endif // ACTION_H
