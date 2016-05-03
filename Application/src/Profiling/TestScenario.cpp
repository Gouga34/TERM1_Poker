#include "../../include/Constants.h"
#include "../../include/Profiling/TestScenario.h"
#include "../../include/Profiling/CalculateProfilingData.h"
#include "../../include/Gui/Logger.h"


#include <QString>
#include <QIODevice>
#include <QJsonArray>
#include <QByteArray>
#include <QStringList>

#include <cstdlib>
#include <QTextStream>

namespace profiling {

    TestScenarios::TestScenarios(Profile *playerProfile, Profile *aiCalibration, Profile opponentProfile) {

        m_wantedCalibration = opponentProfile;
        m_aiCurrentCalibration = aiCalibration;
        m_realActionPlayer = playerProfile;
    }

    TestScenarios::~TestScenarios() {

    }

    void TestScenarios::setAiCurrentCalibration(Profile *aiCurrentCalibration) {
        m_aiCurrentCalibration = aiCurrentCalibration;
    }

    void TestScenarios::setIsPlayerFolded(bool isPlayerFolded) {
        m_isPlayerFolded = isPlayerFolded;
    }

    bool TestScenarios::getIsPlayerFolded() const {
        return m_isPlayerFolded;
    }

    void TestScenarios::setDistance(double distance) {
        m_distance = distance;
    }

    double TestScenarios::getDistance() const{
        return m_distance;
    }

    Profile* TestScenarios::getAiCurrentCalibration() const {
        return m_aiCurrentCalibration;
    }

    void TestScenarios::setExpectedActionPlayer(Profile expectedActionPlayer) {
        m_expectedActionPlayer = expectedActionPlayer;
    }

    Profile TestScenarios::getExpectedActionPlayer() const {
        return m_expectedActionPlayer;
    }

    Profile TestScenarios::getGlobalDeductedProfile() const {
        return m_globalDeductedProfile;
    }

    double TestScenarios::getSimilarityDegree() const {
        return m_similarityDegree;
    }

    void TestScenarios::setRealActionPlayer(Profile *realActionPlayer) {
        m_realActionPlayer = realActionPlayer;
    }

    Profile* TestScenarios::getRealActionPlayer() const {
        return m_realActionPlayer;
    }

    void TestScenarios::setWinningChances(double winningChances) {
        m_winningChances = winningChances;
    }

    double TestScenarios::getWinningChances() const {
        return m_winningChances;
    }


