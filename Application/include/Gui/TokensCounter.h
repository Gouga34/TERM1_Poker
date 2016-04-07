/*========================================================================
Nom: CompteurJetons.h       Auteur: Manuel CHATAIGNER
Maj: 11/03/2015          Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant un LCD Number pour l'interface graphique.
=========================================================================*/

#ifndef TOKENSCOUNTER_H
#define TOKENSCOUNTER_H

#include <QLCDNumber>

namespace gui
{
    class TokensCounter : public QLCDNumber
    {
        public:

            TokensCounter();
            virtual ~TokensCounter();
    };
}

#endif // TOKENSCOUNTER_H
