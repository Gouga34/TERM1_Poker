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
#include <QSlider>

struct Options
{
    QString pseudo;

    double rationaliteIA;
    double agressiviteIA;
};


class ChoixOptionsDialog : public QDialog{

private :
   const int rationaliteDefaut = 100;
   const int agressiviteDefaut = 50;

   QLineEdit nouveau;
   QComboBox pseudos; //liste déroulante contenant les pseudos déjà connus.

   QSlider rationalite;
   QSlider agressivite;

   /**
    * @action ajoute dans la liste déroulante pseudos les pseudos déjà connus.
    * Ces pseudos sont récupérés dans le fichier "../../ressources/Profilage/[ProfilageStatique]OU[ProfilageDynamique]/pseudos.txt" se trouvant
    */
   void ajouterPseudosConnus();

public :
    ChoixOptionsDialog();

    /**
    * @return retourne les options fournies par l'utilisateur
    *         (pseudo déjà existant ou non) et le calibrage de l'IA
    */
    Options getOptions();

};



#endif