    void TestScenarios::saveGame() {

        //On ouvre le fichier dans lequel on sauvegardera les données.
        QString filename = QString::fromStdString(m_realActionPlayer->getPseudo()) + "_scenarios_tests.csv";
        QFile file(QString::fromStdString(STATIC_PROFILING_FOLDER) + filename);

        int nbGames = 0;

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            std::cerr << "Erreur lors de l'ouverture du fichier " << std::endl;
        }
        else {
            QTextStream out(&file);

            //Si le fichiers est vide, on fait l'initialisation:
            calculateExpectedActionPlayer(file);
            calculateDistance();

            //calcul de la rationalité déduite globale et l'agressivité déduite globale:

            if (file.size() == 0) {
                m_globalDeductedProfile.setAggressiveness(m_expectedActionPlayer.getAggressiveness());
                m_globalDeductedProfile.setRationality(m_expectedActionPlayer.getRationality());
            } else {
                file.seek(0);
                QString line = file.readLine();

                QStringList list;
                double previousGlobalRationality = 0;
                double previousGlobalAggressiveness = 0;

                int referenceData = 0;
                while (!line.isEmpty()) {
                    list = line.split(",");
                    if (referenceData == 1) {
                        previousGlobalRationality = list.at(GLOBAL_DEDUCED_RATIONALITY).toDouble();
                        previousGlobalAggressiveness = list.at(GLOBAL_DEDUCED_AGGRESSIVENESS).toDouble();
                    }

                    if (list.at(0) == "agressivite IA") {
                        referenceData = 1;
                    }
                    line = file.readLine();
                }

                file.seek(0);
                line = file.readLine();
                //On récupère le nombre de parties :

                while (!line.isEmpty()) {
                    ++nbGames;
                    line = file.readLine();
                }

                m_globalDeductedProfile.setAggressiveness((m_expectedActionPlayer.getAggressiveness() + (previousGlobalAggressiveness * (nbGames - 1))) / nbGames);
                m_globalDeductedProfile.setRationality((m_expectedActionPlayer.getRationality() + (previousGlobalRationality * (nbGames - 1))) / nbGames);
            }

            if (file.size() == 0) {
              out << "agressivite IA,Chances de gain IA profilee,Agressivite attendue,Rationalite attendue,Agressivite reelle,Rationalite Reelle,distance moyenne,Agressivite deduite globale,Rationalite deduite globale,taux similarite" << endl;
            }

            double aggressivenessSimilarity = 100 - abs(m_globalDeductedProfile.getAggressiveness() - m_wantedCalibration.getAggressiveness());
            double rationalitySimilarity = 100 - abs(m_globalDeductedProfile.getRationality() - m_wantedCalibration.getRationality());

            m_similarityDegree = (aggressivenessSimilarity + rationalitySimilarity) / 2;

             //On écrit l'agressivité de l'IA qui profile et ses chances de gain.
            out << getAiCurrentCalibration()->getAggressiveness() << ","
                << getWinningChances() << ","
                << m_expectedActionPlayer.getAggressiveness() << ","
                << m_expectedActionPlayer.getRationality() << ","
                << m_realActionPlayer->getAggressiveness() << ","
                << m_realActionPlayer->getRationality() << ","
                << getDistance() << ","
                << m_globalDeductedProfile.getAggressiveness() << ","
                << m_globalDeductedProfile.getRationality() << ","
                << m_similarityDegree << endl;

            file.close();
        }
    }

    void TestScenarios::calculateExpectedActionPlayer(QFile& profileFile) {

        double aggressivenessExpectedIntervalLower = 0;
        double aggressivenessExpectedIntervalSuperior = 0;

        double aiWinningChancesIntervalLower = 0;
        double aiWinningChancesIntervalSuperior = 0;

        m_expectedActionPlayer.setAggressiveness(-1);
        m_expectedActionPlayer.setRationality(-1);

        //Si le fichier n'est pas vide, on regarde dedans si on a pas déjà une valeur pour les chances de gains et l'agressivité actuelle
        oldNearestStatus(profileFile);

        //Si on n'a pas d'agressivité attendue et de rationalité attendue:
        if (m_expectedActionPlayer.getAggressiveness() == -1 || m_expectedActionPlayer.getRationality() == -1) {

            //On regarde dans le fichier dans lequel se trouvent les données de base.
            QString filename = QString::fromStdString("scenarios_tests_basiques.csv");
            QFile file(QString::fromStdString(STATIC_PROFILING_FOLDER) + filename);

            if (!file.open(QIODevice::ReadWrite)) {
                std::cerr << "Erreur lors de l'ouverture du fichier " << std::endl;
            }
            else {
                QString line = file.readLine();
                QStringList list;
                int calibrationLine = 0;
                int winningChancesLine = 0;
                int expectedAggressivenessLine = 0;
                int cpt = 0;

                int calibration = 1;

                while (!line.isEmpty()) {
                     list = line.split(",");

                     //Si on a comme mot clef agressivité, ça veut dire qu'on a déjà traité les palliers de calibrage
                     //et qu'on en est aux palliers d'agressivité
                    if (list.at(0) == "Agressivite") {
                        calibration = 0;
                    }
                    else if (list.at(0) == "agressivite IA") {
                        calibration = 2;
                    }

                    ++cpt;

                    //Si on cherche l'intervalle du calibrage
                    if (calibration == 1) {
                        //On regarde si on est dans l'intervalle de l'agressivité de l'IA
                        if (list.at(0).toDouble() <= m_aiCurrentCalibration->getAggressiveness()
                                && m_aiCurrentCalibration->getAggressiveness() <= list.at(1).toDouble()) {
                            calibrationLine = cpt;
                        }

                        // Les paliers pour les chances de gain sont les même
                        //On regarde si on est dans l'intervalle des chances de gains de l'IA
                        if (list.at(0).toDouble() <= getWinningChances()
                                && getWinningChances() <= list.at(1).toDouble()) {
                            aiWinningChancesIntervalLower = list.at(0).toDouble();
                            aiWinningChancesIntervalSuperior = list.at(1).toDouble();
                            winningChancesLine = cpt;
                        }
                    }

                    //Si l'intervalle d'agressivité de l'IA correspond au cptCalibrage
                    //et que les chances de gain de l'IA sont dans le bon intervalle,
                    if (calibration == 2
                       && list.at(0).toInt() == calibrationLine
                       && list.at(1).toInt() == winningChancesLine) {

                        expectedAggressivenessLine = list.at(2).toInt();
                        break;
                        break;
                    }

                    line = file.readLine();
                }

                //On récupère l'intervalle d'agressivité attendue du joueur :
                cpt = 0;
                file.seek(0);
                line = file.readLine();
                while (!line.isEmpty()) {
                    list = line.split(",");
                    ++cpt;
                    if (cpt == expectedAggressivenessLine) {
                        aggressivenessExpectedIntervalLower = list.at(0).toDouble();
                        aggressivenessExpectedIntervalSuperior = list.at(1).toDouble();
                        break;
                        break;
                    }
                    line = file.readLine();
                }

                file.seek(0);
            }

            //On calcule le taux d'agressivité attendu :
           double expectedAggressiveness = ((getWinningChances() - aiWinningChancesIntervalLower)
                                          * (   (aggressivenessExpectedIntervalSuperior - aggressivenessExpectedIntervalLower)
                                              / (aiWinningChancesIntervalSuperior - aiWinningChancesIntervalLower)))
                                        + aggressivenessExpectedIntervalLower;

           m_expectedActionPlayer.setAggressiveness(expectedAggressiveness);

            //Le taux de rationalité attendu est à 50% par défaut :
            m_expectedActionPlayer.setRationality(PROFILING_IA_RATIONALITY);
        }
    }

    void TestScenarios::calculateDistance() {
        //On récupère les distances entre chaque paramètre (agressivité, rationalité, bluff, passivité) :
        double aggressivenessDistance = abs(m_expectedActionPlayer.getAggressiveness() - m_realActionPlayer->getAggressiveness());
        double rationalityDistance = abs(m_expectedActionPlayer.getRationality() - m_realActionPlayer->getRationality());

        //On calcule une distance moyenne:
        m_distance = (aggressivenessDistance + rationalityDistance) / 2;
    }

    void TestScenarios::oldNearestStatus(QFile& profileFile) {
        ///////////////////////////TODO///////////////////////////////
        ///Faire le même test pour les chances de gain du joueur   ///
        ///quand elles seront implémentées.                        ///
        //////////////////////////////////////////////////////////////
        if (profileFile.size() != 0) {
            profileFile.seek(0);
            QString line = profileFile.readLine();
            line = profileFile.readLine(); //on lit une deuxième fois pour sauter la première ligne
            QStringList list;
            int differencePreviousAgressiveness = 100;
            int differencePreviousAIWinningChances = 100;

            //On va parcourir tout le fichier pour récupérer le profil le plus proche possible s'il y en a un
            while (!line.isEmpty()) {
                list = line.split(",");

                int differenceAgressiveness = abs(list.at(AI_AGGRESSIVENESS).toDouble() - m_aiCurrentCalibration->getAggressiveness());

                //Si l'agressivité varie de + ou - la variation autorisée, On fait la même chose pour les chances de gain de l'IA
                if (differenceAgressiveness <= ALLOWED_VARIATION) {

                    //On regarde la différence de chances de gains de l'IA:
                    int differenceAIWinningChances = abs(list.at(AI_WINNING_CHANCES).toDouble() - getWinningChances());
                    if (differenceAIWinningChances <= ALLOWED_VARIATION) {

                        // On regarde si on est dans le même palier

                        double averagePreviousDifference = static_cast<double>(differencePreviousAgressiveness + differencePreviousAIWinningChances) / 2;
                        double actualAverage = static_cast<double>(differenceAgressiveness + differenceAIWinningChances) / 2;

                        if (actualAverage < averagePreviousDifference) {

                            differencePreviousAgressiveness = differenceAgressiveness;
                            differencePreviousAIWinningChances = differenceAIWinningChances;

                            m_expectedActionPlayer.setAggressiveness(list.at(REAL_AGGRESSIVENESS).toDouble());
                            m_expectedActionPlayer.setRationality(list.at(REAL_RATIONALITY).toDouble());
                        }
                    }
                }

                line = profileFile.readLine();
            }
        }
    }
}
