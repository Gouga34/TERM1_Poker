/*========================================================================
Nom: Fenetre.h       Auteur: Manuel CHATAIGNER
Maj: 26/03/2015          Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe contenant l'ensemble des propriétés de la fenêtre
               graphique.
=========================================================================*/

#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "ContenuFenetre.h"

class Fenetre : public QWidget
{
    Q_OBJECT

    private:

        Game *jeu;

        QPushButton boutonDemarrage;

        ContenuFenetre *contenu;

        QVBoxLayout *layout;

    private slots:

        /**
         * @brief Initialise ou réinitialise l'application
         */
        void initialiser();

    public:

        Fenetre();
        ~Fenetre();

    public slots:

        /**
         * @action Distribue les cartes de pre-flop à tous les joueurs et lance le jeu
         */
        void demarragePartie();

        /**
         * @action Lance les tests pour déterminer le calibrage idéal
         */
        void demarrageCalibrageIdeal();
};

#endif // FENETRE_H
