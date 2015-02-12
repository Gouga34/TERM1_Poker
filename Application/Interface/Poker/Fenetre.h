#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QLCDNumber>
#include <QSpinBox>
#include "ListeCartes.h"

class Fenetre : public QWidget
{
    Q_OBJECT

    private:

        QPixmap textureCartes;

        QList<QLabel*> main;
        QList<QLabel*> mainAdverse;
        QList<QLabel*> communes;

        ListeCartes layoutMain;
        ListeCartes layoutMainAdverse;
        ListeCartes layoutCartesCommunes;

        QLCDNumber pot;
        QLCDNumber cave;

        QSpinBox valeur;

    public:

        Fenetre();
        ~Fenetre();

    public slots:

        void miser();

        void ajouterCarte();
};

#endif // FENETRE_H
