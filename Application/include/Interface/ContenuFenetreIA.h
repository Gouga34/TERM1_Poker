/*========================================================================
Nom: ContenuFenetreIA.h       Auteur: Morgane VIDAL
Maj: 22/05/2015          Creation: 22/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre affichée lorsque
               ce sont deux intelligences artificielles qui jouent.
=========================================================================*/

#ifndef CONTENUFENETREIA_H
#define CONTENUFENETREIA_H

#include "ContenuFenetre.h"
#include <QLabel>
#include <QTableWidget>

class ContenuFenetreIA : public ContenuFenetre{
private:
    QLabel etatParties;
    QTableWidget recapParties;

    QTableWidget resultatsGlobaux;

    QLabel calibrageIAProfilee;


    enum COLONNES_RECAPPARTIES{
        SCENARIOS_EN_COURS,
        AGRESSIVITE_IA_QUI_PROFILE,
        RATIONALITE_IA_QUI_PROFILE,
        AGRESSIVITE_DEDUITE,
        RATIONALITE_DEDUITE,
        TAUX_SIMILARITE,
        JEU_AGRESSIF,
        GAINS,
        GAIN_IA_QUI_PROFILE
    };

    enum COLONNES_RESULTATSGLOBAUX{
        AG_DEDUITE,
        RA_DEDUITE,
        SIMILARITE,
        NB_JEUX_AGRESSIFS,
        TOTAL_GAINS,
        NB_PARTIES_GAGNEES
    };


    /**
     * @brief majResultatsGlobaux maj les résultats globaux de la partie
     */
    void majResultatsGlobaux();

    /**
     * @brief majCalibrageIAProfilee maj l'affichage du calibrage de l'ia profilée
     */
    void majCalibrageIAProfilee();

    /**
     * @brief initialisationResultatsGlobaux initialise les données des résultats globaux à 0.
     */
    void initialisationResultatsGlobaux();

    /**
     * @brief initialisationRecapParties initialise le tableau récapitulatif des parties
     */
    void initialisationRecapParties();

    /**
     * @brief ajouterLigne ajoute une ligne dans le récapitulatif des parties
     */
    void ajouterLigne();

    /**
     * @brief Scroll automatiquement vers le bas pour afficher la denrière ligne du tableau
     */
    void scrollAutomatiqueTableau();

public:
    ContenuFenetreIA(Jeu *j);
    virtual ~ContenuFenetreIA();

    /**
     * @brief actualiser actualise le récapitulatif des parties.
     */
    virtual void actualiser();
};

#endif // CONTENUFENETREIA_H
