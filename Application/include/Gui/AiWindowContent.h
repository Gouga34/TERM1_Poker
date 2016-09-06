/*========================================================================
Nom: ContenuFenetreIA.h       Auteur: Morgane VIDAL
Maj: 22/05/2015          Creation: 22/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe définissant la fenêtre affichée lorsque
               ce sont deux intelligences artificielles qui jouent.
=========================================================================*/

#ifndef AIWINDOWCONTENT_H
#define AIWINDOWCONTENT_H

#include "WindowContent.h"
#include "Window.h"
#include <QLabel>
#include <QVector>
#include <QTableWidget>
#include "CustomPlot/qcustomplot.h"

namespace gui
{
    class AiWindowContent : public WindowContent
    {
        private:

            QLabel *m_partsState;
            QTableWidget *m_partsSummary;

            QTableWidget *m_globalResults;

            QLabel *m_profiledAiCalibration;

            QCustomPlot *m_resultsGraph;

            QVector<double> m_partsNumberPoints;            // Nombre de parties
            QVector<double> m_similarityRatePoints;         // Taux de similarité
            QVector<double> m_winningsPluralityPoints;      // Cumul de gains


            enum PartsSummaryColumns
            {
                CURRENT_SCENARIOS,
                PROFILING_AI_AGGRESSIVENESS,
                PROFILING_AI_RATIONALITY,
                DEDUCTED_AGGRESSIVENESS,
                DEDUCTED_RATIONALITY,
                SIMILARITY_RATE,
                //AGGRESSIVE_PLAY,
                WINNINGS,
                PROFILING_AI_WINNINGS
            };

            enum GlobalResultsColumns
            {
                DEDUCTED_AG,
                DEDUCTED_RA,
                SIMILARITY,
                //AGGRESSIVE_PLAY_NB,
                TOTAL_WINNINGS,
                NB_WON_GAMES
            };


            /**
             * @brief updateGlobalResults maj les résultats globaux de la partie
             */
            virtual void updateGlobalResults();

            /**
             * @brief updateProfiledAiCalibration maj l'affichage du calibrage de l'ia profilée
             */
            virtual void updateProfiledAiCalibration();

            /**
             * @brief initGlobalResults initialise les données des résultats globaux à 0.
             */
            virtual void initGlobalResults();

            /**
             * @brief initPartsSummary initialise le tableau récapitulatif des parties
             */
            virtual void initPartsSummary();

            /**
             * @brief initResults initialise le graphique des résultats
             */
            virtual void initResults();

            /**
             * @brief ajouterLigne ajoute une ligne dans le récapitulatif des parties
             */
            virtual void addLine();

            /**
             * @brief Scroll automatiquement vers le bas pour afficher la denrière ligne du tableau
             */
            virtual void autoTabScroll();

            /**
             * @brief changeBackgroundDuringProfiling Change la couleur de fond pendant le profilage
             */
            virtual void changeBackgroundDuringProfiling();

            /**
             * @brief updateResultsGraph Ajoute les points de la nouvelle partie
             */
            virtual void updateResultsGraph();

        public:

            AiWindowContent(game::Game *game, Window *window);
            virtual ~AiWindowContent();

            /**
             * @brief actualiser actualise le récapitulatif des parties.
             */
            virtual void refresh();
    };
}

#endif // AIWINDOWCONTENT_H
