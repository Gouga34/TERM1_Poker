/*========================================================================
Nom: CompteurJetons.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/03/2015            Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CompteurJetons.
=========================================================================*/

#include "../../include/Gui/TokensCounter.h"


namespace gui
{
    TokensCounter::TokensCounter() : QLCDNumber()
    {
        setFixedSize(80, 50);
        setSegmentStyle(QLCDNumber::Flat);
        setStyleSheet("background-color: #89DF57");
    }

    TokensCounter::~TokensCounter()
    {

    }
}
