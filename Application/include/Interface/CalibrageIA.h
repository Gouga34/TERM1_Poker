/*========================================================================
Nom: CalibrageIA.h       Auteur: Manuel CHATAIGNER
Maj: 25/03/2015          Creation: 25/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre du calibrage de l'IA.
=========================================================================*/

#ifndef CALIBRAGEIA_H
#define CALIBRAGEIA_H

#include <QDialog>
#include <QSlider>

class CalibrageIA : public QDialog
{
    private:

        QSlider rationalite;
        QSlider agressivite;

    public:

        CalibrageIA();

        /**
         * @return Valeurs de rationalité et d'agressivité pour le jeu de l'IA
         */
        std::pair<double, double> getCalibrage();
};

#endif // CALIBRAGEIA_H
