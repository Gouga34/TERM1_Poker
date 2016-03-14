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

class ScenariosDeTests{
private:
  //Attributs :
    Profile calibrageRecherche;
    Profile *calibrageActuelIA;
   // int numeroDuTestActuel; //correspond à l'itération du test où on en est. exemple : 3m itération du test avec pour calibrage 100 100.
    Profile actionAttendueJoueur;
    Profile *actionReelleJoueur;
    double chancesDeGain;
    bool joueurSeCouche; //attribut pas encore traité dans les méthodes. A traiter plus tard
    double distance;
    //int numeroCalibrage;

    Profile profilDeduitGlobal;
    double tauxSimilarite;

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
    void ancienneSituationLaPlusProche(QFile &fichierProfil);

public:
    ScenariosDeTests(Profile *profilJoueur, Profile *calibrageIA, Profile calibrageAdversaire);

    ~ScenariosDeTests();

    /**
     * @brief setCalibrageActuelIA
     * @param calibrage
     */
    void setCalibrageActuelIA(Profile *calibrage);

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
    Profile *getCalibrageActuelIA();

    /**
     * @brief setActionAttendueJoueur
     * @param actionAttendue
     */
    void setActionAttendueJoueur(Profile actionAttendue);

    /**
     * @brief getActionAttendueJoueur
     * @return l'action attendue du joueur
     */
    Profile getActionAttendueJoueur() const;

    /**
     * @brief getProfilDeduitGlobal
     * @return Profil déduit global du joueur profilé
     */
    Profile getProfilDeduitGlobal() const;

    /**
     * @brief getTauxSimilarite
     * @return Taux de similarité du profilage
     */
    double getTauxSimilarite() const;

    /**
     * @brief setActionReelleJoueur
     * @param action
     */
    void setActionReelleJoueur(Profile *action);

    /**
     * @brief getActionReelleJoueur
     * @return l'action réelle effectuée par le joueur
     */
    Profile *getActionReelleJoueur() const;

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
     * @action écrit le scénario dans le fichier de sortie "pseudo_scenarios_tests.json"
     */
    void sauvegarderPartie();
};

#endif // SCENARIOSDETESTS_H
