#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QLCDNumber>
#include <QSpinBox>

class Fenetre : public QWidget
{
    Q_OBJECT

    private:

        QPixmap textureCartes;

        QList<QLabel*> main;
        QList<QLabel*> mainAdverse;
        QList<QLabel*> communes;

        QLCDNumber pot;
        QLCDNumber cave;

        QSpinBox valeur;

    public:

        Fenetre();
        ~Fenetre();

    public slots:

        void miser();
};

#endif // FENETRE_H
