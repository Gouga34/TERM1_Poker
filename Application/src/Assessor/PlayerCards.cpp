/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 09/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Evaluateur.
=========================================================================*/

#include "../../include/Assessor/PlayerCards.h"
#include "../../include/Constants.h"

namespace assessor {

    PlayerCards::PlayerCards(std::vector<game::Card> table, std::vector<game::Card> playersHand) : m_occurrences(), m_weight(0) {
        initializeOccurrences();
        fillTab(table,playersHand);
        calculatesHandCategory();
    }

    PlayerCards::~PlayerCards() {

    }

    HAND_STRENGHT PlayerCards::getHandCategory() const {
        return m_handCategory;
    }

    unsigned int PlayerCards::getWeight() const {
        return m_weight;
    }

    void PlayerCards::setHandCategory(HAND_STRENGHT handCategory) {
        m_handCategory = handCategory;
    }

    void PlayerCards::setWeight(unsigned int weight) {
        m_weight = weight;
    }

    void PlayerCards::initializeOccurrences() {
        m_occurrences = {{0}};
    }

    void PlayerCards::fillTab(std::vector<game::Card> table, std::vector<game::Card> playersHand) {
        registerCardsOccurrences(playersHand);
        registerCardsOccurrences(table);
    }

    void PlayerCards::registerCardsOccurrences(std::vector<game::Card> cardsList) {
        for (auto a_card : cardsList) {
            unsigned int cardRank = a_card.getRank();
            unsigned int cardColor = a_card.getColor();

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

    bool PlayerCards::isStraight(unsigned int start, unsigned int line) const {
        unsigned int i = start;

        while (i < 14 && m_occurrences[i][line] >= 1 && (start + 5) > i) {
            ++i;
        }

        unsigned int end = start + 5;

        return (end == i);
    }

    bool PlayerCards::containsStraight(unsigned int line) {
        unsigned int i = 0;
        unsigned int cpt = 0;

        while (i < 14 && cpt < 5) {
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

    int PlayerCards::identicalCards(unsigned int nb, unsigned int nbTimes) const {
        unsigned int weight = 0;
        unsigned int cpt = 0;

        for (unsigned int i = 13; i > 0; --i) {
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
        for (unsigned int i = 0; i < 4; ++i) {
            if (m_occurrences[14][i] >= 5) {
                //Le poids est alors égal à la ligne de la couleur
                //afin de pouvoir trouver plus tard si égalité la carte la plus haute de la couleur
                //sans avoir à chercher à nouveau quelle couleur c'est
                setWeight(i);
                setHandCategory(FLUSH);

                return true;
            }
        }

        return false;
    }

    bool PlayerCards::checkIfQuinteFlushRoyaleAndSetCombinaison() {
        unsigned int i = 0;
        while (i < 4 && getHandCategory() == 0) {
            if (m_occurrences[14][i] >= 5) {
                if (isStraight(9,i)) {
                    setHandCategory(ROYAL_FLUSH);
                    setWeight(14);
                    return true;
                }
            }
            ++i;
        }

        return false;
    }

    bool PlayerCards::checkIfQuinteFlushAndSetCombinaison() {
        unsigned int i = 0;
        while (i < 4 && getHandCategory() == 0) {
            if (m_occurrences[14][i] >= 5 && containsStraight(i)) {
                setHandCategory(STRAIGHT_FLUSH);
                return true;
            }
            ++i;
        }

        return false;
    }

    bool PlayerCards::checkIfCarreAndSetCombinaison() {
        int card = -1;

        if ((card = identicalCards(4)) > 0) {
            setHandCategory(FOUR_OF_A_KIND);
            setWeight(card);
            return true;
        }
        return false;
    }

    bool PlayerCards::checkIfFullAndSetCombinaison() {
        int card;

        if ((card = identicalCards(3)) > 0 && identicalCards(2) > 0) {
            setHandCategory(FULL_HOUSE);
            setWeight(card);
            return true;
        }

        return false;
    }

    bool PlayerCards::checkIfBrelanAndSetCombinaison() {
        int card;

        if ((card = identicalCards(3)) > 0) {
            setHandCategory(THREE_OF_A_KIND);
            setWeight(card);
            return true;
        }
        return false;
    }

    bool PlayerCards::checkIfDoublePaireAndSetCombinaison() {
        int card;

        if ((card = identicalCards(2,2)) > 0) {
            setHandCategory(TWO_PAIR);
            setWeight(card);
            return true;
        }
        return false;
    }

    bool PlayerCards::checkIfPaireAndSetCombinaison() {
        int card;

        if ((card = identicalCards(2,1)) > 0) {
            setHandCategory(PAIR);
            setWeight(card);
            return true;
        }

        return false;
    }

    bool PlayerCards::checkIfSuiteAndSetCombinaison() {
        if (containsStraight(4)) {
            setHandCategory(STRAIGHT);
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
        setHandCategory(HIGHT_CARD);
    }

    void PlayerCards::setHighestCardWeight() {
        int lineNumber = -1;

        if (getHandCategory() == FLUSH) {
            lineNumber = getWeight();
        }
        else if (getHandCategory() == HIGHT_CARD) {
            lineNumber = 4;
        }

        unsigned int i = 13;

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

    GAME_RESULT PlayerCards::compareWeights(PlayerCards hand2) {
        if (getWeight() > hand2.getWeight()) {
            return WON;
        }
        else if (getWeight() < hand2.getWeight()) {
            return LOOSE;
        }
        else {
            return EQUALITY;
        }
    }

    GAME_RESULT PlayerCards::compareEqualFourOfAKind(PlayerCards hand2) {
        unsigned int i = 13;

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

    GAME_RESULT PlayerCards::compareEqualFullHouse(PlayerCards hand2) {
        setWeight(identicalCards(2));
        hand2.setWeight(hand2.identicalCards(2));

        return compareWeights(hand2);
    }

    GAME_RESULT PlayerCards::compareEqualFlush(PlayerCards hand2) {
        unsigned int i = 13;
        unsigned int color = getWeight();

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

    GAME_RESULT PlayerCards::compareThreeOfAKind(PlayerCards hand2) {
        unsigned int i = 13;

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

    GAME_RESULT PlayerCards::compareTwoPair(PlayerCards hand2) {
        unsigned int i = 13;
        unsigned int poidsCarte = getWeight();

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
            unsigned int hand1PairCardWeight = getWeight(), hand2PairCardWeight = hand2.getWeight();

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

    GAME_RESULT PlayerCards::comparePair(PlayerCards hand2) {
        unsigned int i = 13;

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

    GAME_RESULT PlayerCards::compareHightCard(PlayerCards hand2) {
        unsigned int i = getWeight();

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

    GAME_RESULT PlayerCards::compareEqualHandCategories(PlayerCards hand2) {
        if (getWeight() == 0) {
            calculateBasicWeight();
        }
        if (hand2.getWeight() == 0) {
            hand2.calculateBasicWeight();
        }

        if (getWeight() == hand2.getWeight()) {
            if (getHandCategory() == FOUR_OF_A_KIND) {
               return compareEqualFourOfAKind(hand2);
            }

            else if (getHandCategory() == FULL_HOUSE) {
               return compareEqualFullHouse(hand2);
            }
            else if (getHandCategory() == FLUSH) {
               return compareEqualFlush(hand2);
            }
            else if (getHandCategory() == THREE_OF_A_KIND) {
                return compareThreeOfAKind(hand2);
            }
            else if (getHandCategory() == TWO_PAIR) {
               return compareTwoPair(hand2);
            }
            else if (getHandCategory() == PAIR) {
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

}
