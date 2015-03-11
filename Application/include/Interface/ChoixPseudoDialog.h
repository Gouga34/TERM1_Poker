/*========================================================================
Nom: ChoixPseudoDialog.h       Auteur: Morgane Vidal
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
#include <QPushButton>
#include<QString>


class ChoixPseudoDialog : public QDialog{

private :
   QLineEdit nouveau;
   QComboBox pseudos; //liste déroulante contenant les pseudos déjà connus.

   /**
    * @action ajoute dans la liste déroulante pseudos les pseudos déjà connus.
    * Ces pseudos sont récupérés dans le fichier "../../ressources/Profilage/[ProfilageStatique]OU[ProfilageDynamique]/pseudos.txt" se trouvant
    */
   void ajouterPseudosConnus();

public :
    ChoixPseudoDialog();

    /**
    * @return retourne le pseudo fourni par l'utilisateur (pseudo déjà existant ou non)
    */
    QString getPseudo();


};



#endif
