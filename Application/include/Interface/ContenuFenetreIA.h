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
    QLabel calibrageIAProfilee;

    enum COLONNES{
        SCENARIOS_EN_COURS=0,
        AGRESSIVITE_IA_QUI_PROFILE=1,
        RATIONALITE_IA_QUI_PROFILE=2,
        AGRESSIVITE_DEDUITE=3,
        RATIONALITE_DEDUITE=4,
        TAUX_SIMILARITE=5,
        GAINS=6,
        GAIN_IA_QUI_PROFILE=7
    };

    /**
     * @brief majCalibrageIAProfilee maj l'affichage du calibrage de l'ia profilée
     */
    void majCalibrageIAProfilee();

    /**
     * @brief initialisationRecapParties initialise le tableau récapitulatif des parties
     */
    void initialisationRecapParties();

    /**
     * @brief ajouterLigne ajoute une ligne dans le récapitulatif des parties
     */
    void ajouterLigne();

public:
    ContenuFenetreIA(Jeu *j);
    virtual ~ContenuFenetreIA();

    /**
     * @brief actualiser actualise le récapitulatif des parties.
     */
    virtual void actualiser();
};

#endif // CONTENUFENETREIA_H
