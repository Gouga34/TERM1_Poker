#include "../../include/Game/Game.h"
#include "../../include/AI/ArtificialIntelligenceProfiling.h"
#include "../../include/Gui/Logger.h"

#include <iterator>

namespace game {

    Game::Game(int nbPlayers, int blind, gui::GameOptions options)
        : m_actions(nbPlayers), m_options(options), m_blind(blind),
          m_currentPlayer(0), m_pot(0), m_dealer(0) {

        srand((unsigned)time(0));

        this->m_deck = newDeck();
        this->shuffleDeck();
    }

    Game::~Game() {
        for (unsigned int i = 0; i < m_players.size(); i++) {
            delete m_players.at(i);
        }
    }

    void Game::addPlayer(Player *player) {
        player->setGame(this);
        m_players.push_back(player);
    }

    void Game::newGame() {
        resetCaves();
        handDistribution();
    }

    void Game::resetCaves() {

        setPot(0);

        for (unsigned int i = 0; i < m_players.size(); i++) {
            if (m_options.resetCaves) {
                getPlayer(i)->setCave(PLAYERS_CAVE);
            }
            else {
                if (getPlayer(i)->getCave() == 0) {
                    getPlayer(i)->setCave(PLAYERS_CAVE);
                }
            }

            getPlayer(i)->setStartingCave(getPlayer(i)->getCave());
        }
    }

    void Game::handDistribution() {

        m_currentStep = GAME_STEPS::PREFLOP;
        m_isGameOver = false;

        resetActions();

        ai::ArtificialIntelligenceProfiling *iaProfilage = static_cast<ai::ArtificialIntelligenceProfiling*>(m_players.at(1));
        iaProfilage->determineGameType();

        getPlayer(0)->setAccumulatedBetsAndRaises(0);
        getPlayer(1)->setAccumulatedBetsAndRaises(0);

        for (int i = 0; i < (int) (2 * m_players.size()); i++) {
           if(m_players.at(i % m_players.size())->getHand().size() != 2) {
                int position = rand() % m_deck.size();
                m_players.at(i % m_players.size())->addCard(m_deck.at(position));
                m_deck.erase(m_deck.begin() + position);
            }
        }

        newStep(GAME_STEPS::PREFLOP);
        blindDistribution();
    }

    void Game::newStep(GAME_STEPS currentStep) {

        if (currentStep != GAME_STEPS::PREFLOP) {

            m_currentPlayer = (m_dealer + 1) % m_players.size();
            m_currentBet = 0;
            m_accumulatedBetsAndRaises = 0;

            resetActions();

            int cardsNumber = 0;
            if (currentStep == GAME_STEPS::FLOP) {
                cardsNumber = 3;
            }
            else if (currentStep == GAME_STEPS::TURN
                     || currentStep == GAME_STEPS::RIVER) {
                cardsNumber = 1;
            }

            tableCardsDistribution(cardsNumber);

            gui::Logger::getInstance()->addLogs("Ajout de cartes sur la table");
        }

        calculateWinningChances();
    }

    void Game::calculateWinningChances() {

        ai::ArtificialIntelligence *ai;
        ai::ArtificialIntelligence *ai2 = static_cast<ai::ArtificialIntelligence*>(getPlayer(1));

        if (!getPlayer(0)->isHumain()) {
            ai = static_cast<ai::ArtificialIntelligence*>(getPlayer(0));
            ai->launchWinningChancesEstimator(TESTS_NUMBER, 2);
        }

        ai2->launchWinningChancesEstimator(TESTS_NUMBER, 2);
        ai2->waitWinningChancesEstimatorResult();

        if (!getPlayer(0)->isHumain()) {
            ai->waitWinningChancesEstimatorResult();
        }
    }

