/*========================================================================
Nom: CalibrageIdeal.cpp       Auteur: Manuel CHATAIGNER
Maj: 20/05/2015            Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe PhaseJeu.
=========================================================================*/

#include "../../include/Profiling/GamePhase.h"

GamePhase::GamePhase()
{

}

int GamePhase::getNbPartsDone() const
{
    return m_nbPartsDone;
}

int GamePhase::getNbMinParts() const
{
    return m_nbMinParts;
}

int GamePhase::getGains() const
{
    return m_gains;
}

void GamePhase::newPhase(int nbParts)
{
    m_nbMinParts = nbParts;
    m_nbPartsDone = 0;
    m_gains = 0;
}

void GamePhase::finishedPart(int partGains)
{
    ++m_nbPartsDone;
    m_gains += partGains;
}
