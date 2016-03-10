/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 09/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Evaluateur.
=========================================================================*/

#include "../../include/Assessor/PlayerCards.h"
#include "../../include/Constantes.h"

PlayerCards::PlayerCards(std::vector<Carte> table, std::vector<Carte> playersHand) : m_occurrences(), m_weight(0) {
    initializeOccurrences();
    fillTab(table,playersHand);
    calculatesHandCategory();
}

PlayerCards::~PlayerCards() {

}

FORCE_MAIN PlayerCards::getHandCategory() const {
    return m_handCategory;
}

int PlayerCards::getWeight() const {
    return m_weight;
}

void PlayerCards::setHandCategory(FORCE_MAIN handCategory) {
    m_handCategory = handCategory;
}

void PlayerCards::setWeight(int weight) {
    m_weight = weight;
}

void PlayerCards::initializeOccurrences() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 5; ++j) {
            m_occurrences[i][j] = 0;
        }
    }
}

void PlayerCards::fillTab(std::vector<Carte> table, std::vector<Carte> playersHand) {
    registerCardsOccurrences(playersHand);
    registerCardsOccurrences(table);
}

void PlayerCards::registerCardsOccurrences(std::vector<Carte> cardsList) {
    for (auto a_card : cardsList) {
        int cardRank = a_card.getRang();
        int cardColor = a_card.getCouleur();

        if (cardRank == AS) {
            ++m_occurrences[0][cardColor];
            ++m_occurrences[13][cardColor];

            ++m_occurrences[0][4];
            ++m_occurrences[13][4];
        }
        else {
            ++m_occurrences[cardRank-1][cardColor];
            ++m_occurrences[cardRank-1][4];
        }
        ++m_occurrences[14][cardColor];
    }
}

bool PlayerCards::isStraight(int start, int line) const {
    int i = start;

    while (i < 14 && m_occurrences[i][line] >= 1 && (start + 5) > i) {
        ++i;
	}

    int end = start + 5;

    return (end == i);
}

bool PlayerCards::containsStraight(int line) {
    int i = 0;
    int cpt = 0;

    while (i < 14 && cpt < 5){
        if (m_occurrences[i][line] >= 1) {
            ++cpt;
		}
        else {
            cpt = 0;
		}
        ++i;
	}

    if (cpt == 5) {
        setWeight(i);
	}

    return (cpt == 5);
}

int PlayerCards::identicalCards(int nb, int nbTimes) const {
    int weight = 0;
    int cpt = 0;

    for (int i = 13; i > 0; --i) {
        if (m_occurrences[i][4] == nb) {
            ++cpt;

            if (weight == 0) {
                weight = i;
            }

            if (cpt == nbTimes) {
                return weight;
			}
		}
	}
	return -1;
}

bool PlayerCards::checkIfFlushAndSetCombinaison() {
    for (int i = 0; i < 4; ++i) {
        if (m_occurrences[14][i] >= 5) {
            //Le poids est alors égal à la ligne de la couleur
			//afin de pouvoir trouver plus tard si égalité la carte la plus haute de la couleur 
			//sans avoir à chercher à nouveau quelle couleur c'est
            setWeight(i);
            setHandCategory(COULEUR);

            return true;
		}
	}

	return false;
}

bool PlayerCards::checkIfQuinteFlushRoyaleAndSetCombinaison() {
    int i = 0;
    while (i < 4 && getHandCategory() == 0) {
        if (m_occurrences[14][i] >= 5) {
            if (isStraight(9,i)) {
                setHandCategory(QUINTE_FLUSH_ROYALE);
                setWeight(14);
                return true;
            }
        }
        ++i;
    }

    return false;
}

bool PlayerCards::checkIfQuinteFlushAndSetCombinaison() {
    int i = 0;
    while (i < 4 && getHandCategory() == 0) {
        if (m_occurrences[14][i] >= 5 && containsStraight(i)) {
            setHandCategory(QUINTE_FLUSH);
            return true;
        }
        ++i;
    }

    return false;
}

bool PlayerCards::checkIfCarreAndSetCombinaison() {
    int card = -1;

    if ((card = identicalCards(4)) > 0) {
        setHandCategory(CARRE);
        setWeight(card);
        card = 0;
    }
    return (card != -1);
}

