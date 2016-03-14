/*========================================================================
Nom: ChoixCalibrage.h       Auteur: Morgane Vidal
Maj: 11/03/2015          Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe contenant les widgets .
=========================================================================*/

#ifndef CHOIXCALIBRAGE_H
#define CHOIXCALIBRAGE_H

#include <QWidget>
#include <QSlider>
#include <QString>
#include <QGroupBox>
#include "../Profilage/Profile.h"

class ChoixCalibrage : public QGroupBox
{
    private:

        QSlider rationalite;
        QSlider agressivite;

    public:

        ChoixCalibrage(const QString& titre, QWidget *parent = 0);

        /**
         * @brief getCalibrage
         * @return Calibrage des sliders
         */
        Profile getCalibrage() const;
};

#endif // CHOIXCALIBRAGE_H
