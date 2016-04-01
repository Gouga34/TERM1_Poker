/*========================================================================
Nom: ScenariosDeTests.h       Auteur: Morgane VIDAL
Maj: 01/04/2015          Creation: 01/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe permettant d'écrire et
                                      d'exploiter les scénarios de tests
=========================================================================*/

#ifndef TESTSCENARIOS_H
#define TESTSCENARIOS_H

#include "Profile.h"
#include <QJsonObject>
#include <QFile>

namespace profiling {

    class TestScenarios {
    private:

        Profile m_wantedCalibration;
        Profile *m_aiCurrentCalibration;
        Profile m_expectedActionPlayer;
        Profile *m_realActionPlayer;
        double m_winningChances;
        bool m_isPlayerFolded; //attribut pas encore traité dans les méthodes. A traiter plus tard
        double m_distance;

        Profile m_globalDeductedProfile;
        double m_similarityDegree;

        /**
         * @brief calculateExpectedActionPlayer
         * @action calcule l'action attendue du joueur,
         *         éventuellement en fonction des actions réelles précédentes
         */
        virtual void calculateExpectedActionPlayer(QFile &profileFile);

        /**
         * @brief calculateDistance
         * @action calcule la distance entre l'action réelle et l'action attendue
         */
        virtual void calculateDistance();

        /**
         * @brief oldNearestStatus
         * @action cherche et trouve s'il y en a une, la situation déjà rencontrée la plus proche de la situation actuelle
         * (chances de gain et agressivité et chances de gain joueur)
         * @param fichierProfil
         */
        virtual void oldNearestStatus(QFile &profileFile);

    public:
        TestScenarios(Profile *playerProfile, Profile *aiCalibration, Profile opponentProfile);

        virtual ~TestScenarios();

        virtual void setAiCurrentCalibration(Profile *aiCurrentCalibration);
        virtual void setIsPlayerFolded(bool isPlayerFolded);
        virtual void setDistance(double distance);
        virtual void setExpectedActionPlayer(Profile expectedActionPlayer);
        virtual void setRealActionPlayer(Profile *realActionPlayer);
        virtual void setWinningChances(double winningChances);

        virtual Profile *getAiCurrentCalibration() const;
        virtual bool getIsPlayerFolded() const;
        virtual double getDistance() const;
        virtual Profile getExpectedActionPlayer() const;
        virtual Profile *getRealActionPlayer() const;
        virtual double getWinningChances() const;
        virtual Profile getGlobalDeductedProfile() const;
        virtual double getSimilarityDegree() const;

        /**
         * @brief saveGame écrit le scénario dans le fichier de sortie "pseudo_scenarios_tests.json"
         */
        virtual void saveGame();
    };
}

#endif // TESTSCENARIOS_H