bool PlayerCards::checkIfFullAndSetCombinaison() {
    int card;

    if ((card = identicalCards(3)) > 0 && identicalCards(2) > 0) {
        setHandCategory(FULL);
        setWeight(card);
        return true;
    }

    return false;
}

bool PlayerCards::checkIfBrelanAndSetCombinaison() {
    int card;

    if ((card = identicalCards(3)) > 0) {
        setHandCategory(BRELAN);
        setWeight(card);
        return true;
    }
    return false;
}

bool PlayerCards::checkIfDoublePaireAndSetCombinaison() {
    int card;

    if ((card = identicalCards(2,2)) > 0) {
        setHandCategory(DOUBLE_PAIRE);
        setWeight(card);
        return true;
    }
    return false;
}

bool PlayerCards::checkIfPaireAndSetCombinaison() {
    int card;

    if ((card = identicalCards(2,1)) > 0) {
        setHandCategory(PAIRE);
        setWeight(card);
        return true;
    }

    return false;
}

bool PlayerCards::checkIfSuiteAndSetCombinaison() {
    if (containsStraight(4)) {
        setHandCategory(SUITE);
        return true;
    }

    return false;
}

void PlayerCards::calculatesHandCategory() {

    if (checkIfQuinteFlushRoyaleAndSetCombinaison()) {
        return;
    }

    if (checkIfQuinteFlushAndSetCombinaison()) {
        return;
    }

    if (checkIfCarreAndSetCombinaison()) {
        return;
    }

    if (checkIfFullAndSetCombinaison()) {
        return;
    }

    if (checkIfFlushAndSetCombinaison()) {
        return;
    }

    if (checkIfSuiteAndSetCombinaison()) {
        return;
    }

    if (checkIfBrelanAndSetCombinaison()) {
        return;
    }

    if (checkIfDoublePaireAndSetCombinaison()) {
        return;
    }

    if (checkIfPaireAndSetCombinaison()) {
        return;
    }

    //Si on n'est rentré dans aucun cas,
    //la seule combinaison possible est la carte haute.
    setHandCategory(CARTE_HAUTE);
}

void PlayerCards::setHighestCardWeight() {
    int lineNumber = -1;

    if (getHandCategory() == COULEUR) {
        lineNumber = getWeight();
    }
    else if (getHandCategory() == CARTE_HAUTE) {
        lineNumber = 4;
    }

    int i = 13;

    while (i > 0) {
        if (m_occurrences[i][lineNumber] > 0) {
            setWeight(i);
            break;
        }
        --i;
    }
}

void PlayerCards::calculateBasicWeight() {
   setHighestCardWeight();
}

RESULTAT_PARTIE PlayerCards::compareWeights(PlayerCards hand2) {
    if (getWeight() > hand2.getWeight()) {
        return GAGNE;
    }
    else if (getWeight() < hand2.getWeight()) {
        return PERDU;
    }
    else {
        return EGALITE;
    }
}

