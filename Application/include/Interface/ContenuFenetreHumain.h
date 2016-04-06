/*========================================================================
Nom: ContenuFenetre.h       Auteur: Manuel CHATAIGNER
Maj: 20/05/2015          Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre de choix des cartes.
=========================================================================*/

#ifndef CONTENUFENETREHUMAIN_H
#define CONTENUFENETREHUMAIN_H

#include "WindowContent.h"
#include "CardsList.h"
#include "TokensCounter.h"
#include "Window.h"

#include <QPixmap>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>

class ContenuFenetreHumain : public WindowContent
{
    Q_OBJECT

    private:

        //Informations jeu
        QLineEdit actionEffectueeIA;
        QLabel resultatPartie;

        // Logs
        QPushButton boutonLogs;

        // Liste des cartes
        CardsList layoutMain;
        CardsList layoutMainAdverse;
        CardsList layoutCartesCommunes;

        // Compteur du pot et des caves des joueurs
        TokensCounter pot;
        TokensCounter caveJoueur;
        TokensCounter caveIA;

        // Boite de saisie du montant de la mise
        QSpinBox valeurMise;

        QPushButton boutonDemarrage;
        QCheckBox boutonChoixCartes;


        enum { CHECKER, MISER, SUIVRE, RELANCER, SE_COUCHER, TAPIS, NB_BOUTONS };

        // Boutons d'action de jeu
        QPushButton boutons[NB_BOUTONS];

        ACTION actionUtilisateur;


        /**
         * @action Active ou désactive l'ensemble des boutons d'action du joueur
         *         Lors de l'activation, vérifie les actions possibles
         * @param active Vrai s'il faut activer les boutons, faux pour les désactiver
         */
        void activeBoutons(bool active);

        /**
         * @action afficheTable Réupère l'ensemble des cartes de la table
         *                      à partir du jeu et les affiche
         */
        void afficheTable();

        /**
         * @action Effectue les actions de fin de partie
         */
        void partieTermine();

    signals:

        /**
         * @action Signal émis lorsque le joueur a choisi une action
         */
        void actionChoisie();

    public:

        // Image contenant l'ensemble des cartes graphiques du jeu de carte
        static QPixmap *textureCartes;

        ContenuFenetreHumain(game::Game *j, Window *f);
        virtual ~ContenuFenetreHumain();

        /**
         * @brief Met à jour l'interface pour le début de partie
         */
        void debutPartie();

        /**
         * @brief getAction
         * @return Retourne l'action du joueur lorsque c'est à lui de jouer
         */
        game::Action getAction();

        /**
         * @brief Met à jour l'interface
         */
        virtual void refresh();

    public slots:

        /**
         * @action Affiche/cache la boite de logs lorsque le joueur appuie sur le bouton correspondant
         */
        void affichageLogs();

        /** Méthodes d'action de jeu appelées une fois l'action choisie par le joueur courant **/

        void checker();

        void miser();

        void suivre();

        void relancer();

        void seCoucher();

        void tapis();
};

#endif // CONTENUFENETREHUMAIN_H
