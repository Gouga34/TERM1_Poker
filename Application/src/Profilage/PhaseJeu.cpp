/*========================================================================
Nom: CalibrageIdeal.cpp       Auteur: Manuel CHATAIGNER
Maj: 20/05/2015            Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe PhaseJeu.
=========================================================================*/

#include "../../include/Profilage/PhaseJeu.h"

PhaseJeu::PhaseJeu()
{

}

int PhaseJeu::getNbPartiesRealisees() const
{
    return nbPartiesRealisees;
}

int PhaseJeu::getNbPartiesMinimum() const
{
    return nbPartiesMinimum;
}

int PhaseJeu::getGains() const
{
    return gains;
}

void PhaseJeu::nouvellePhase(int nbParties)
{
    nbPartiesMinimum = nbParties;
    nbPartiesRealisees = 0;
    gains = 0;
}

void PhaseJeu::partieTerminee(int gainsPartie)
{
    nbPartiesRealisees++;
    gains += gainsPartie;
}
