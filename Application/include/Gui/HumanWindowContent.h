/*========================================================================
Nom: ContenuFenetre.h       Auteur: Manuel CHATAIGNER
Maj: 20/05/2015          Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre de choix des cartes.
=========================================================================*/

#ifndef HUMANWINDOWCONTENT_H
#define HUMANWINDOWCONTENT_H

#include "WindowContent.h"
#include "CardsList.h"
#include "TokensCounter.h"
#include "Window.h"

#include <QPixmap>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>

namespace gui
{
    class HumanWindowContent : public WindowContent
    {
        Q_OBJECT

        private:

            // Informations jeu
            QLineEdit m_aiActionDone;
            QLabel m_partResult;

            // Logs
            QPushButton m_logsButton;

            // Liste des cartes
            CardsList m_handLayout;
            CardsList m_opponentHandLayout;
            CardsList m_commonCardsLayout;

            // Compteur du pot et des caves des joueurs
            TokensCounter m_pot;
            TokensCounter m_playerCave;
            TokensCounter m_aiCave;

            // Boite de saisie du montant de la mise
            QSpinBox m_betValue;

            QPushButton m_startButton;
            QCheckBox m_cardsChoiceButton;


            enum { CHECK, BET, CALL, RAISE, DROP, ALL_IN, BUTTONS_NB };

            // Boutons d'action de jeu
            std::array<QPushButton, BUTTONS_NB> m_buttons;

            ACTION m_userAction;


            /**
             * @brief Active ou désactive l'ensemble des boutons d'action du joueur
             *         Lors de l'activation, vérifie les actions possibles
             * @param value Vrai s'il faut activer les boutons, faux pour les désactiver
             */
            virtual void enableButtons(bool value);

            /**
             * @brief displayTable Réupère l'ensemble des cartes de la table
             *                      à partir du jeu et les affiche
             */
            virtual void displayTable();

            /**
             * @brief Effectue les actions de fin de partie
             */
            virtual void endOfPart();

        signals:

            /**
             * @brief Signal émis lorsque le joueur a choisi une action
             */
            void actionSelected();

        public:

            // Image contenant l'ensemble des cartes graphiques du jeu de carte
            static QPixmap *m_cardsTexture;

            HumanWindowContent(game::Game *game, Window *window);
            virtual ~HumanWindowContent();

            /**
             * @brief Met à jour l'interface pour le début de partie
             */
            virtual void startPart();

            /**
             * @brief getAction
             * @return Retourne l'action du joueur lorsque c'est à lui de jouer
             */
            virtual game::Action getAction();

            /**
             * @brief Met à jour l'interface
             */
            virtual void refresh();

        public slots:

            /**
             * @brief Affiche/cache la boite de logs lorsque le joueur appuie sur le bouton correspondant
             */
            virtual void logsDisplay();

            /** Méthodes d'action de jeu appelées une fois l'action choisie par le joueur courant **/

            virtual void check();

            virtual void bet();

            virtual void call();

            virtual void raise();

            virtual void drop();

            virtual void allIn();
    };
}

#endif // HUMANWINDOWCONTENT_H
