/*========================================================================
Nom: IntelligenceArtificielleProfilage.h       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015          Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe d'une IA qui profile.
=========================================================================*/

#ifndef INTELLIGENCEARTIFICIELLEPROFILAGE_H
#define INTELLIGENCEARTIFICIELLEPROFILAGE_H

#include "ArtificialIntelligence.h"
#include "../Profiling/Profiling.h"
#include "../Profiling/TestScenario.h"


class IntelligenceArtificielleProfilage : public ArtificialIntelligence
{
    private:

        PHASE_JEU_IA phaseJeu;

        profiling::Profile profilJoueur;

        profiling::Profiling *profilage;

        profiling::TestScenarios *scenario;


        double calculValeurProportionnelle(double minVal1, double val1, double maxVal1, double minVal2, double maxVal2) const;


        /**
         * @brief Détermine la phase de jeu pour la prochaine partie (profilage, gains)
         * @return Nouvelle phase de jeu
         */
        PHASE_JEU_IA prochainTypeDeJeu();

        /**
         * @brief Modifie le calibrage de l'ia pour profiler
         */
        void setCalibragePourProfiler();

        /**
         * @brief Modifie la calibrage de l'ia pour gagner
         */
        void setCalibragePourJouer();

        /**
         * @action Calcule les données globales de profilage et sauvegarde les données de la partie
         */
        void calculProfilGlobalJoueur();

        /**
         * @action Ecrit les résultats des scénarios de tests pour la partie courante
         */
        void ecritureScenariosDeTests();

    public:

        IntelligenceArtificielleProfilage(bool estDealer, int jetons, int position);
        IntelligenceArtificielleProfilage(const ArtificialIntelligence& joueur);
        virtual ~IntelligenceArtificielleProfilage();

        /**
         * @brief getScenario
         * @return copie de scenarios de tests
         */
        profiling::TestScenarios getScenario() const;

        /**
         * @return Profilage du joueur par l'IA
         */
        profiling::Profiling* getProfilage() const;

        /**
         * @action Modifie le pseudo du joueur profilé
         * @param pseudo Pseudo du joueur
         */
        void setPseudoJoueurProfile(std::string pseudo);

        /**
         * @action Remplit l'objet contenant les données de profilage du joueur
         */
        void remplissageDonneesProfilage();

        /**
         * @action Ecrit les résultats de la partie dans le csv et dans les scénarios de tests
         *          si l'ia est en phase de profilage
         */
        void ecritureResultatsPartie();

        /**
         * @action Change le calibrage de l'IA en fonction de la phase de jeu (profilage/gains)
         */
        void determinerTypeDeJeu();

        /**
         * @action Ajoute une ligne d'analyse des résultats des gains pour le calibrage courant
         */
        void ecritureAnalyseDesGains();
};

#endif // INTELLIGENCEARTIFICIELLEPROFILAGE_H
