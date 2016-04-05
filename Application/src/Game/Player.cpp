#include "../../include/Game/Player.h"
#include "../../include/Game/Game.h"

namespace game {

    Player::Player(bool isDealer, int cave, int position)
        : m_isDealer(isDealer), m_cave(cave), m_position(position),
          m_currentBet(0), m_highestBet(0), m_totalBet(0), m_actionsCounter(new int[3]()) {

    }

    Player::~Player() {
        delete [] m_actionsCounter;
    }

    double Player::getWinningChances() const {
        return m_winningChances;
    }

    void Player::setWinningChances(double winningChances) {
        m_winningChances = winningChances;
    }

    int Player::getCave() const {
        return m_cave;
    }

    int Player::getStartingCave() const {
        return m_startingCave;
    }

    void Player::setCave(int cave) {
        m_cave = cave;
    }

    void Player::setStartingCave(int cave) {
        m_startingCave = cave;
    }

    int Player::getPosition() const {
        return m_position;
    }

    std::vector<game::Card> Player::getHand() const {
        return m_hand;
    }

    void Player::setGame(Game* game) {
        m_game = game;
    }

    Game* Player::getGame() const {
        return m_game;
    }

    void Player::changeDealer() {
        m_isDealer = !m_isDealer;
    }

    int Player::getCurrentBet() const {
        return m_currentBet;
    }

    void Player::setCurrentBet(int currentBet) {
        m_currentBet = currentBet;
    }

    int Player::getHighestBet() const {
        return m_highestBet;
    }

    void Player::setHighestBet(int highestBet) {
        m_highestBet = highestBet;
    }

    int* Player::getActionsCounter() const {
        return m_actionsCounter;
    }

    int Player::getAccumulatedBetsAndRaises() const {
        return m_accumulatedBetsAndRaises;
    }

    void Player::setAccumulatedBetsAndRaises(int accumulatedBetsAndRaises) {
        m_accumulatedBetsAndRaises = accumulatedBetsAndRaises;
    }

    int Player::getTotalBet() const {
        return m_totalBet;
    }

    void Player::setTotalBet(int totalBets) {
        m_totalBet = totalBets;
    }

    void Player::addTokens(int tokens) {
        m_cave += tokens;
    }

    void Player::removesTokens(int tokens) {
        m_cave -= tokens;
    }

    void Player::addCard(game::Card card) {
        if (m_hand.size() == 0) {
            m_hand.push_back(card);
        } else {
            if (card.getRank() == 1) {
                game::Card c = m_hand.at(0);
                m_hand.pop_back();
                m_hand.push_back(card);
                m_hand.push_back(c);
            } else if (m_hand.at(0).getRank() == 1) {
                m_hand.push_back(card);
            } else if (m_hand.at(0).getRank() < card.getRank()) {
                game::Card c = m_hand.at(0);
                m_hand.pop_back();
                m_hand.push_back(card);
                m_hand.push_back(c);
            } else {
                m_hand.push_back(card);
            }
        }
    }

    bool Player::isDealer() {
        return m_isDealer;
    }

    void Player::resetHand() {
        m_hand.clear();
    }

    void Player::clearHand() {
        m_hand.clear();
    }

    void Player::resetActionsCounter() {
        memset(m_actionsCounter, 0, 3);
    }
}
