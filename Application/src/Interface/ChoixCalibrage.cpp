/*========================================================================
Nom: ChoixCalibrage.cpp       Auteur: Manuel CHATAIGNER
Maj: 24/05/2015            Creation: 24/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ChoixCalibrage.
=========================================================================*/


#include "../../include/Interface/ChoixCalibrage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


ChoixCalibrage::ChoixCalibrage(const QString& titre, QWidget *parent) : QGroupBox(titre, parent)
{
    setCheckable(true);
    setChecked(false);

    QVBoxLayout *layout = new QVBoxLayout;

    // Rationalité
    QHBoxLayout *layoutRationalite = new QHBoxLayout;

    rationalite.setRange(0, 100);
    rationalite.setValue(50);
    rationalite.setOrientation(Qt::Horizontal);

    QLabel *valRationalite = new QLabel(QString::number(50));

    connect(&rationalite, SIGNAL(valueChanged(int)), valRationalite, SLOT(setNum(int)));

    layoutRationalite->addWidget(&rationalite);
    layoutRationalite->addWidget(valRationalite);


    // Agressivité
    QHBoxLayout *layoutAgressivite = new QHBoxLayout;

    agressivite.setRange(0, 100);
    agressivite.setValue(50);
    agressivite.setOrientation(Qt::Horizontal);

    QLabel *valAgressivite = new QLabel(QString::number(50));

    connect(&agressivite, SIGNAL(valueChanged(int)), valAgressivite, SLOT(setNum(int)));

    layoutAgressivite->addWidget(&agressivite);
    layoutAgressivite->addWidget(valAgressivite);

    layout->addLayout(layoutRationalite);
    layout->addLayout(layoutAgressivite);

    setLayout(layout);
}

Profil ChoixCalibrage::getCalibrage() const
{
    Profil p;

    p.setAgressivite(agressivite.value());
    p.setRationalite(rationalite.value());

    return p;
}
