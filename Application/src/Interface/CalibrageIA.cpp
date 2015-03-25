/*========================================================================
Nom: CalibrageIA.cpp       Auteur: Manuel CHATAIGNER
Maj: 25/03/2015            Creation: 25/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe CalibrageIA.
=========================================================================*/

#include "../../include/Interface/CalibrageIA.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>


CalibrageIA::CalibrageIA() : QDialog()
{
    QFormLayout *layout = new QFormLayout;

    /** Rationnalité **/

    QHBoxLayout *layoutRationalite = new QHBoxLayout;

    rationalite.setRange(0, 100);
    rationalite.setValue(50);
    rationalite.setOrientation(Qt::Horizontal);

    QLabel *valRationalite = new QLabel("50");

    connect(&rationalite, SIGNAL(valueChanged(int)), valRationalite, SLOT(setNum(int)));

    layoutRationalite->addWidget(&rationalite);
    layoutRationalite->addWidget(valRationalite);


    /** Agressivité **/

    QHBoxLayout *layoutAgressivite = new QHBoxLayout;

    agressivite.setRange(0, 100);
    agressivite.setValue(50);
    agressivite.setOrientation(Qt::Horizontal);

    QLabel *valAgressivite = new QLabel("50");

    connect(&agressivite, SIGNAL(valueChanged(int)), valAgressivite, SLOT(setNum(int)));

    layoutAgressivite->addWidget(&agressivite);
    layoutAgressivite->addWidget(valAgressivite);



    QDialogButtonBox *boutonDialog = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout->addRow("Rationalité", layoutRationalite);
    layout->addRow("Agressivité", layoutAgressivite);
    layout->addWidget(boutonDialog);

    setLayout(layout);

    connect(boutonDialog, SIGNAL(accepted()), this, SLOT(accept()));
    connect(boutonDialog, SIGNAL(rejected()), this, SLOT(reject()));
}

std::pair<double, double> CalibrageIA::getCalibrage()
{
    if (exec() == QDialog::Accepted) {
        return std::make_pair(rationalite.value(), agressivite.value());
    }
    else {
        return std::make_pair(50.0, 50.0);
    }
}
