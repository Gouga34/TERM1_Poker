/*========================================================================
Nom: ChoixOptionsDialog.h       Auteur: Morgane Vidal
Maj: 11/03/2015          Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre de choix du pseudo.
=========================================================================*/

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QStackedLayout>
#include "CalibrationChoice.h"

namespace gui
{
    struct GameOptions
    {
        bool profiling;         // Choix profilage/calibrage optimal

        bool aiPlayer;

        QString pseudo;

        int nbCalibrations;
        int nbParts;

        int nbProfilingParts;
        int nbReprofilingParts;
        int nbWinningsParts;

        bool partsWinningsAnalysis;
        bool resetCaves;

        bool fixedProfiledAiCalibration;
        bool fixedProfilingAiCalibration;

        profiling::Profile profiledAi;
        profiling::Profile profilingAi;
    };


    class OptionsDialog : public QDialog
    {

        Q_OBJECT

    private :

       QRadioButton m_profilingMode;
       QRadioButton m_optimalCalibrationMode;

       QStackedLayout m_parametersLayout;

       //// Paramètres profilage ////

       QLineEdit *m_pseudoField;
       QGroupBox m_opponentChoice;
       QSpinBox m_calibrationsNumberBox;
       QSpinBox m_partsNumberBox;

       QSpinBox m_profilingPartsNumberBox;
       QSpinBox m_reprofilingPartsNumberBox;
       QSpinBox m_winningsPartsNumberBox;

       QCheckBox m_winningsAnalysisCheckBox;
       QCheckBox m_resetCavesCheckBox;

       CalibrationChoice m_profiledAiCalibration;
       CalibrationChoice m_profilingAiCalibration;

       //// Paramètres calibrage optimal ////

       QSpinBox m_optimalCalibrationPartsNumberBox;

       CalibrationChoice m_opponentCalibration;


    public :

        OptionsDialog();
        virtual ~OptionsDialog();

        /**
        * @return retourne les options fournies par l'utilisateur
        *         (pseudo déjà existant ou non) et le calibrage de l'IA
        */
        virtual GameOptions getOptions();

    private slots:

        /**
         * @brief Change l'index du QStackedLayout
         */
        virtual void changeLayoutPage();
    };
}

#endif // OPTIONSDIALOG_H
