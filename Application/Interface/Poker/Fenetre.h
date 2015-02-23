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

        QTextEdit logs;
        QPushButton boutonLogs;

        ListeCartes layoutMain;
        ListeCartes layoutMainAdverse;
        ListeCartes layoutCartesCommunes;

        QLCDNumber pot;
        QLCDNumber caveJoueur;
        QLCDNumber caveIA;

        QSpinBox valeur;

        QPushButton next;

        QPushButton boutonChecker;
        QPushButton boutonMiser;
        QPushButton boutonSuivre;
        QPushButton boutonRelancer;
        QPushButton boutonSeCoucher;

        Jeu *jeu;

    public:

        static QPixmap *textureCartes;

        Fenetre(Jeu *j);
        ~Fenetre();

        void activeBoutons(bool active);

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