    void Game::tableCardsDistribution(int cardsToDealNumber) {

        if (m_tableTmp.empty()) {
            for (int i = 0; i < cardsToDealNumber; i++) {
                if (m_deck.size() > 0) {
                    int position = rand() % m_deck.size();
                    m_table.push_back(m_deck.at(position));
                    m_deck.erase(m_deck.begin() + position);
                }
            }
        } else {
            for (int i = 0; i < cardsToDealNumber; i++) {
                    m_table.push_back(m_tableTmp.at(0));
                    m_deck.erase(m_tableTmp.erase(m_tableTmp.begin()));
            }
        }
    }

    void Game::blindDistribution() {

        gui::Logger::getInstance()->addLogs("Joueur " + QString::number(getOpponentsPosition(getDealer())) + " : petite blind");

        executeAction(getOpponentsPosition(getDealer()), Action(BET, getBlind()));
        m_actions[getOpponentsPosition(getDealer())].back() = ACTION::SMALL_BLIND;

        gui::Logger::getInstance()->addLogs("Joueur " + QString::number(getDealer()) + " : grosse blind");

        executeAction(getDealer(), Action(RAISE, getBlind()*2));
        m_actions[getDealer()].back() = ACTION::BIG_BLIND;

        m_currentPlayer = getDealer();

        for (unsigned int i = 0; i < m_players.size(); i++) {
            getPlayer(i)->resetActionsCounter();
        }
    }

    gui::GameOptions Game::getOptions() const {
        return m_options;
    }

    GAME_RESULT Game::getGameResult() const {
        return m_gameResult;
    }

    int Game::getDealer() const{
        return m_dealer;
    }

    std::vector<game::Card> Game::newDeck() {

        std::vector<game::Card> deck;

        for (int i = CARD_COLOR::SPADES; i <= CARD_COLOR::DIAMONDS; i++ ) {
            for (int j = CARD_RANK::AS; j<=CARD_RANK::K; j++) {
                game::Card carte(j,i);
                deck.push_back(carte);
            }
        }

        return deck;
    }

    void Game::shuffleDeck() {
        std::random_shuffle(m_deck.begin(), m_deck.end());
    }


    int Game::getBlind() const {
        return m_blind;
    }

    std::vector<game::Card> Game::getDeck() const {
        return m_deck;
    }

    int Game::getCurrentPlayer() const {
        return m_currentPlayer;
    }

    int Game::getOpponentsPosition(int player) const {
        return (player == 0) ? 1 : 0;
    }

    Player* Game::getPlayer(int i) {
        return m_players.at(i);
    }

    int Game::getPot() const {
        return m_pot;
    }


    void Game::setPot(int pot) {
        m_pot = pot;
    }

    void Game::putMoneyIntoGame(int playerPosition, int tokens) {
        setPot(getPot() + tokens);

        getPlayer(playerPosition)->setCurrentBet(tokens);
        getPlayer(playerPosition)->setAccumulatedBetsAndRaises(getPlayer(playerPosition)->getAccumulatedBetsAndRaises() + tokens);

        if (tokens > getPlayer(playerPosition)->getHighestBet()) {
            getPlayer(playerPosition)->setHighestBet(tokens);
        }

        getPlayer(playerPosition)->removesTokens(tokens);
    }

    void Game::bet(int playerPosition, int tokens) {

        //Si on fait pas tapis:
        if (tokens < getPlayer(playerPosition)->getCave()) {
            putMoneyIntoGame(playerPosition, tokens);
            m_currentBet = tokens;
            m_accumulatedBetsAndRaises = tokens;

            m_actions[getPlayer(playerPosition)->getPosition()].push_back(ACTION::BET);
            getPlayer(playerPosition)->getActionsCounter()[0]++;
        }
        else{ //Sinon: tapis
            allIn(playerPosition, BET);
        }
    }

