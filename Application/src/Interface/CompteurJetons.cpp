/*========================================================================
Nom: CompteurJetons.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/03/2015            Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CompteurJetons.
=========================================================================*/

#include "../../include/Interface/CompteurJetons.h"


CompteurJetons::CompteurJetons() : QLCDNumber()
{
    setMaximumSize(100, 50);
    setSegmentStyle(QLCDNumber::Flat);
    setStyleSheet("background-color: #89DF57");
}

CompteurJetons::~CompteurJetons()
{

}
