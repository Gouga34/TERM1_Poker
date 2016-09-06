#include "../../include/AI/ArtificialIntelligence.h"
#include "../../include/AI/WinningChancesEstimator.h"
#include "../../include/Gui/Logger.h"

#include <sstream> 
#include <string>

namespace ai {

    ArtificialIntelligence::ArtificialIntelligence(bool isDealer, int tokens, int position)
        : Player(isDealer, tokens, position) {
        m_resolver = new Resolver(this);

        changeRandomlyCalibration();
    }

    ArtificialIntelligence::ArtificialIntelligence(const ArtificialIntelligence& player)
        : Player(player) {
        m_resolver = new Resolver(this);
    }

    ArtificialIntelligence::~ArtificialIntelligence() {
        if (m_resolver != 0) {
            delete m_resolver;
        }
    }

    bool ArtificialIntelligence::isHumain() const {
        return false;
    }

    profiling::Profile* ArtificialIntelligence::getCalibration() {
        return m_resolver->getCalibration();
    }

    void ArtificialIntelligence::setCalibration(profiling::Profile profile) {
        m_resolver->setCalibration(profile);
    }

    void ArtificialIntelligence::changeRandomlyCalibration()
    {
        int agressivite=rand()%100+1;
        int rationalite=rand()%100+1;

        profiling::Profile p;
        p.setAggressiveness(agressivite);
        p.setRationality(rationalite);
        setCalibration(p);
    }

    void ArtificialIntelligence::launchWinningChancesEstimator(int testsNumber, int threadsNumber)
    {
        m_winningChancesEstimators.clear();
        double nbTestsByThread = static_cast<double>(testsNumber) / threadsNumber;

        for (int i = 0; i < threadsNumber; i++) {
            WinningChancesEstimator *estimateur = new WinningChancesEstimator(getGame(), getGame()->getPlayer(getPosition()), nbTestsByThread);
            m_winningChancesEstimators.push_back(estimateur);
            estimateur->start();
        }
    }

    void ArtificialIntelligence::waitWinningChancesEstimatorResult()
    {
        double estimationsSum = 0;

        for (unsigned int i = 0; i < m_winningChancesEstimators.size(); i++) {
            m_winningChancesEstimators[i]->wait();
            estimationsSum += m_winningChancesEstimators[i]->getEstimateResult();

            delete m_winningChancesEstimators[i];
        }

        setWinningChances(100 * (estimationsSum / m_winningChancesEstimators.size()));
        m_winningChancesEstimators.clear();
    }

    game::Action ArtificialIntelligence::play() {
        std::vector<QString> actions { "mise", "relance", "suit", "check", "se couche", "rien", "fait tapis" };

        game::Action action = m_resolver->calculateAction();

        QString log = "IA " + QString::number(m_position) + " " + actions[action.getAction()];
        if (action.getAction() == BET || action.getAction() == RAISE) {
            log += " " + QString::number(action.getTokens());
        }

        gui::Logger::getInstance()->addLogs(log);

        return action;
    }
}