    void Game::allIn(int playerPostion, ACTION action) {

        putMoneyIntoGame(playerPostion, getPlayer(playerPostion)->getCave());
        m_actions[getPlayer(playerPostion)->getPosition()].push_back(ACTION::ALL_IN);

        m_currentBet = getPlayer(playerPostion)->getCave();
        m_accumulatedBetsAndRaises = getPlayer(playerPostion)->getAccumulatedBetsAndRaises();

        if (action == BET || action == RAISE) {
            getPlayer(playerPostion)->getActionsCounter()[0]++;
        }
        else if (getPlayersLastAction(getOpponentsPosition(playerPostion)) == ALL_IN) {
            getPlayer(playerPostion)->getActionsCounter()[1]++;
        }
    }

    void Game::raise(int playerPosition, int tokens) {

        //Si on fait pas tapis:
        if (tokens < getPlayer(playerPosition)->getCave()) {

            putMoneyIntoGame(playerPosition, tokens);
            m_currentBet = tokens;
            m_accumulatedBetsAndRaises = getPlayer(playerPosition)->getAccumulatedBetsAndRaises();

            m_actions[getPlayer(playerPosition)->getPosition()].push_back(ACTION::RAISE);
            getPlayer(playerPosition)->getActionsCounter()[0]++;
        }
        else{ //Sinon: tapis
            allIn(playerPosition, RAISE);
        }
    }

    void Game::call(int playerPosition) {

        // Le nombre de jetons à ajouter est le cumul moins le nombre de jetons déjà mis par le joueur
        int tokensToAdd = m_accumulatedBetsAndRaises - getPlayer(playerPosition)->getAccumulatedBetsAndRaises();

        // Si on a assez d'argent on suit
        if (getPlayer(playerPosition)->getCave() > tokensToAdd) {
            putMoneyIntoGame(playerPosition,tokensToAdd);
            m_actions[getPlayer(playerPosition)->getPosition()].push_back(ACTION::CALL);
            getPlayer(playerPosition)->getActionsCounter()[1]++;
        }
        else {      // Sinon on fait tapis
            allIn(playerPosition, CALL);
        }
    }

    void Game::check(int playerPosition) {
        m_actions[getPlayer(playerPosition)->getPosition()].push_back(ACTION::CHECK);
        getPlayer(playerPosition)->getActionsCounter()[2]++;
    }

    void Game::fold(int playerPosition) {

        m_actions[getPlayer(playerPosition)->getPosition()].push_back(ACTION::FOLD);

        getPlayer(0)->setTotalBet(getPlayer(0)->getTotalBet() + getPlayer(0)->getAccumulatedBetsAndRaises());
        getPlayer(1)->setTotalBet(getPlayer(1)->getTotalBet() + getPlayer(1)->getAccumulatedBetsAndRaises());

        ai::ArtificialIntelligenceProfiling *ia = static_cast<ai::ArtificialIntelligenceProfiling*>(getPlayer(1));
        ia->fillProfilingData();

        partEnd();
    }

    bool Game::isRoundBeginning() {

        for (int i = 0; i < (int) m_actions.size(); i++) {
            if (getPlayersLastAction(i) != ACTION::NO_ACTION && getPlayersLastAction(i) != ACTION::ALL_IN) {
                return false;
            }
        }

        return true;
    }


    bool Game::isRoundEnd() {

        // Si un joueur n'a pas encore joué
        if (getPlayersLastAction(0) == NO_ACTION
                || getPlayersLastAction(1) == NO_ACTION) {
            return false;
        }

        // Si un joueur s'est couché
        if (getPlayersLastAction(0) == FOLD
                || getPlayersLastAction(1) == FOLD) {
            return true;
        }

        // Si tout le monde a checké
        if (getPlayersLastAction(0) == CHECK
                && getPlayersLastAction(1) == CHECK) {
            return true;
        }

        // Si un joueur a fait tapis et que l'adversaire a joué
        for (unsigned int i = 0; i < m_players.size(); i++) {
            if (getPlayersLastAction(i) == ALL_IN) {

                // Si l'autre a suivi, on cherche si c'est avant ou après le tapis (suivi de grosse blind)
                if (getPlayersLastAction(getOpponentsPosition(i)) == ALL_IN
                        || (getPlayersLastAction(getOpponentsPosition(i)) == CALL && m_actions[i].at(m_actions[i].size()-2) != BIG_BLIND)) {
                    return true;
                }
            }
        }

        // Si la suite de mises/relances est terminée (suivi)
        for (unsigned int i = 0; i < m_players.size(); i++) {
            if (getPlayersLastAction(i) == CALL
                    && (m_actions[i].at(m_actions[i].size()-2) != SMALL_BLIND
                        || (m_actions[i].at(m_actions[i].size()-2) == SMALL_BLIND
                            && getPlayersLastAction(getOpponentsPosition(i)) == CHECK))) {
                return true;
            }
        }

        return false;
    }


