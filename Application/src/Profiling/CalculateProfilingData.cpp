/*========================================================================
Nom: CalculDonneesProfilage.h       Auteur: Morgane Vidal
Maj: 25/03/2015          Creation: 25/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de 
                                        la classe CalculDonneesProfilage.
=========================================================================*/

#include "../../include/Profiling/CalculateProfilingData.h"
#include "../../include/Constants.h"
#include "../../include/MathematicalFormula/MathematicalFormula.h"

#include <cmath>

namespace profiling {



    double CalculateProfilingData::calculateTheoreticalBet(const double maxGain, const double minGain, const double winningChances, const double minTheoreticalBet, const double maxTheoreticalBet) {
        double dividend=(maxGain-minGain);
        if (dividend != 0) {
            double theoreticalBet = mathematicalformula::MathematicalFormula::calculateProportionalValue(minGain, winningChances, maxGain, minTheoreticalBet, maxTheoreticalBet);
            return theoreticalBet;
        }
        else {
            return -1;
        }
    }

    double CalculateProfilingData::theoreticalBet(const double winningChances) {
        //Variable qui contiendront les données des paliers :
        double minGain, maxGain, minTheoreticalBet, maxTheoreticalBet;

        //On regarde si le pourcentage de chances de gain est dans le premier palier :
        if(winningChances >= RATIONALITY::PALIER1::DEBUT_GAIN
                && winningChances <= RATIONALITY::PALIER1::FIN_GAIN) {
            minGain = RATIONALITY::PALIER1::DEBUT_GAIN;
            maxGain = RATIONALITY::PALIER1::FIN_GAIN;
            minTheoreticalBet = RATIONALITY::PALIER1::DEBUT_MISE_THEORIQUE;
            maxTheoreticalBet = RATIONALITY::PALIER1::FIN_MISE_THEORIQUE;
        }
        //chancesGains palier 2
        else if(winningChances > RATIONALITY::PALIER1::FIN_GAIN
                && winningChances <= RATIONALITY::PALIER2::FIN_GAIN) {
            minGain = RATIONALITY::PALIER2::DEBUT_GAIN;
            maxGain = RATIONALITY::PALIER2::FIN_GAIN;
            minTheoreticalBet = RATIONALITY::PALIER2::DEBUT_MISE_THEORIQUE;
            maxTheoreticalBet = RATIONALITY::PALIER2::FIN_MISE_THEORIQUE;
        }
        //chancesGains palier 3
        else if(winningChances > RATIONALITY::PALIER2::FIN_GAIN
                && winningChances <= RATIONALITY::PALIER3::FIN_GAIN) {
            minGain = RATIONALITY::PALIER3::DEBUT_GAIN;
            maxGain = RATIONALITY::PALIER3::FIN_GAIN;
            minTheoreticalBet = RATIONALITY::PALIER3::DEBUT_MISE_THEORIQUE;
            maxTheoreticalBet = RATIONALITY::PALIER3::FIN_MISE_THEORIQUE;
        }
        //chancesGains palier 4
        else if(winningChances > RATIONALITY::PALIER3::FIN_GAIN
                && winningChances <= RATIONALITY::PALIER4::FIN_GAIN) {
            minGain = RATIONALITY::PALIER4::DEBUT_GAIN;
            maxGain = RATIONALITY::PALIER4::FIN_GAIN;
            minTheoreticalBet = RATIONALITY::PALIER4::DEBUT_MISE_THEORIQUE;
            maxTheoreticalBet = RATIONALITY::PALIER4::FIN_MISE_THEORIQUE;
        }

        return CalculateProfilingData::calculateTheoreticalBet(maxGain, minGain, winningChances, minTheoreticalBet, maxTheoreticalBet);
    }

    double CalculateProfilingData::rationality(const double winningChances, const double totalBets) {
        double thereoticalBet = theoreticalBet(winningChances);

        if (thereoticalBet != 0) {
            return 100 - abs(thereoticalBet - totalBets);
        }
        else {
            return -1;
        }
    }

