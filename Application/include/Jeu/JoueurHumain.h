/*========================================================================
Nom: JoueurHumain.h       Auteur: Manuel CHATAIGNER
Maj: 11/04/2015          Creation: 11/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant un joueur humain.
=========================================================================*/

#ifndef JOUEURHUMAIN_H
#define JOUEURHUMAIN_H

#include "Joueur.h"
#include "../Interface/ContenuFenetreHumain.h"

class JoueurHumain : public Joueur
{
    private:

        ContenuFenetreHumain *fenetre;

    public:

        JoueurHumain(bool estDealer, int jetons, int position, ContenuFenetreHumain *f);
        JoueurHumain(const JoueurHumain& joueur, ContenuFenetreHumain *f);
        virtual ~JoueurHumain();

        virtual bool estHumain() const;

        virtual Action jouer();
};

#endif // JOUEURHUMAIN_H
