#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QLCDNumber>
#include <QSpinBox>
#include <QPushButton>
#include "ListeCartes.h"
#include "../../include/Jeu.h"

class Fenetre : public QWidget
{
    Q_OBJECT

    private:

        QList<QLabel*> main;
        QList<QLabel*> mainAdverse;
        QList<QLabel*> communes;

        ListeCartes layoutMain;
        ListeCartes layoutMainAdverse;
        ListeCartes layoutCartesCommunes;

        QLCDNumber pot;
        QLCDNumber cave;

        QSpinBox valeur;

        QPushButton *next;

        Jeu *jeu;

    public:

        static QPixmap *textureCartes;

        Fenetre(Jeu *j);
        ~Fenetre();

    public slots:

        void demarragePartie();

        void distributionFlop();

        void miser();

        void ajouterCarte();
};

#endif // FENETRE_H
