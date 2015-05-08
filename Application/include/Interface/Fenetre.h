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
#include <QPixmap>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QLineEdit>

#include "ListeCartes.h"
#include "CompteurJetons.h"
#include "../Jeu/Jeu.h"

class Fenetre : public QWidget
{
    Q_OBJECT

    private:

        int nombreDeParties;

        //Informations jeu
        QLineEdit actionEffectueeIA;
        QLabel resultatPartie;

        // Logs
        QTextEdit logs;
        QPushButton boutonLogs;

        // Liste des cartes
        ListeCartes layoutMain;
        ListeCartes layoutMainAdverse;
        ListeCartes layoutCartesCommunes;

        // Compteur du pot et des caves des joueurs
        CompteurJetons pot;
        CompteurJetons caveJoueur;
        CompteurJetons caveIA;

        // Boite de saisie du montant de la mise
        QSpinBox valeurMise;

        QPushButton boutonDemarrage;
        QCheckBox boutonChoixCartes;


        enum { CHECKER, MISER, SUIVRE, RELANCER, SE_COUCHER, TAPIS, NB_BOUTONS };

        // Boutons d'action de jeu
        QPushButton boutons[NB_BOUTONS];

        Jeu *jeu;
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

    public:

        // Image contenant l'ensemble des cartes graphiques du jeu de carte
        static QPixmap *textureCartes;

        Fenetre();
        ~Fenetre();

        /**
         * @action Ajoute dans les logs le contenu passé en paramètre
         * @param contenu Texte à ajouter aux logs
         */
        void ajoutLogs(QString contenu);

        /**
         * @brief getAction
         * @return Retourne l'action du joueur lorsque c'est à lui de jouer
         */
        Action getAction();

    signals:

        /**
         * @action Signal émis lorsque le joueur a choisi une action
         */
        void actionChoisie();

    public slots:

        /**
         * @action Affiche/cache la boite de logs lorsque le joueur appuie sur le bouton correspondant
         */
        void affichageLogs();

        /**
         * @action Distribue les cartes de pre-flop à tous les joueurs et lance le jeu
         */
        void demarragePartie();

        /**
         * @action Lance les tests pour déterminer le calibrage idéal
         */
        void demarrageCalibrageIdeal();

        /**
         * @action Effectue les actions de fin de partie
         */
        void partieTermine();

        /** Méthodes d'action de jeu appelées une fois l'action choisie par le joueur courant **/

        void checker();

        void miser();

        void suivre();

        void relancer();

        void seCoucher();

        void tapis();
};

#endif // FENETRE_H
