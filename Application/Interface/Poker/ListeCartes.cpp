#include "ListeCartes.h"


ListeCartes::ListeCartes() : QHBoxLayout()
{
    setSpacing(10);
    setAlignment(Qt::AlignLeft);
}

ListeCartes::~ListeCartes()
{

}

void ListeCartes::ajoutCartes(QList<QLabel*> cartes)
{
    for (int i = 0; i < cartes.size(); ++i)
    {
        this->addWidget(cartes.at(i));
    }
}
