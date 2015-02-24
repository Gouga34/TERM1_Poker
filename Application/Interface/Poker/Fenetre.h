#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QLCDNumber>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include "ListeCartes.h"
#include "../../include/Jeu.h"

class Fenetre : public QWidget
{
    Q_OBJECT

    private:

        // Logs
        QTextEdit logs;
        QPushButton boutonLogs;

        // Liste des cartes
        ListeCartes layoutMain;
        ListeCartes layoutMainAdverse;
        ListeCartes layoutCartesCommunes;

        // Compteur du pot et des caves des joueurs
        QLCDNumber pot;
        QLCDNumber caveJoueur;
        QLCDNumber caveIA;

        // Boite de saisie du montant de la mise
        QSpinBox valeurMise;

        QPushButton next;

        // Boutons d'action de jeu
        QPushButton boutonChecker;
        QPushButton boutonMiser;
        QPushButton boutonSuivre;
        QPushButton boutonRelancer;
        QPushButton boutonSeCoucher;

        Jeu *jeu;


        void activeBoutons(bool active);

        void ajoutLogs(QString contenu);

        void jeuIA();

    public:

        static QPixmap *textureCartes;

        Fenetre(Jeu *j);
        ~Fenetre();

    signals:

        void tourFini();

    public slots:

        void affichageLogs();

        void demarragePartie();

        void afficheTable();

        void joueurCourant();

        void prochainJoueur();

        /** Actions joueur **/

        void checker();

        void miser();

        void suivre();

        void relancer();

        void seCoucher();
};

#endif // FENETRE_H