RESULTAT_PARTIE PlayerCards::compareEqualFourOfAKind(PlayerCards hand2) {
    int i = 13;

    //On calcule la carte la plus haute autre que celles du carré pour this
    while (i > 0) {
        hand2.setWeight(0);
        setWeight(0);

        if (m_occurrences[i][4] < 4 && m_occurrences[i][4] > 0) {
            setWeight(i);
        }

        if (hand2.m_occurrences[i][4] < 4 && hand2.m_occurrences[i][4] > 0) {
            hand2.setWeight(i);
        }

        if (hand2.getWeight() != getWeight()) {
            break;
        }

        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE PlayerCards::compareEqualFullHouse(PlayerCards hand2) {
    setWeight(identicalCards(2));
    hand2.setWeight(hand2.identicalCards(2));

    return compareWeights(hand2);
}

RESULTAT_PARTIE PlayerCards::compareEqualFlush(PlayerCards hand2) {
    int i = 13;
    int color = getWeight();

    while (i > 0) {
        hand2.setWeight(0);
        setWeight(0);

        if (m_occurrences[i][color] > 0) {
            setWeight(i);
        }
        if (hand2.m_occurrences[i][color] > 0) {
            hand2.setWeight(i);
        }

        if (getWeight() != hand2.getWeight()) {
            break;
        }

        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE PlayerCards::compareThreeOfAKind(PlayerCards hand2) {
    int i = 13;

    while (i > 0) {
        hand2.setWeight(0);
        setWeight(0);

        if (m_occurrences[i][4] > 0 && m_occurrences[i][4] != 3) {
            setWeight(i);
        }
        if (hand2.m_occurrences[i][4] > 0 && hand2.m_occurrences[i][4] != 3) {
            hand2.setWeight(i);
        }
        if (getWeight() != hand2.getWeight()) {
            break;
        }
        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE PlayerCards::compareTwoPair(PlayerCards hand2) {
    int i = 13;
    int poidsCarte = getWeight();

    //On commence par comparer la seconde paire
    while (i > 0) {
        hand2.setWeight(0);
        setWeight(0);

        if (m_occurrences[i][4] == 2 && i != poidsCarte) {
            setWeight(i);
        }
        if (hand2.m_occurrences[i][4] == 2 && i != poidsCarte) {
            hand2.setWeight(i);
        }
        if(getWeight() != hand2.getWeight()) {
            break;
        }

        --i;
    }

    //Si la seconde paire est la même, on compare la carte la plus haute
    if (getWeight() == hand2.getWeight()) {
        i = 13;
        int hand1PairCardWeight = getWeight(), hand2PairCardWeight = hand2.getWeight();

        //On remet les poids à 0 pour compter juste une seule carte.
        setWeight(0);
        hand2.setWeight(0);

        while (i > 0){
            if ((m_occurrences[i][4] == 1 || (i < hand1PairCardWeight && m_occurrences[i][4] >= 1))
                    && getWeight() == 0) {
                setWeight(i);
            }
            if ((hand2.m_occurrences[i][4] == 1 || (i < hand2PairCardWeight && m_occurrences[i][4] >= 1))
                    && hand2.getWeight() == 0) {
                hand2.setWeight(i);
            }
            if (getWeight() > 0 || hand2.getWeight() > 0){ //si au moins un nouveau poids, on arrête
                break;
            }
            --i;
        }
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE PlayerCards::comparePair(PlayerCards hand2) {
    int i = 13;

    while (i > 0) {
        hand2.setWeight(0);
        setWeight(0);

        if (m_occurrences[i][4] == 1) {
            setWeight(i);
        }
        if (hand2.m_occurrences[i][4] == 1) {
            hand2.setWeight(i);
        }

        if (getWeight() != hand2.getWeight()) {
            break;
        }
        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE PlayerCards::compareHightCard(PlayerCards hand2) {
    int i = getWeight();

    while (i > 0) {
        hand2.setWeight(0);
        setWeight(0);

        if (m_occurrences[i][4] == 1) {
            setWeight(i);
        }
        if (hand2.m_occurrences[i][4] == 1) {
            hand2.setWeight(i);
        }

        if(getWeight() != hand2.getWeight()) {
            break;
        }

        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE PlayerCards::compareEqualHandCategories(PlayerCards hand2) {
    if (getWeight() == 0) {
        calculateBasicWeight();
 	}
    if (hand2.getWeight() == 0) {
        hand2.calculateBasicWeight();
 	}

    if (getWeight() == hand2.getWeight()) {
        if (getHandCategory() == CARRE) {
           return compareEqualFourOfAKind(hand2);
 		}

        else if (getHandCategory() == FULL) {
           return compareEqualFullHouse(hand2);
 		}
        else if (getHandCategory() == COULEUR) {
           return compareEqualFlush(hand2);
 		}
        else if (getHandCategory() == BRELAN) {
            return compareThreeOfAKind(hand2);
 		}
        else if (getHandCategory() == DOUBLE_PAIRE) {
           return compareTwoPair(hand2);
 		}
        else if (getHandCategory() == PAIRE) {
           return comparePair(hand2);
 		}
        else {
           return compareHightCard(hand2);
		}
 	}
    else {
        return compareWeights(hand2);
    }
 }
