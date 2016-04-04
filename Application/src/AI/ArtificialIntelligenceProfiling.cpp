/*========================================================================
Nom: IntelligenceArtificielleProfilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015            Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe IntelligenceArtificielleProfilage.
=========================================================================*/


#include "../../include/AI/ArtificialIntelligenceProfiling.h"
#include "../../include/Interface/Logger.h"
#include "../../include/MathematicalFormula/MathematicalFormula.h"
#include <QStringList>
#include <QTextStream>

namespace ai {

    ArtificialIntelligenceProfiling::ArtificialIntelligenceProfiling(bool isDealer, int tokens, int position)
        : ArtificialIntelligence(isDealer, tokens, position), m_gamePhase(PHASE_PROFILAGE), m_profiling(0), m_scenario(0)
    {

    }

    ArtificialIntelligenceProfiling::ArtificialIntelligenceProfiling(const ArtificialIntelligence& player)
        : ArtificialIntelligence(player), m_profiling(0), m_scenario(0)
    {

    }

    ArtificialIntelligenceProfiling::~ArtificialIntelligenceProfiling()
    {
        if (m_profiling != 0) {
            delete m_profiling;
        }

        if (m_scenario != 0) {
            delete m_scenario;
        }
    }

    profiling::TestScenarios ArtificialIntelligenceProfiling::getScenario() const{
        return *m_scenario;
    }

    profiling::Profiling* ArtificialIntelligenceProfiling::getProfiling() const {
        return m_profiling;
    }

    void ArtificialIntelligenceProfiling::setProfiledPlayerNickname(std::string nickname) {
        m_playerProfile.setPseudo(nickname);

        if (m_profiling) {
            delete m_profiling;
        }
        m_profiling = new profiling::Profiling(m_resolver->getCalibration(), &m_playerProfile);
        m_profiling->m_currentProfilingPhase.newPhase(jeu->getOptions().nombrePartiesProfilage);

        profiling::Profile searchedCalibration;
        if (!jeu->getJoueur(0)->isHumain()) {
            ArtificialIntelligence *profiledAI = static_cast<ArtificialIntelligence*>(jeu->getJoueur(0));
            searchedCalibration = *(profiledAI->getCalibration());
        }

        if (m_scenario) {
            delete m_scenario;
        }
        m_scenario = new profiling::TestScenarios(&m_playerProfile, m_resolver->getCalibration(), searchedCalibration);
    }

    void ArtificialIntelligenceProfiling::fillProfilingData() {

        int totalNumberOfActions = 0;

        for (int i = 0; i < 3; i++) {
            totalNumberOfActions += jeu->getJoueur(0)->getCompteurActions()[i];
        }

        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].adversaryWinningProbabilities = jeu->getJoueur(0)->getChancesGain();

        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate += jeu->getJoueur(0)->getCompteurActions()[0];
        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate += jeu->getJoueur(0)->getCompteurActions()[1];
        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate += jeu->getJoueur(0)->getCompteurActions()[2];

