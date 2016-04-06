/*========================================================================
Nom: Fenetre.h       Auteur: Manuel CHATAIGNER
Maj: 26/03/2015          Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe contenant l'ensemble des propriétés de la fenêtre
               graphique.
=========================================================================*/

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "WindowContent.h"

class Window : public QWidget
{
    Q_OBJECT

    private:

        game::Game *m_game;

        QPushButton m_startButton;

        WindowContent *m_content;

        QVBoxLayout *m_layout;

    private slots:

        /**
         * @brief Initialise ou réinitialise l'application
         */
        virtual void initialize();

    public:

        Window();
        virtual ~Window();

    public slots:

        /**
         * @brief Distribue les cartes de pre-flop à tous les joueurs et lance le jeu
         */
        virtual void startGame();

        /**
         * @brief Lance les tests pour déterminer le calibrage idéal
         */
        virtual void startOptimalCalibration();
};

#endif // WINDOW_H
