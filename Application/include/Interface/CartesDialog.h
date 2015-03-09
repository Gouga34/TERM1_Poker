/*========================================================================
Nom: CartesDialog.h       Auteur: Manuel CHATAIGNER
Maj: 09/03/2015          Creation: 05/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre de choix des cartes.
=========================================================================*/

#ifndef CARTESDIALOG_H
#define CARTESDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <vector>
#include "CarteCliquable.h"

class CartesDialog : public QDialog
{
    Q_OBJECT

    private:

        QGridLayout layoutCartes;

        std::vector<int> cartesSelectionnees;

    public:

        CartesDialog(QWidget *parent = 0);
        ~CartesDialog();

        /**
         * @action Ouvre la fenêtre permettant au joueur de choisir les 2 cartes de l'adversaire
         * @return Vecteur des identifiants des deux cartes séletionnées, vide si pas de choix
         */
        std::vector<int> choixCartes();

    public slots:

        /**
         * @action Selectionne la carte cliquée
         * @param id Identifiant de la carte
         */
        void carteSelectionnee(int id);
};

#endif // CARTESDIALOG_H