        if (jeu->getJoueur(0)->getMisePlusHaute() > m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet) {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet = jeu->getJoueur(0)->getMisePlusHaute();
        }

        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet = jeu->getJoueur(0)->getMiseTotale();

        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].isFolded = jeu->estCouche(0);

        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].numberOfActions += totalNumberOfActions;
    }


    void ArtificialIntelligenceProfiling::calculatePlayerGlobalProfile() {

        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].adversaryWinningProbabilities = jeu->getJoueur(0)->getChancesGain();

        double result;

        result = profiling::CalculateProfilingData::taux(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate,m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].numberOfActions);
        if (result == -1) {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate = 0;
        }
        else {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate = result;
        }

        result = profiling::CalculateProfilingData::taux(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate,m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].numberOfActions);
        if (result == -1) {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate = 0;
        }
        else {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate = result;
        }

        result = profiling::CalculateProfilingData::taux(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate,m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].numberOfActions);
        if (result == -1) {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate = 0;
        }
        else {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate = result;
        }

        result = profiling::CalculateProfilingData::taux(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet,getCaveDeDepart());
        if (result == -1) {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet = 0;
        }
        else {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet = result;
        }

        result = profiling::CalculateProfilingData::taux(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet,getCaveDeDepart());
        if (result == -1) {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet = 0;
        }
        else {
            m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet = result;
        }

        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].aggressivenessRate = profiling::CalculateProfilingData::aggressiveness(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet,m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate,m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet);
        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].rationalityRate = profiling::CalculateProfilingData::rationality(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].adversaryWinningProbabilities,m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet);
        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].passivityRate = profiling::CalculateProfilingData::passivity( m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate, m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate);
        m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].bluffRate = profiling::CalculateProfilingData::bluff(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].rationalityRate);


        m_profiling->m_playerProfile->setAggressiveness(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].aggressivenessRate);
        m_profiling->m_playerProfile->setRationality(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].rationalityRate);
        m_profiling->m_playerProfile->setPassivity(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].passivityRate);
        m_profiling->m_playerProfile->setBluff(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].bluffRate);

        RESULTAT_PARTIE gameResult = getJeu()->getResultatPartie();
        if (gameResult == GAGNE) {
            m_profiling->m_numberTokensWonProfilingAI = getCave()-getCaveDeDepart();
            gameResult = PERDU;
        }
        else if (gameResult == PERDU) {
            gameResult = GAGNE;
            m_profiling->m_numberTokensWonProfilingAI = getCave()-getCaveDeDepart();
        }
        else {
            m_profiling->m_numberTokensWonProfilingAI = 0;
        }

        m_profiling->m_roughPlay = m_resolver->isAggressive();
        m_profiling->m_testScenario = (m_gamePhase == PHASE_PROFILAGE);
        m_profiling->m_gameResultProfilingAIviewpoint = gameResult;

        if (m_gamePhase == PHASE_PROFILAGE) {
            m_profiling->m_currentProfilingPhase.finishedPart(m_profiling->m_numberTokensWonProfilingAI);
        }
        else {
            m_profiling->m_currentGamePhase.finishedPart(m_profiling->m_numberTokensWonProfilingAI);
        }

        m_profiling->save();
    }

    void ArtificialIntelligenceProfiling::writeScenariosTestsResults() {
        m_scenario->setWinningChances(m_profiling->m_stepParts[ETAPE_JEU::NB_ETAPES].adversaryWinningProbabilities);
        m_scenario->saveGame();
    }

    void ArtificialIntelligenceProfiling::writeGameResult() {
        calculatePlayerGlobalProfile();

        // Si on profile le joueur
        if (m_gamePhase == PHASE_PROFILAGE) {
            writeScenariosTestsResults();
        }
    }

    PHASE_JEU_IA ArtificialIntelligenceProfiling::getNextGameType() {

        if (m_profiling->m_currentProfilingPhase.getNbPartsDone() < m_profiling->m_currentProfilingPhase.getNbMinParts()) {
            return PHASE_PROFILAGE;
        }
        else if (m_profiling->m_currentGamePhase.getNbPartsDone() < m_profiling->m_currentGamePhase.getNbMinParts()) {
            return PHASE_GAINS;
        }
        else {
            if (m_gamePhase == PHASE_PROFILAGE) {
                m_profiling->m_currentGamePhase.newPhase(jeu->getOptions().nombrePartiesGains);
                return PHASE_GAINS;
            }
            else {
                double gainParPartieJeu = static_cast<double>(m_profiling->m_currentGamePhase.getGains()) / m_profiling->m_currentGamePhase.getNbPartsDone();
                double gainParPartieProfilage = static_cast<double>(m_profiling->m_currentProfilingPhase.getGains()) / m_profiling->m_currentProfilingPhase.getNbPartsDone();

                if (m_profiling->m_currentGamePhase.getGains() < 0 || gainParPartieJeu < gainParPartieProfilage) {
                    m_profiling->m_currentProfilingPhase.newPhase(jeu->getOptions().nombrePartiesReprofilage);
                    return PHASE_PROFILAGE;
                }
                else {
                    return PHASE_GAINS;
                }
            }
        }
    }

    void ArtificialIntelligenceProfiling::determineGameType() {

        // Si on recherche calibrage optimal
        if (!jeu->getOptions().profilage) {
            m_gamePhase = PHASE_GAINS;
        }
        else {
            m_gamePhase = getNextGameType();
            m_resolver->setRoughtGame(false);

            if (m_gamePhase == PHASE_PROFILAGE) {
                setProfilingCalibration();
            }
            else {
                if (UTILISATION_DELTA_AGRESSIVITE && chancesGain >= 50 && chancesGain <= 70) {
                    // Aléatoirement, on augmente l'agressivité
                    m_resolver->setRoughtGame((rand() % DELTA_AGRESSIVITE) == 0);
                }

                setCalibrationToPlay();
            }

            Logger::getInstance()->ajoutLogs("Calibrage IA profilage: agressivité: " + QString::number(m_resolver->getCalibration()->getAggressiveness())
                                             + " rationalité: " + QString::number(m_resolver->getCalibration()->getRationality()));
        }

        getProfiling()->reset();
    }

    void ArtificialIntelligenceProfiling::setProfilingCalibration() {

        if (jeu->getOptions().calibrageIaQuiProfileFixe) {
            return;
        }

        //On tire aléatoirement un nouveau taux d'agressivite:
        int agressivite = rand()%100+1;

        m_resolver->getCalibration()->setAggressiveness(agressivite);
        m_resolver->getCalibration()->setRationality(RATIONALITE_IA_PROFILAGE);
    }

    void ArtificialIntelligenceProfiling::setCalibrationToPlay() {

        if (jeu->getOptions().calibrageIaQuiProfileFixe) {
            return;
        }

        /////// Lecture du calibrage pour gagner //////////

        QFile file(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + "jeu_IA.csv");
        if (!file.open(QIODevice::ReadOnly)) {
            std::cerr << "Erreur lors de l'ouverture du fichier jeu_IA.csv" << std::endl;
            return;
        }

        QString line = file.readLine();
        line = file.readLine();

        while (!line.isEmpty()) {
            QStringList list = line.split(",");

            double agressiviteMin = list.at(0).split("-").at(0).toDouble();
            double agressiviteMax = list.at(0).split("-").at(1).toDouble();

            if (m_scenario->getGlobalDeductedProfile().getAggressiveness() >= agressiviteMin
                    && m_scenario->getGlobalDeductedProfile().getAggressiveness() <= agressiviteMax) {
                double rationaliteMin = list.at(1).split("-").at(0).toDouble();
                double rationaliteMax = list.at(1).split("-").at(1).toDouble();

                if (m_scenario->getGlobalDeductedProfile().getRationality() >= rationaliteMin
                        && m_scenario->getGlobalDeductedProfile().getRationality() <= rationaliteMax) {

                    profiling::Profile newProfile;

                    if (list.at(2).split("-").size() == 1) {
                        newProfile.setAggressiveness(list.at(2).toDouble());
                    }
                    else {
                        newProfile.setAggressiveness(mathematicalformula::MathematicalFormula::calculateProportionalValue(agressiviteMin, m_scenario->getGlobalDeductedProfile().getAggressiveness(), agressiviteMax,
                                                                                 list.at(2).split("-").at(0).toDouble(), list.at(2).split("-").at(1).toDouble()));
                    }

                    if (list.at(3).split("-").size() == 1) {
                        newProfile.setRationality(list.at(3).toDouble());
                    }
                    else {
                        newProfile.setRationality(mathematicalformula::MathematicalFormula::calculateProportionalValue(rationaliteMin, m_scenario->getGlobalDeductedProfile().getRationality(), rationaliteMax,
                                                                                 list.at(3).split("-").at(0).toDouble(), list.at(3).split("-").at(1).toDouble()));
                    }

                    setCalibration(newProfile);

                    file.close();
                    return;
                }
            }

            line = file.readLine();
        }

        file.close();
    }

    void ArtificialIntelligenceProfiling::writeEarningsAnalysis() {

        if (jeu->getOptions().analyseGainsParties) {

            profiling::Profile searchedCalibration;

            if (!jeu->getJoueur(0)->isHumain()) {
                ArtificialIntelligence *iaProfilee = static_cast<ArtificialIntelligence*>(jeu->getJoueur(0));
                searchedCalibration = *(iaProfilee->getCalibration());
            }

            /////////// On cherche le palier ///////////

            QFile levelsFile(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + "jeu_IA.csv");
            if (!levelsFile.open(QIODevice::ReadOnly)) {
                std::cerr << "Erreur lors de l'ouverture du fichier jeu_IA.csv" << std::endl;
                return;
            }

            double agressivenessMin, agressivenessMax, rationalityMin, rationalityMax;

            QString line = levelsFile.readLine();
            line = levelsFile.readLine();

            bool levelFound = false;

            while (!line.isEmpty() && !levelFound) {
                QStringList list = line.split(",");

                agressivenessMin = list.at(0).split("-").at(0).toDouble();
                agressivenessMax = list.at(0).split("-").at(1).toDouble();

                if (searchedCalibration.getAggressiveness() >= agressivenessMin
                        && searchedCalibration.getAggressiveness() <= agressivenessMax) {
                    rationalityMin = list.at(1).split("-").at(0).toDouble();
                    rationalityMax = list.at(1).split("-").at(1).toDouble();

                    if (searchedCalibration.getRationality() >= rationalityMin
                            && searchedCalibration.getRationality() <= rationalityMax) {
                        levelFound = true;
                    }
                }

                line = levelsFile.readLine();
            }

            levelsFile.close();


            if (!levelFound) {
                std::cerr << "Problème : le palier n'a pas été trouvé dans le fichier !" << std::endl;
                return;
            }


            // On ouvre le fichier du palier correspondant
            QString filename = QString::number(agressivenessMin) + "-" + QString::number(agressivenessMax)
                                   + "_" + QString::number(rationalityMin) + "-" + QString::number(rationalityMax) + ".csv";
            QFile file(QString::fromStdString(DOSSIER_ANALYSE_GAINS) + filename);
            if (!file.open(QIODevice::Append | QIODevice::Text)) {
                std::cerr << "Erreur lors de l'ouverture du fichier d'analyse de gains" << std::endl;
                return;
            }

            QTextStream out(&file);

            if (file.size() == 0) {
                out << "Calibrage IA profilee,AG deduite globale,Ra deduite Globale,Taux similarite,NbParties gagnees scenarios,"
                    << "Argent gagne scenarios,Nb parties gagnees apres scenarios,Argent gagne apres scenarios" << endl;
            }



            double rateWonPartsProfiling = profiling::CalculateProfilingData::taux(m_profiling->m_numberProfilingGamesWon, m_profiling->m_numberProfilingGames);
            double rateWonPartsGame = profiling::CalculateProfilingData::taux(m_profiling->m_numberGameGamesWon, m_profiling->m_numberGamesGame);

            double rateEarningsProfiling = 0;
            double rateEarningsGame=0;

            if (m_profiling->m_numberProfilingGames != 0) {
                rateEarningsProfiling = m_profiling->m_profilingEarnings / m_profiling->m_numberProfilingGames;
            }

            if (m_profiling->m_numberGamesGame != 0) {
                rateEarningsGame=m_profiling->m_gameEarnings /  m_profiling->m_numberGamesGame;
            }

            out << searchedCalibration.getAggressiveness() << "-" << searchedCalibration.getRationality()
                << "," << m_scenario->getGlobalDeductedProfile().getAggressiveness()
                << "," << m_scenario->getGlobalDeductedProfile().getRationality()
                << "," << m_scenario->getSimilarityDegree()
                << "," << rateWonPartsProfiling
                << "," << rateEarningsProfiling
                << "," << rateWonPartsGame
                << "," << rateEarningsGame << endl;

            file.close();
        }
    }
}
