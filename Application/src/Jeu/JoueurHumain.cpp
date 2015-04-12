/*========================================================================
Nom: JoueurHumain.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/04/2015            Creation: 11/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe JoueurHumain.
=========================================================================*/


#include "../../include/Jeu/JoueurHumain.h"


JoueurHumain::JoueurHumain(bool estDealer, int jetons, int position, Fenetre *f)
    : Joueur(estDealer, jetons, position), fenetre(f)
{

}

JoueurHumain::JoueurHumain(const JoueurHumain& joueur, Fenetre *f)
    : Joueur(joueur), fenetre(f)
{

}

JoueurHumain::~JoueurHumain()
{

}

Action JoueurHumain::jouer(){

    return fenetre->getAction();
}
