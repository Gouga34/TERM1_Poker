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
#include "../Game/Game.h"

namespace ai {

    class IdealCalibration
    {
        private:

            game::Game *m_game;

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

            IdealCalibration(game::Game *game, profiling::Profile *profilingAI, profiling::Profile *profiledAI, int nbParts);
            virtual ~IdealCalibration();

            /**
             * @brief Effectue nombreParties pour chaque calibrage possible face au calibrage adverse
             */
            virtual void launchParts();
    };
}

#endif // IDEALCALIBRATION_H
