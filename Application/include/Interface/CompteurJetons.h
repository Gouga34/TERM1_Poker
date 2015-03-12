/*========================================================================
Nom: CompteurJetons.h       Auteur: Manuel CHATAIGNER
Maj: 11/03/2015          Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant un LCD Number pour l'interface graphique.
=========================================================================*/

#ifndef COMPTEURJETONS_H
#define COMPTEURJETONS_H

#include <QLCDNumber>

class CompteurJetons : public QLCDNumber
{
    public:

        CompteurJetons();
        ~CompteurJetons();
};

#endif // COMPTEURJETONS_H