    ACTION Game::getPlayersLastAction(int playerPosition) const {
        return m_actions.at(playerPosition).back();
    }

    GAME_STEPS Game::getStep() const {
        return m_currentStep;
    }

    bool Game::nextPlayer() {

        if (m_isGameOver) {
            return false;
        }

        m_currentPlayer = (m_currentPlayer + 1) % m_players.size();

        if (isRoundEnd()) {

            getPlayer(0)->setTotalBet(getPlayer(0)->getTotalBet() + getPlayer(0)->getAccumulatedBetsAndRaises());
            getPlayer(1)->setTotalBet(getPlayer(1)->getTotalBet() + getPlayer(1)->getAccumulatedBetsAndRaises());

            ai::ArtificialIntelligenceProfiling *ia = static_cast<ai::ArtificialIntelligenceProfiling*>(getPlayer(1));

            ia->fillProfilingData();

            // Fin de la partie
            if (getStep() == GAME_STEPS::RIVER || m_isGameOver || isPlayerAllIn()) {
                m_isGameOver = true;
                partEnd();
                return false;
            } else {
                // On incrémente l'étape courante en passant par des entiers
                if (m_currentStep < GAME_STEPS::STEPS_NB) {
                    m_currentStep = static_cast<GAME_STEPS>(static_cast<int>(m_currentStep) + 1);
                }

                newStep(getStep());
            }
        }

        return true;
    }

    void Game::resetActions() {
        for (int i = 0; i < (int) m_actions.size(); i++) {

            // Si le joueur n'a pas fait tapis
            if (std::find(m_actions.at(i).begin(), m_actions.at(i).end(), ACTION::ALL_IN) == m_actions.at(i).end()) {
                m_actions.at(i).clear();
                m_actions.at(i).push_back(ACTION::NO_ACTION);
            }

            getPlayer(i)->setCurrentBet(0);
            getPlayer(i)->setHighestBet(0);
            getPlayer(i)->resetActionsCounter();
            getPlayer(i)->setAccumulatedBetsAndRaises(0);
        }
    }