    double CalculateProfilingData::calculateAggressiveness(const double betsRatio, const double rateBets, const double ratioHighestBet,
                                                          const double highestBet, const double maxAggressiveness, const double totalBets) {
        double x = betsRatio * rateBets + ratioHighestBet * highestBet;
        double y = x * ((maxAggressiveness - totalBets) / 100.0);
        double agressiveness = totalBets + y;

        return agressiveness;
    }

    double CalculateProfilingData::aggressiveness(const double highestBet, const double rateBets, const double totalBets) {
        double maxAggressiveness;
        double betsRatio, ratioHighestBet;

        //miseLaPlusHaute dans PALIER1 ?
        if (totalBets >= AGGRESSIVENESS::PALIER1::DEBUT_MISE_TOTALE
                && totalBets <= AGGRESSIVENESS::PALIER1::FIN_MISE_TOTALE) {

            maxAggressiveness = AGGRESSIVENESS::PALIER1::FIN_AG_THEORIQUE;

            betsRatio = static_cast<double>(AGGRESSIVENESS::PALIER1::RATIO_NB_MISES_DIVISE) / AGGRESSIVENESS::PALIER1::RATIO_NB_MISES_DIVISEUR;
            ratioHighestBet = static_cast<double>(AGGRESSIVENESS::PALIER1::RATIO_MPH_DIVISE) / AGGRESSIVENESS::PALIER1::RATIO_MPH_DIVISEUR;
        }
        //miseLaPlusHaute dans PALIER2 ?
        else if(totalBets > AGGRESSIVENESS::PALIER1::FIN_MISE_TOTALE
                && totalBets <= AGGRESSIVENESS::PALIER2::FIN_MISE_TOTALE) {

            maxAggressiveness = AGGRESSIVENESS::PALIER2::FIN_AG_THEORIQUE;

            betsRatio = static_cast<double>(AGGRESSIVENESS::PALIER2::RATIO_NB_MISES_DIVISE) / AGGRESSIVENESS::PALIER2::RATIO_NB_MISES_DIVISEUR;
            ratioHighestBet = static_cast<double>(AGGRESSIVENESS::PALIER2::RATIO_MPH_DIVISE) / AGGRESSIVENESS::PALIER2::RATIO_MPH_DIVISEUR;
        }
        //miseLaPlusHaute dans PALIER3 ?
        else if(totalBets > AGGRESSIVENESS::PALIER2::FIN_MISE_TOTALE
                && totalBets < AGGRESSIVENESS::PALIER3::FIN_MISE_TOTALE) {

            maxAggressiveness = AGGRESSIVENESS::PALIER3::FIN_AG_THEORIQUE;

            betsRatio = static_cast<double>(AGGRESSIVENESS::PALIER3::RATIO_NB_MISES_DIVISE) / AGGRESSIVENESS::PALIER3::RATIO_NB_MISES_DIVISEUR;
            ratioHighestBet = static_cast<double>(AGGRESSIVENESS::PALIER3::RATIO_MPH_DIVISE) / AGGRESSIVENESS::PALIER3::RATIO_MPH_DIVISEUR;
        }
        //tapis
        else {
            return 100.0;
        }

         return CalculateProfilingData::calculateAggressiveness(betsRatio, rateBets, ratioHighestBet,
                                                               highestBet, maxAggressiveness, totalBets);
    }

    double CalculateProfilingData::bluff(const double rationality) {
        return (100.0 - rationality);
    }

    double CalculateProfilingData::passivity(const double rateCall, const double rateChecks) {
        return (rateChecks + rateCall);
    }

    double CalculateProfilingData::taux(const double valeur, const double valeurReference) {
        if (valeurReference == 0) {
            return -1;
        }
        else {
            return ((valeur * 100.0) / valeurReference);
        }
    }
}
