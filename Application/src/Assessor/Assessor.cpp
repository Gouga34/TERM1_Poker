/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 04/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Evaluateur.
=========================================================================*/

#include "../../include/Assessor/Assessor.h"
#include "../../include/Constants.h"

namespace assessor {

    GAME_RESULT Assessor::compareHands(std::vector<game::Card> table,
                                           std::vector<game::Card> handPlayer1,
                                           std::vector<game::Card> handPlayer2){

        PlayerCards hand1(table, handPlayer1);
        PlayerCards hand2(table, handPlayer2);

        if (hand1.getHandCategory() > hand2.getHandCategory()) {
            return GAME_RESULT::WON;
        }
        else if (hand1.getHandCategory() < hand2.getHandCategory()) {
            return GAME_RESULT::LOOSE;
        }
        else {
            return (hand1.compareEqualHandCategories(hand2));
        }
    }
}
