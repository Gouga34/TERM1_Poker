/*========================================================================
Nom: IntelligenceArtificielleProfilage.h       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015          Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe d'une IA qui profile.
=========================================================================*/

#ifndef ARTIFICIALINTELLIGENCEPROFILING_H
#define ARTIFICIALINTELLIGENCEPROFILING_H

#include "ArtificialIntelligence.h"
#include "../Profiling/Profiling.h"
#include "../Profiling/TestScenario.h"


class ArtificialIntelligenceProfiling : public ArtificialIntelligence
{
    private:

        PHASE_JEU_IA m_gamePhase;

        profiling::Profile m_playerProfile;
        profiling::Profiling *m_profiling;
        profiling::TestScenarios *m_scenario;

        /**
         * @brief Détermine la phase de jeu pour la prochaine partie (profilage, gains)
         * @return Nouvelle phase de jeu
         */
        virtual PHASE_JEU_IA getNextGameType();

        /**
         * @brief Modifie le calibrage de l'ia pour profiler
         */
        virtual void setProfilingCalibration();

        /**
         * @brief Modifie la calibrage de l'ia pour gagner
         */
        virtual void setCalibrationToPlay();

        /**
         * @action Calcule les données globales de profilage et sauvegarde les données de la partie
         */
        virtual void calculatePlayerGlobalProfile();

        /**
         * @action Ecrit les résultats des scénarios de tests pour la partie courante
         */
        virtual void writeScenariosTestsResults();

    public:

        ArtificialIntelligenceProfiling(bool isDealer, int tokens, int position);
        ArtificialIntelligenceProfiling(const ArtificialIntelligence& player);
        virtual ~ArtificialIntelligenceProfiling();

        /**
         * @brief getScenario
         * @return copie de scenarios de tests
         */
        virtual profiling::TestScenarios getScenario() const;

        /**
         * @return Profilage du joueur par l'IA
         */
        virtual profiling::Profiling* getProfiling() const;

        /**
         * @action Modifie le pseudo du joueur profilé
         * @param pseudo Pseudo du joueur
         */
        virtual void setProfiledPlayerNickname(std::string nickname);

        /**
         * @action Remplit l'objet contenant les données de profilage du joueur
         */
        virtual void fillProfilingData();

        /**
         * @action Ecrit les résultats de la partie dans le csv et dans les scénarios de tests
         *          si l'ia est en phase de profilage
         */
        virtual void writeGameResult();

        /**
         * @action Change le calibrage de l'IA en fonction de la phase de jeu (profilage/gains)
         */
        virtual void determineGameType();

        /**
         * @action Ajoute une ligne d'analyse des résultats des gains pour le calibrage courant
         */
        virtual void writeEarningsAnalysis();
};

#endif // ARTIFICIALINTELLIGENCEPROFILING_H
