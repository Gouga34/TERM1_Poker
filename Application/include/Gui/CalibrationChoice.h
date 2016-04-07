/*========================================================================
Nom: ChoixCalibrage.h       Auteur: Morgane Vidal
Maj: 11/03/2015          Creation: 11/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe contenant les widgets .
=========================================================================*/

#ifndef CALIBRATIONCHOICE_H
#define CALIBRATIONCHOICE_H

#include <QWidget>
#include <QSlider>
#include <QString>
#include <QGroupBox>
#include "../Profiling/Profile.h"

namespace gui
{
    class CalibrationChoice : public QGroupBox
    {
    private:

        QSlider m_rationality;
        QSlider m_aggressiveness;

    public:

        CalibrationChoice(const QString& title, QWidget *parent = 0);
        virtual ~CalibrationChoice();

        /**
             * @brief getCalibrage
             * @return Calibrage des sliders
             */
        virtual profiling::Profile getCalibration() const;
    };
}

#endif // CALIBRATIONCHOICE_H
