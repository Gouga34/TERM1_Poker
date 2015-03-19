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
#include <QHBoxLayout>
#include <QVBoxLayout>


CartesDialog::CartesDialog(QWidget *parent) : QDialog(parent), cartesSelectionnees(9, -1)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(20, 127, 20));
    setPalette(pal);

    QHBoxLayout *layout = new QHBoxLayout;

    for (int i = 0; i < 4; i++) {
        layoutCartes.setRowMinimumHeight(i, 107);
    }

    for (int i = 1; i <= 13; i++) {
        layoutCartes.setColumnMinimumWidth(i-1, 83);

        for (int j = 0; j < 4; j++) {
            CarteCliquable *c = new CarteCliquable(i, j);
            layoutCartes.addWidget(c, j, i, Qt::AlignCenter);

            connect(c, SIGNAL(clicked(int)), this, SLOT(carteSelectionnee(int)));
        }
    }

    QDialogButtonBox *boutonDialog = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layoutCartes.addWidget(boutonDialog, 4, 12, 1, 2);

    QVBoxLayout *layoutBoutons = new QVBoxLayout;

    choixJoueur.setText("Joueur");
    choixIA.setText("IA");
    choixMilieu.setText("Milieu");
    choixJoueur.setChecked(true);

    layoutBoutons->addWidget(&choixJoueur);
    layoutBoutons->addWidget(&choixIA);
    layoutBoutons->addWidget(&choixMilieu);

    QWidget *boutons = new QWidget;
    boutons->setLayout(layoutBoutons);
    boutons->setObjectName(QString("Box"));
    boutons->setStyleSheet("QWidget#Box { background-color: rgb(200, 200, 200); border-style: solid; border-color: black; border-width: 1px }");

    layout->setSpacing(3);
    layout->addWidget(boutons);
    layout->addLayout(&layoutCartes);

    setLayout(layout);

    connect(boutonDialog, SIGNAL(accepted()), this, SLOT(accept()));
    connect(boutonDialog, SIGNAL(rejected()), this, SLOT(reject()));
}

CartesDialog::~CartesDialog()
{

}

std::vector<int> CartesDialog::choixCartes()
{
    if (exec() == QDialog::Accepted
        && std::find(cartesSelectionnees.begin(), cartesSelectionnees.end(), -1) == cartesSelectionnees.end()) {
        return cartesSelectionnees;
    }
    else {
        return std::vector<int>();
    }
}

bool CartesDialog::ajoutCarte(int id, ListeCartes liste)
{
    if (std::find(cartesSelectionnees.begin(), cartesSelectionnees.end(), id) == cartesSelectionnees.end()) {

        int pos = 0, nbCartes = 2;
        if (liste == IA) {
            pos = 2;
        }
        else if (liste == MILIEU) {
            pos = 4;
            nbCartes = 5;
        }

        int i = pos;
        while (i < (pos + nbCartes) && cartesSelectionnees.at(i) != -1) {
            i++;
        }

        if (i < (pos + nbCartes)) {
            cartesSelectionnees.at(i) = id;
            return true;
        }
    }

    return false;
}

void CartesDialog::carteSelectionnee(int id)
{
    QString style = "border: 5px solid ";
    ListeCartes liste;

    if (choixJoueur.isChecked()) {
        style += "blue";
        liste = JOUEUR;
    }
    else if (choixIA.isChecked()) {
        style += "red";
        liste = IA;
    }
    else if (choixMilieu.isChecked()) {
        style += "rgb(100, 250, 100)";
        liste = MILIEU;
    }

    if (ajoutCarte(id, liste)) {
        int rang = id % 13 + 1;
        int couleur = id / 13;

        QWidget *item = layoutCartes.itemAtPosition(couleur, rang)->widget();
        item->setStyleSheet(style);
    }
}
