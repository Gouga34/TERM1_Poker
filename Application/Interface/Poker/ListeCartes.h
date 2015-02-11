#ifndef LISTECARTES_H
#define LISTECARTES_H

#include <QHBoxLayout>
#include <QList>
#include <QLabel>

class ListeCartes : public QHBoxLayout
{
    public:

        ListeCartes();
        ~ListeCartes();

        void ajoutCartes(QList<QLabel*> cartes);
};

#endif // LISTECARTES_H
