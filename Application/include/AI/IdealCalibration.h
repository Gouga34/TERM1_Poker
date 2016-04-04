/*========================================================================
Nom: CalibrageIdeal.h       Auteur: Manuel CHATAIGNER
Maj: 07/05/2015          Creation: 07/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe déterminant le calibrage optimal pour gagner
                face à un profilage donné.
=========================================================================*/

#ifndef IDEALCALIBRATION_H
#define IDEALCALIBRATION_H

#include <QVector>
#include "../Profiling/Profile.h"
#include "../Jeu/Jeu.h"

namespace ai {

    class IdealCalibration
    {
        private:

            Jeu *m_game;

            profiling::Profile *m_profilingAICalibration;
            profiling::Profile *m_profiledAICalibration;

            int m_partsNumber;
            int m_numberPartsWon;
            int m_totalTokensWon;

            QVector<int> m_profilingAIRates;


            /**
             * @brief Ajout une ligne dans le fichier avec les résultats
             *        du calibrage donné face au calibrage testé.
             */
            virtual void writeTotalPartsResult() const;

        public:

            IdealCalibration(Jeu *j, profiling::Profile *iaQuiProfile, profiling::Profile *iaProfilee, int nbParties);
            virtual ~IdealCalibration();

            /**
             * @brief Effectue nombreParties pour chaque calibrage possible face au calibrage adverse
             */
            virtual void launchParts();
    };
}

#endif // IDEALCALIBRATION_H
