/*========================================================================
Nom: ScenariosDeTests.h       Auteur: Morgane VIDAL
Maj: 01/04/2015          Creation: 01/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe permettant d'écrire et
                                      d'exploiter les scénarios de tests
=========================================================================*/

#ifndef SCENARIOSDETESTS_H
#define SCENARIOSDETESTS_H

#include "Profile.h"
#include <QJsonObject>
#include <QFile>

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
    void calculateExpectedActionPlayer(QFile &profileFile);

    /**
     * @brief calculateDistance
     * @action calcule la distance entre l'action réelle et l'action attendue
     */
    void calculateDistance();

    /**
     * @brief oldNearestStatus
     * @action cherche et trouve s'il y en a une, la situation déjà rencontrée la plus proche de la situation actuelle
     * (chances de gain et agressivité et chances de gain joueur)
     * @param fichierProfil
     */
    void oldNearestStatus(QFile &profileFile);

public:
    TestScenarios(Profile *playerProfile, Profile *aiCalibration, Profile opponentProfile);

    ~TestScenarios();

    void setAiCurrentCalibration(Profile *aiCurrentCalibration);
    void setIsPlayerFolded(bool isPlayerFolded);
    void setDistance(double distance);
    void setExpectedActionPlayer(Profile expectedActionPlayer);
    void setRealActionPlayer(Profile *realActionPlayer);
    void setWinningChances(double winningChances);

    Profile *getAiCurrentCalibration() const;
    bool getIsPlayerFolded() const;
    double getDistance() const;
    Profile getExpectedActionPlayer() const;
    Profile *getRealActionPlayer() const;
    double getWinningChances() const;
    Profile getGlobalDeductedProfile() const;
    double getSimilarityDegree() const;


    /**
     * @brief saveGame écrit le scénario dans le fichier de sortie "pseudo_scenarios_tests.json"
     */
    void saveGame();
};

#endif // SCENARIOSDETESTS_H
