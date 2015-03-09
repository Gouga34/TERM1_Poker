/*========================================================================
Nom: CartesDialog.cpp       Auteur: Manuel CHATAIGNER
Maj: 09/03/2015            Creation: 05/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CartesDialog.
=========================================================================*/

#include "../../include/Interface/CartesDialog.h"
#include <QDialogButtonBox>
#include <QPalette>


CartesDialog::CartesDialog(QWidget *parent) : QDialog(parent)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    setPalette(pal);

    for (int i = 1; i <= 13; i++) {
        for (int j = 0; j < 4; j++) {
            CarteCliquable *c = new CarteCliquable(i, j);
            layoutCartes.addWidget(c, j, i);

            connect(c, SIGNAL(clicked(int)), this, SLOT(carteSelectionnee(int)));
        }
    }

    QDialogButtonBox *boutonDialog = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layoutCartes.addWidget(boutonDialog, 4, 12, 1, 2);

    setLayout(&layoutCartes);

    connect(boutonDialog, SIGNAL(accepted()), this, SLOT(accept()));
    connect(boutonDialog, SIGNAL(rejected()), this, SLOT(reject()));
}

CartesDialog::~CartesDialog()
{

}

std::vector<int> CartesDialog::choixCartes()
{
    if (exec() == QDialog::Accepted && cartesSelectionnees.size() == 2) {
        return cartesSelectionnees;
    }
    else {
        return std::vector<int>();
    }
}

void CartesDialog::carteSelectionnee(int id)
{
    if (cartesSelectionnees.size() < 2) {
        int rang = id % 13 + 1;
        int couleur = id / 13;

        QWidget *item = layoutCartes.itemAtPosition(couleur, rang)->widget();
        item->setStyleSheet("border: 5px solid blue");

        if (cartesSelectionnees.size() == 0 || cartesSelectionnees.at(0) != id) {
            cartesSelectionnees.push_back(id);
        }
    }
}
