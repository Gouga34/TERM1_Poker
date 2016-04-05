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

        ACTION m_action;

        int m_tokens;

    public:

        Action();
        Action(ACTION ac, int tokens = -1);
        virtual ~Action();

        virtual ACTION getAction() const;
        virtual int getTokens() const;

        virtual void setAction(ACTION action);
        virtual void setTokens(int tokens);
};

#endif // ACTION_H
