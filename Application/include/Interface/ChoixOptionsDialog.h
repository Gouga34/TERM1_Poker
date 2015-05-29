/*========================================================================
Nom: ChoixOptionsDialog.h       Auteur: Morgane Vidal
Maj: 11/03/2015          Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre de choix du pseudo.
=========================================================================*/

#ifndef CHOIXPSEUDODIALOG_H
#define CHOIXPSEUDODIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QCheckBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QStackedLayout>
#include "ChoixCalibrage.h"

struct OptionsJeu
{
    bool profilage;         // Choix profilage/calibrage optimal

    bool joueurIA;

    QString pseudo;

    int nombreCalibrages;
    int nombreParties;

    int nombrePartiesProfilage;
    int nombrePartiesReprofilage;
    int nombrePartiesGains;

    bool analyseGainsParties;
    bool reinitialisationCaves;

    bool calibrageIaProfileeFixe;
    bool calibrageIaQuiProfileFixe;

    Profil iaProfilee;
    Profil iaQuiProfile;
};


class ChoixOptionsDialog : public QDialog{

    Q_OBJECT

private :

   QRadioButton modeProfilage;
   QRadioButton modeCalibrageOptimal;

   QStackedLayout layoutParametres;

   //// Paramètres profilage ////

   QLineEdit champPseudo;
   QGroupBox choixAdversaire;
   QSpinBox boiteNombreCalibrages;
   QSpinBox boiteNombreParties;

   QSpinBox boiteNombrePartiesProfilage;
   QSpinBox boiteNombrePartiesReprofilage;
   QSpinBox boiteNombrePartiesGains;

   QCheckBox caseAnalyseGainsParties;
   QCheckBox caseReinitialisationCaves;

   ChoixCalibrage calibrageIAProfilee;
   ChoixCalibrage calibrageIAQuiProfile;

   //// Paramètres calibrage optimal ////

   QSpinBox boiteNombrePartiesCalibrageOptimal;

   ChoixCalibrage calibrageAdversaire;


public :
    ChoixOptionsDialog();
    virtual ~ChoixOptionsDialog();

    /**
    * @return retourne les options fournies par l'utilisateur
    *         (pseudo déjà existant ou non) et le calibrage de l'IA
    */
    OptionsJeu getOptions();

private slots:

    /**
     * @brief Change l'index du QStackedLayout
     */
    void changePageLayout();
};

#endif
