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
#include <QRadioButton>
#include <vector>
#include "CarteCliquable.h"

class CartesDialog : public QDialog
{
    Q_OBJECT

    private:

        enum ListeCartes { JOUEUR, IA, MILIEU };

        QGridLayout layoutCartes;

        // Vecteur des identifants des cartes choisies (2 Joueur, 2 IA, 5 milieu)
        std::vector<int> cartesSelectionnees;

        QRadioButton choixJoueur;
        QRadioButton choixIA;
        QRadioButton choixMilieu;

        /**
         * @action Ajoute la carte sélectionnée dans la liste de cartes correspondante
         * @param id Identifiant de la carte à ajouter
         * @param liste Liste dans laquelle ajouter la carte
         * @return true si la carte a été ajoutée, faux sinon (liste pleine)
         */
        bool ajoutCarte(int id, ListeCartes liste);

    private slots:

        /**
         * @action Selectionne la carte cliquée
         * @param id Identifiant de la carte
         */
        void carteSelectionnee(int id);

    public:

        CartesDialog(QWidget *parent = 0);
        ~CartesDialog();

        /**
         * @action Ouvre la fenêtre permettant au joueur de choisir les 2 cartes de l'adversaire
         * @return Vecteur des identifiants des deux cartes séletionnées, vide si pas de choix
         */
        std::vector<int> choixCartes();
};

#endif // CARTESDIALOG_H
