/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 04/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe correspondant l'évaluateur de main, compare 
				deux mains et donne celle ayant le poids le plus fort
=========================================================================*/

#ifndef ASSESSOR_H
#define ASSESSOR_H

#include "PlayerCards.h"
#include "../Jeu/Carte.h"
#include <iostream>
#include <vector>

namespace assessor {

    class Assessor {
        public:
            /**
            * @brief compare les deux mains pour voir laquelle a le poids le plus fort
            * @return 0 si hand1 a gagné, 1 si elle a perdu et 2 si égalité
            */
            static RESULTAT_PARTIE compareHands(std::vector<Carte> table, std::vector<Carte> handPlayer1, std::vector<Carte> handPlayer2);
    };
}

#endif
