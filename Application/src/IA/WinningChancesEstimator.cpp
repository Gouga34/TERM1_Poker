#include "../../include/IA/WinningChancesEstimator.h"
#include "../../include/Jeu/Card.h"

#include <string>
#include <unordered_map>
#include <vector>


WinningChancesEstimator::WinningChancesEstimator(Jeu *game, Joueur *player, int testsNumber)
    : m_testsNumber(testsNumber), m_estimateResult(0), m_game(game), m_player(player) {

}

WinningChancesEstimator::~WinningChancesEstimator() {

}

double WinningChancesEstimator::getEstimateResult() const {
    return m_estimateResult;
}

void WinningChancesEstimator::setTestsNumber(int testsNumber) {
    m_testsNumber = testsNumber;
}

void WinningChancesEstimator::setGame(Jeu *game) {
    m_game = game;
}

void WinningChancesEstimator::setPlayer(Joueur *player) {
    m_player = player;
}

void WinningChancesEstimator::run() {
    estimateWinningChances();
}

void WinningChancesEstimator::estimateWinningChances() {

    double numberOfPartsWon = 0;
	int position;
    int tableSize;
    std::vector<game::Card> table;
    std::vector<game::Card> opponentsHand;
    std::vector<game::Card> deck;
	
    for (int t = 0; t < m_testsNumber; t++) {
        deck.clear();
        table.clear();
        opponentsHand.clear();

        deck = newDeck();
        WinningChancesEstimator::shuffle(deck);

        table = m_game->getTable();
        tableSize =  5 - (int) table.size();

        for (int i = 0; i < tableSize; i++) {
            position = rand() % deck.size();
            table.push_back(deck.at(position));
            deck.erase(deck.begin() + position);
        }

        for (int i = 0; i < 2; i++) {
            position = rand() % deck.size();
            opponentsHand.push_back(deck.at(position));
            deck.erase(deck.begin() + position);
        }
		
        RESULTAT_PARTIE compareHandsResult = assessor::Assessor::compareHands(table,m_player->getMain(), opponentsHand);

        if (compareHandsResult == GAGNE) {
            ++numberOfPartsWon;

        }
	}
	
    m_estimateResult = numberOfPartsWon / m_testsNumber;
}


std::vector<game::Card> WinningChancesEstimator::newDeck() {
    std::vector<game::Card> deck;
	
    for (int i = COULEUR_CARTE::PIQUE; i <= COULEUR_CARTE::CARREAU; i++) {
        for (int j = RANG_CARTE::AS; j <= RANG_CARTE::K; j++) {
            game::Card carte(j,i);
            deck.push_back(carte);
		}
	}

    deck.erase(deck.begin() + m_player->getMain().at(0).getId());
    deck.erase(deck.begin() + m_player->getMain().at(1).getId());

	return deck;
}

void WinningChancesEstimator::shuffle(std::vector<game::Card>& deck) {
	std::random_shuffle(deck.begin(),deck.end());
}
