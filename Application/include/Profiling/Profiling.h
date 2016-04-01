/*========================================================================
Nom: Profilage.h       Auteur: Manuel CHATAIGNER
Maj: 04/03/2015          Creation: 04/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe contenant l'écriture et la lecture
               des données de profilage.
=========================================================================*/


#ifndef PROFILAGE_H
#define PROFILAGE_H

#include <string>
#include <array>
#include <QString>
#include "../Constantes.h"
#include "Profile.h"
#include "GamePhase.h"

namespace profiling {

    class Profiling
    {
        private:

            Profile *m_aiProfile;

        public:

            Profile *m_playerProfile;

            RESULTAT_PARTIE m_gameResultProfilingAIviewpoint;
            int m_numberTokensWonProfilingAI;
            bool m_testScenario;
            bool m_roughPlay;

            struct StepPart
            {
                bool isFolded;                    // vrai si le joueur s'est couché

                double adversaryWinningProbabilities;     // % de chance de gagner du joueur (-1 si cartes inconnues)

                /* Type du joueur (somme des taux = 100%) */
                double aggressivenessRate;         // Agressivité du joueur (mise la plus haute, mise totale, nb checks)
                double rationalityRate;        // Rationalite du joueur (mise totale, s'est couché ?)
                double bluffRate;               // Bluff du joueur (mise totale, s'est couché ?)
                double passivityRate;           // Passivite du joueur (suivis, checks)

                /* Taux en % des différentes actions effectuées par le joueur (les relances sont incluses dans les mises) */
                double callRate;
                double checkRate;
                double betRate;

                /* Taux en % des mises en fonction des jetons du joueur */
                double highestBet;           // Mise plus haute du joueur
                double totalBet;        // Mise totale du joueur

                int numberOfActions;
            };

            // Une ligne par étape + 1 ligne pour le global
            std::array<StepPart, ETAPE_JEU::NB_ETAPES + 1> m_stepParts;


            // Données des phases de profilage et de gains en cours
            GamePhase m_currentProfilingPhase;
            GamePhase m_currentGamePhase;


            // Données globales au profilage
            int m_partsNumber;
            int m_numberProfilingGames;
            int m_numberProfilingGamesWon;

            int m_numberGameGamesWon;
            int m_numberGamesGame;

            int m_profilingEarnings;
            int m_gameEarnings;


            Profiling(Profile *aiCalibration, Profile *profile);
            virtual ~Profiling();

            /**
             * @action Efface les données de profilage de la partie enregistrée
             */
            virtual void reset();

            /**
             * @action Ajoute les données de profilage de la partie dans le fichier correspondant au joueur
             */
            virtual void save();
    };
}

#endif // PROFILAGE_H
