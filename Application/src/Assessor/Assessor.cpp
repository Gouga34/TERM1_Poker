/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 04/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Evaluateur.
=========================================================================*/
#include "../../include/Assessor/Assessor.h"
#include "../../include/Constantes.h"

namespace assessor {

    RESULTAT_PARTIE Assessor::compareHands(std::vector<Carte> table,
                                           std::vector<Carte> handPlayer1,
                                           std::vector<Carte> handPlayer2){

        PlayerCards hand1(table, handPlayer1);
        PlayerCards hand2(table, handPlayer2);

        if (hand1.getHandCategory() > hand2.getHandCategory()) {
            return RESULTAT_PARTIE::GAGNE;
        }
        else if (hand1.getHandCategory() < hand2.getHandCategory()) {
            return RESULTAT_PARTIE::PERDU;
        }
        else {
            return (hand1.compareEqualHandCategories(hand2));
        }
    }
}
