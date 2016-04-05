/*========================================================================
Nom: Resolveur.h         Auteur: Morgane Vidal
Maj: 31/03/2015          Creation: 31/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe permettant de calculer les actions de
                                             l'intelligence artificielle.
=========================================================================*/

#ifndef RESOLVER_H
#define RESOLVER_H

#include <utility>
#include "../Game/Game.h"
#include "ArtificialIntelligence.h"
#include "../Profiling/Profile.h"
#include <cstdlib>

namespace ai {

    class ArtificialIntelligence;

    class Resolver {

    private:

        bool m_roughtPlay;
        profiling::Profile *m_calibration;
        double m_aggressivenessTheoreticalTotalBet;

        ArtificialIntelligence *m_ai;

        /**
         * @action calcule l'action à effectuer en fonction du taux d'agressivité
         * @return l'action à effectuer et le montant de sa mise s'il y en a un.
         *          Ce montant est égal à -1 si pas de mise.
        */
        virtual game::Action calculateRoughtAction();

        /**
         * @brief calculateRationalAction
         * @action calcule l'action à effectuer en fonction du taux de rationalité
         * @return l'action à effectuer et le montant de sa mise s'il y en a un.
         *          Ce montant est égal à -1 si pas de mise.
         */
        virtual game::Action calculateRationalAction();

        /**
         * @brief calculateAggressivenessTheoreticalTotalBet
         * @action calcule la mise totale théorique de la partie en fonction de l'agressivité
         */
        virtual void calculateAggressivenessTheoreticalTotalBet();

        /**
         * @brief calculateRationalityBet
         * @param action l'action à effectuer
         * @return les jetons à jouer en fonction de l'action qui sera effectuée
         *          et en fonction du taux de rationalité
         */
        virtual int calculateRationalityBet(ACTION action);

    public:
        Resolver(ArtificialIntelligence* ai);
        virtual ~Resolver();

        /**
         * @brief isAggressive
         * @return true si le joueur a joué plus agressif
         */
        bool isAggressive() const;

        /**
         * @brief setRoughtGame Modifie le choix de l'action pour que le résolveur privilégie ou non l'agressivité.
         * @param ag true si action agressive prioritaire
         */
        void setRoughtGame(bool roughtPlay);

        /**
        * @brief setCalibration
        * @param profil nouveau calibrage de l'IA
        */
       void setCalibration(profiling::Profile profile);

        /**
         * @brief getCalibration
         * @return le taux de rationalité
         */
        profiling::Profile* getCalibration();

        /**
         * @brief calculateAction
         * @return l'action finale et sa possible mise en nombre de jetons
         */
        game::Action calculateAction();
    };
}

#endif // RESOLVER_H