    void Game::partEnd() {
        m_isGameOver = true;

        std::vector<Player*> joueursRestants;

        for (Player *joueur : m_players) {
            if (isFolded(joueur->getPosition())) {
                joueursRestants.push_back(joueur);
            }
        }

        GAME_RESULT gameResult;

        //Si aucun des deux joueurs ne s'est couché:
        if (!isFolded(0) && !isFolded(1)) {
            if (getTable().size() < 5) { //Dans le cas où il y a eu un tapis et que toutes les cartes ont pas été dévoilées
                tableCardsDistribution(5-(getTable().size()));
                calculateWinningChances();
            }

            GAME_RESULT comparaisonMains = assessor::Assessor::compareHands(this->getTable(), this->getPlayer(0)->getHand(), this->getPlayer(1)->getHand());

            if (comparaisonMains == WON) {

                getPlayer(0)->addTokens(getPot());
                gameResult = WON;
            } else if (comparaisonMains == LOOSE) {
                getPlayer(1)->addTokens(getPot());
                gameResult = LOOSE;

            } else {
                gameResult = EQUALITY;

                getPlayer(0)->addTokens(getPot() / 2);
                getPlayer(1)->addTokens(getPot() / 2);
            }
        } else { //Un joueur s'est couché
            if (isFolded(0)) {
                gameResult = LOOSE;
                getPlayer(1)->addTokens(getPot());
            } else {
                gameResult = WON;
                getPlayer(0)->addTokens(getPot());
            }
       }

        m_gameResult = gameResult;

        // Si c'est un joueur humain, on calcule ses chances de gain
        if (getPlayer(0)->isHumain()) {

            int nbThreads = 4;
            double nbTestsParThread = static_cast<double>(TESTS_NUMBER) / nbThreads;
            std::vector<ai::WinningChancesEstimator*> estimators;

            for (int i = 0; i < 4; i++) {
                ai::WinningChancesEstimator *estimator = new ai::WinningChancesEstimator(this, getPlayer(0), nbTestsParThread);
                estimators.push_back(estimator);
                estimator->start();
            }

            double estimatesSum = 0;

            for (unsigned int i = 0; i < estimators.size(); i++) {
                estimators[i]->wait();
                estimatesSum += estimators[i]->getEstimateResult();

                delete estimators[i];
            }

            getPlayer(0)->setWinningChances(100 * (estimatesSum / estimators.size()));
            estimators.clear();
        }

        ai::ArtificialIntelligenceProfiling *ai = static_cast<ai::ArtificialIntelligenceProfiling*>(getPlayer(1));
        ai->writeGameResult();
    }

    std::vector<game::Card> Game::getTable() const {
        return m_table;
    }

    std::vector<ACTION>  Game::getPlayersActions(int playerPosition) const {
        return m_actions.at(playerPosition);
    }

    int Game::getCurrentBet() const {
        return m_currentBet;
    }

    int Game::getAccumulatedBetsAndRaises() const {
        return m_accumulatedBetsAndRaises;
    }

    void Game::newHand() {

        setPot(0);
        m_table.clear();
        resetActions();

        for (int i = 0; i < (int) m_players.size(); i++) {
            getPlayer(i)->clearHand();
            getPlayer(i)->setTotalBet(0);
        }

        m_deck = newDeck();

        getPlayer(m_dealer)->changeDealer();
        m_dealer = (m_dealer + 1) % m_players.size();
        getPlayer(m_dealer)->changeDealer();

        m_currentStep = GAME_STEPS::PREFLOP;
    }


    bool Game::canCheck(int playerPosition) {

        int opponentsPosition = getOpponentsPosition(playerPosition);

        //On peut checker quand le joueur précédent a checké ou suivi.

        if (getPlayersLastAction(opponentsPosition) == ACTION::ALL_IN
                && isRoundBeginning()) {
            return true;
        }

        //Si l'action de l'autre joueur est miser, relancer ou grosse blinde, on retourne false
        if (getPlayersLastAction(opponentsPosition) == ACTION::BET
                || getPlayersLastAction(opponentsPosition) == ACTION::RAISE
                || getPlayersLastAction(opponentsPosition) == ACTION::BIG_BLIND
                || getPlayersLastAction(opponentsPosition) == ACTION::ALL_IN) {
            return false;
        }

        return true;
    }

    bool Game::canBet(int playerPosition, int tokens) {

        int opponentsPosition = getOpponentsPosition(playerPosition);

        //On peut miser quand le joueur précédent a checké
        if (getPlayersLastAction(opponentsPosition) == ACTION::BET
                || getPlayersLastAction(opponentsPosition) == ACTION::RAISE
                || getPlayersLastAction(opponentsPosition) == ACTION::BIG_BLIND
                || getPlayersLastAction(opponentsPosition)==ACTION::ALL_IN) {

            if (getPlayersLastAction(opponentsPosition) == ACTION::CALL
                    && getPlayersLastAction(playerPosition) == ACTION::BIG_BLIND) {
                return true;
            }
            return false;
        }

        if (tokens <= 0 || getPlayer(playerPosition)->getCave() < tokens) {
            return false;
        }

        return true;
    }


