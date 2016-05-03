/*========================================================================
Nom: ChoixOptionsDialog.cpp       Auteur: Morgane Vidal
Maj: 11/03/2015            Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe
                                                        ChoixOptionsDialog.
=========================================================================*/

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "../../include/Gui/OptionsDialog.h"
#include "../../include/Constants.h"


namespace gui
{
    OptionsDialog::OptionsDialog()
        : QDialog(), m_profiledAiCalibration("IA profilée"), m_profilingAiCalibration("IA qui profile"),
            m_opponentCalibration("")
    {
        //titre fenêtre
        setWindowTitle("Choix des options");

        //fenetre
        QVBoxLayout *layout = new QVBoxLayout;

        //ajouterPseudosConnus();

        m_profilingMode.setText("Profilage");
        m_optimalCalibrationMode.setText("Recherche du calibrage optimal");

        connect(&m_profilingMode, SIGNAL(clicked()), this, SLOT(changeLayoutPage()));
        connect(&m_optimalCalibrationMode, SIGNAL(clicked()), this, SLOT(changeLayoutPage()));

        QGroupBox *parameters = new QGroupBox("Paramètres");

        QWidget *profilingWidget = new QWidget;
        QVBoxLayout *profilingLayout = new QVBoxLayout;

        m_opponentChoice.setTitle("Joueur humain");
        m_opponentChoice.setCheckable(true);
        m_opponentChoice.setChecked(false);

        m_pseudoField = new QLineEdit;

        QFormLayout *humanPlayerLayout = new QFormLayout;
        humanPlayerLayout->addRow("Pseudo", m_pseudoField);

        m_opponentChoice.setLayout(humanPlayerLayout);

        m_calibrationsNumberBox.setValue(1);
        m_calibrationsNumberBox.setMaximum(500);

        m_partsNumberBox.setValue(10);
        m_partsNumberBox.setMaximum(500);

        m_optimalCalibrationPartsNumberBox.setMaximum(1000);
        m_optimalCalibrationPartsNumberBox.setValue(100);

        m_profilingPartsNumberBox.setValue(10);
        m_reprofilingPartsNumberBox.setValue(5);
        m_winningsPartsNumberBox.setValue(10);
        m_winningsPartsNumberBox.setMaximum(500);

        m_resetCavesCheckBox.setChecked(true);

        //bouton valider
        QPushButton *button = new QPushButton("Valider");

        //Formulaire
        QFormLayout *form = new QFormLayout;

        form->addRow("Nombre de calibrages à profiler", &m_calibrationsNumberBox);
        form->addRow("Nombre de parties", &m_partsNumberBox);
        form->addRow("Nombre de parties profilage", &m_profilingPartsNumberBox);
        form->addRow("Nombre de parties reprofilage", &m_reprofilingPartsNumberBox);
        form->addRow("Nombre de parties minimum phase de gains", &m_winningsPartsNumberBox);
        form->addRow("Analyse des gains de parties", &m_winningsAnalysisCheckBox);
        form->addRow("Réinitialisation des caves entre chaque partie", &m_resetCavesCheckBox);

        profilingLayout->addWidget(&m_opponentChoice);
        profilingLayout->addLayout(form);
        profilingLayout->addWidget(&m_profiledAiCalibration);
        profilingLayout->addWidget(&m_profilingAiCalibration);

        profilingWidget->setLayout(profilingLayout);


        QWidget *optimalCalibrationWidget = new QWidget;
        QFormLayout *optimalCalibrationLayout = new QFormLayout;

        m_opponentCalibration.setCheckable(false);

        optimalCalibrationLayout->addRow("Nombre de parties", &m_optimalCalibrationPartsNumberBox);
        optimalCalibrationLayout->addRow("Calibrage adverse", &m_opponentCalibration);

        optimalCalibrationWidget->setLayout(optimalCalibrationLayout);


        m_parametersLayout.addWidget(profilingWidget);
        m_parametersLayout.addWidget(optimalCalibrationWidget);

        m_profilingMode.setChecked(true);

        parameters->setLayout(&m_parametersLayout);

        //Ajout du formulaire et du bouton valider dans la fenetre
        layout->setAlignment(Qt::AlignHCenter);
        layout->addWidget(&m_profilingMode);
        layout->addWidget(&m_optimalCalibrationMode);
        layout->addWidget(parameters);
        layout->addStretch();
        layout->addWidget(button);

        setLayout(layout);

        //Connection avec l'interface graphique du jeu
        connect(button, SIGNAL(clicked()), this, SLOT(accept()));
    }

    OptionsDialog::~OptionsDialog()
    {

    }

    void OptionsDialog::changeLayoutPage()
    {
        if (m_profilingMode.isChecked()) {
            m_parametersLayout.setCurrentIndex(0);
        }
        else {
            m_parametersLayout.setCurrentIndex(1);
        }
    }

    GameOptions OptionsDialog::getOptions()
    {
        GameOptions options;

        if (exec() == QDialog::Accepted) {

            options.profiling = m_profilingMode.isChecked();

            if (m_profilingMode.isChecked()) {

                options.aiPlayer = !m_opponentChoice.isChecked();
                options.pseudo = m_pseudoField->text();

                options.nbCalibrations = m_calibrationsNumberBox.value();
                options.nbParts = m_partsNumberBox.value();

                options.nbProfilingParts = m_profilingPartsNumberBox.value();
                options.nbReprofilingParts = m_reprofilingPartsNumberBox.value();
                options.nbWinningsParts = m_winningsPartsNumberBox.value();

                options.partsWinningsAnalysis = m_winningsAnalysisCheckBox.isChecked();
                options.resetCaves = m_resetCavesCheckBox.isChecked();

                options.fixedProfiledAiCalibration = m_profiledAiCalibration.isChecked();
                options.fixedProfilingAiCalibration = m_profilingAiCalibration.isChecked();

                if (m_profiledAiCalibration.isChecked()) {
                    options.profiledAi = m_profiledAiCalibration.getCalibration();
                }

                if (m_profilingAiCalibration.isChecked()) {
                    options.profilingAi = m_profilingAiCalibration.getCalibration();
                }
            }
            else {
                options.aiPlayer = true;
                options.nbParts = m_optimalCalibrationPartsNumberBox.value();
                options.profiledAi = m_opponentCalibration.getCalibration();
            }
        }

        return options;
    }
}
