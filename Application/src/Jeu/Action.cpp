/*========================================================================
Nom: Action.cpp       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015            Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Action.
=========================================================================*/

#include "../../include/Jeu/Action.h"


Action::Action() : action(PAS_ENCORE_D_ACTION), montant(-1)
{

}

Action::Action(ACTION ac, int m) : action(ac), montant(m)
{

}

Action::~Action()
{

}

ACTION Action::getAction() const
{
    return action;
}

int Action::getMontant() const
{
    return montant;
}

void Action::setAction(ACTION a)
{
    action = a;
}

void Action::setMontant(int m)
{
    montant = m;
}
