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

struct Options
{
    QString pseudo;

    bool joueurIA;

    int nombreCalibrages;
    int nombreParties;

    int nombrePartiesProfilage;
    int nombrePartiesReprofilage;
    int nombrePartiesGains;

    bool analyseGainsParties;

    double rationaliteIA;
    double agressiviteIA;
};


class ChoixOptionsDialog : public QDialog{

    Q_OBJECT

private :

   QLineEdit pseudo;

   QRadioButton modeProfilage;
   QRadioButton modeCalibrageOptimal;

   QStackedLayout layoutParametres;

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

   QSpinBox boiteNombrePartiesCalibrageOptimal;

   /**
    * @action ajoute dans la liste déroulante pseudos les pseudos déjà connus.
    * Ces pseudos sont récupérés dans le fichier "../../ressources/Profilage/[ProfilageStatique]OU[ProfilageDynamique]/pseudos.txt" se trouvant
    */
   void ajouterPseudosConnus();

public :
    ChoixOptionsDialog();
    virtual ~ChoixOptionsDialog();

    /**
    * @return retourne les options fournies par l'utilisateur
    *         (pseudo déjà existant ou non) et le calibrage de l'IA
    */
    Options getOptions();

private slots:

    /**
     * @brief Change l'index du QStackedLayout
     */
    void changePageLayout();
};

#endif
