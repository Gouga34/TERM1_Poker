#include "../../include/IA/ArtificialIntelligence.h"
#include "../../include/IA/WinningChancesEstimator.h"
#include "../../include/Interface/Logger.h"
#include <sstream> 
#include <string>

ArtificialIntelligence::ArtificialIntelligence(bool isDealer, int tokens, int position)
    : Joueur(isDealer, tokens, position) {
    m_resolver = new Resolver(this);

    changeRandomlyCalibration();
}

ArtificialIntelligence::ArtificialIntelligence(const ArtificialIntelligence& player)
    : Joueur(player) {
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
        WinningChancesEstimator *estimateur = new WinningChancesEstimator(getJeu(), getJeu()->getJoueur(getPosition()), nbTestsByThread);
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

    setChancesGain(100 * (estimationsSum / m_winningChancesEstimators.size()));
    m_winningChancesEstimators.clear();
}

Action ArtificialIntelligence::play() {
    QString actions[] = {"mise", "relance", "suit", "check", "se couche", "rien", "fait tapis"};

    Action action = m_resolver->calculateAction();

    QString log = "IA " + QString::number(position) + " " + actions[action.getAction()];
    if (action.getAction() == MISER || action.getAction() == RELANCER) {
        log += " " + QString::number(action.getMontant());
    }

    Logger::getInstance()->ajoutLogs(log);

    return action;
}
