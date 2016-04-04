/*========================================================================
Nom: JoueurHumain.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/04/2015            Creation: 11/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe JoueurHumain.
=========================================================================*/


#include "../../include/Jeu/JoueurHumain.h"


JoueurHumain::JoueurHumain(bool estDealer, int jetons, int position, ContenuFenetreHumain *f)
    : Joueur(estDealer, jetons, position), fenetre(f)
{

}

JoueurHumain::JoueurHumain(const JoueurHumain& joueur, ContenuFenetreHumain *f)
    : Joueur(joueur), fenetre(f)
{

}

JoueurHumain::~JoueurHumain()
{

}

bool JoueurHumain::isHumain() const {
    return true;
}

Action JoueurHumain::play(){

    return fenetre->getAction();
}