    bool Game::canRaise(int playerPosition, int tokens){

        int opponentsPosition = getOpponentsPosition(playerPosition);

        //On peut pas relancer quand le joueur précédent a checké, n'as pas agit, a fait tapis ou a suivi.
        if (getPlayersLastAction(opponentsPosition) == ACTION::CHECK
                || getPlayersLastAction(opponentsPosition) == ACTION::NO_ACTION
                || getPlayersLastAction(opponentsPosition) == ACTION::ALL_IN
                || getPlayersLastAction(opponentsPosition) == ACTION::CALL) {
            return false;
        }

        if (tokens > 0 && getPlayer(playerPosition)->getCave() >= tokens
                && tokens >= m_currentBet * 2) {
            return true;
        }

        return false;
    }

    bool Game::canCall(int playerPosition) {

        int opponentsPosition = getOpponentsPosition(playerPosition);

        //On peut suivre quand le joueur précédent a misé, relancé, grosse blind ou fait tapis
        if (getPlayersLastAction(opponentsPosition) == ACTION::CHECK
                || getPlayersLastAction(opponentsPosition) == ACTION::SMALL_BLIND
                || getPlayersLastAction(opponentsPosition) == ACTION::CALL
                || getPlayersLastAction(opponentsPosition) == ACTION::NO_ACTION) {
            return false;
        }

        return true;
    }

    bool Game::isFolded(int playerPosition) const {
        return (getPlayersLastAction(playerPosition) == ACTION::FOLD);
    }

    void Game::executeAction(int playerPosition, Action action) {
        int raiseTokens;

        switch (action.getAction()) {
            case ACTION::CHECK:
                if (canCheck(playerPosition)) {
                    check(playerPosition);
                }
                break;

            case ACTION::BET:
                if (canBet(playerPosition, action.getTokens())) {
                    bet(playerPosition, action.getTokens());
                }
                break;

            case ACTION::CALL:
                if (canCall(playerPosition)) {
                    call(playerPosition);
                }
                break;

            case ACTION::RAISE:

                raiseTokens = (action.getTokens() < 2 * getCurrentBet()) ? 2 * getCurrentBet() : action.getTokens();

                if (canRaise(playerPosition, raiseTokens)) {
                    raise(playerPosition, raiseTokens);
                }
                break;

            case ACTION::FOLD:
                fold(playerPosition);
                break;

            case ACTION::ALL_IN:
                allIn(playerPosition, BET);
                break;

            default:
                break;
        }
    }

    void Game::affectsCards(std::vector<int> cardsIds) {

        int position = 0;

        for (int i = 0; i < 9; i++) {

            if (cardsIds.at(i) != -1) {
                position = 0;
                for (game::Card carte : getDeck()) {
                    if (carte.getId() == cardsIds.at(i)) {
                        if (i < 2) {
                            m_players.at(0)->addCard(m_deck.at(position));
                            m_deck.erase(m_deck.begin() + position);
                            position--;
                        } else if (i < 4) {
                            m_players.at(1)->addCard(m_deck.at(position));
                            m_deck.erase(m_deck.begin() + position);
                            position--;
                        } else {
                            m_tableTmp.push_back(m_deck.at(position));
                            m_deck.erase(m_deck.begin() + position);
                            position--;
                        }
                    }

                    position++;
                }
            }
        }
    }

    void Game::launchPart()
    {
        while (nextPlayer()) {
            if (isRoundBeginning()) {
                gui::Logger::getInstance()->addLogs("--- Nouvelle Etape ---");
            }

            Action a = m_players.at(m_currentPlayer)->play();
            executeAction(m_currentPlayer, a);
        }
    }

    bool Game::isPlayerAllIn() {
        return getPlayersLastAction(0)== ACTION::ALL_IN || getPlayersLastAction(1) == ACTION::ALL_IN;
    }
}
