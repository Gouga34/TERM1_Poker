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

#include "Profil.h"
#include <QJsonObject>
#include <QFile>

class ScenariosDeTests{
private:
  //Attributs :
    Profil *calibrageActuelIA;
   // int numeroDuTestActuel; //correspond à l'itération du test où on en est. exemple : 3m itération du test avec pour calibrage 100 100.
    Profil actionAttendueJoueur;
    Profil *actionReelleJoueur;
    double chancesDeGain;
    bool joueurSeCouche; //attribut pas encore traité dans les méthodes. A traiter plus tard
    double distance;
    //int numeroCalibrage;

    /**
     * @brief scenarioSuivant
     * @action passe calibrageActuelIA au calibrage du scénario suivant
     */
    void scenarioSuivant();

public:
    ScenariosDeTests(Profil *profilJoueur, Profil *calibrageIA);

    ~ScenariosDeTests();

    /**
     * @brief setCalibrageActuelIA
     * @param calibrage
     */
    void setCalibrageActuelIA(Profil *calibrage);

    /**
     * @brief setJoueurSeCouche
     * @param couche
     */
    void setJoueurSeCouche(bool couche);

    /**
     * @brief getJoueurSeCouche
     * @return true si le joueur c'est couché pendant la partie et false sinon
     */
    bool getJoueurSeCouche();

    /**
     * @brief setDistance
     * @param dist
     */
    void setDistance(double dist);

    /**
     * @brief getDistance
     * @return la distance entre l'action réelle et l'action attendue
     */
    double getDistance();

    /**
     * @brief getCalibrageActuelIA
     * @return le calibrage actuel de l'IA
     */
    Profil *getCalibrageActuelIA();

    /**
     * @brief setActionAttendueJoueur
     * @param actionAttendue
     */
    void setActionAttendueJoueur(Profil actionAttendue);

    /**
     * @brief getActionAttendueJoueur
     * @return l'action attendue du joueur
     */
    Profil getActionAttendueJoueur() const;

    /**
     * @brief setActionReelleJoueur
     * @param action
     */
    void setActionReelleJoueur(Profil *action);

    /**
     * @brief getActionReelleJoueur
     * @return l'action réelle effectuée par le joueur
     */
    Profil *getActionReelleJoueur() const;

    /**
     * @brief setNumeroDuTestActuel
     * @param numeroTest nouveau numéro de test
     */
    void setNumeroDuTestActuel(int numeroTest);

    /**
     * @brief getNumeroDuTestActuel
     * @return le numéro du test actuel
     */
    int getNumeroDuTestActuel() const;

    /**
     * @brief setChancesDeGain
     * @param chancesGain chances de gain du joueur humain
     */
    void setChancesDeGain(double chancesGain);

    /**
     * @brief getChancesDeGain
     * @return les chances de gain du joueur humain
     */
    double getChancesDeGain();

    /**
     * @brief sauvegarderLigne
     * @action écrit le scénario dans le fichier de sortie "pseudo_scenarios_tests.json"
     */
    void sauvegarderPartie();

    /**
     * @brief calculerActionAttendueJoueur
     * @action calcule l'action attendue du joueur,
     *         éventuellement en fonction des actions réelles précédentes
     */
    void calculerActionAttendueJoueur(QFile &fichierProfil);

    /**
     * @brief calculerDistance
     * @action calcule la distance entre l'action réelle et l'action attendue
     */
    void calculerDistance();

    /**
     * @brief ancienneSituationLAPlusProche
     * @action cherche et trouve s'il y en a une, la situation déjà rencontrée la plus proche de la situation actuelle
     * (chances de gain et agressivité et chances de gain joueur)
     * @param fichierProfil
     */
    void ancienneSituationLAPlusProche(QFile &fichierProfil);



};

#endif // SCENARIOSDETESTS_H
