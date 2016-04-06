/*========================================================================
Nom: ChoixCalibrage.cpp       Auteur: Manuel CHATAIGNER
Maj: 24/05/2015            Creation: 24/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ChoixCalibrage.
=========================================================================*/


#include "../../include/Interface/CalibrationChoice.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>


CalibrationChoice::CalibrationChoice(const QString& title, QWidget *parent) : QGroupBox(title, parent)
{
    setCheckable(true);
    setChecked(false);

    QFormLayout *layout = new QFormLayout;

    // Rationalité
    QHBoxLayout *rationalityLayout = new QHBoxLayout;

    m_rationality.setRange(0, 100);
    m_rationality.setValue(50);
    m_rationality.setOrientation(Qt::Horizontal);

    QLabel *rationalityValue = new QLabel(QString::number(50));

    connect(&m_rationality, SIGNAL(valueChanged(int)), rationalityValue, SLOT(setNum(int)));

    rationalityLayout->addWidget(&m_rationality);
    rationalityLayout->addWidget(rationalityValue);


    // Agressivité
    QHBoxLayout *aggressivenessLayout = new QHBoxLayout;

    m_aggressiveness.setRange(0, 100);
    m_aggressiveness.setValue(50);
    m_aggressiveness.setOrientation(Qt::Horizontal);

    QLabel *aggressivenessValue = new QLabel(QString::number(50));

    connect(&m_aggressiveness, SIGNAL(valueChanged(int)), aggressivenessValue, SLOT(setNum(int)));

    aggressivenessLayout->addWidget(&m_aggressiveness);
    aggressivenessLayout->addWidget(aggressivenessValue);

    layout->addRow("Agressivité", aggressivenessLayout);
    layout->addRow("Rationalite", rationalityLayout);

    setLayout(layout);
}

CalibrationChoice::~CalibrationChoice()
{

}

profiling::Profile CalibrationChoice::getCalibration() const
{
    profiling::Profile profile;

    profile.setAggressiveness(m_aggressiveness.value());
    profile.setRationality(m_rationality.value());

    return profile;
}
