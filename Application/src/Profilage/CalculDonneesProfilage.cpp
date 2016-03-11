/*========================================================================
Nom: CalculDonneesProfilage.h       Auteur: Morgane Vidal
Maj: 25/03/2015          Creation: 25/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de 
                                        la classe CalculDonneesProfilage.
=========================================================================*/

#include "../../include/Profilage/CalculDonneesProfilage.h"
#include "../../include/Constantes.h"
#include "../../include/MathematicalFormula/MathematicalFormula.h"

#include <cmath>

double CalculDonneesProfilage::calculateTheoreticalBet(const double maxGain, const double minGain, const double winningChances, const double minTheoreticalBet, const double maxTheoreticalBet) {
    double dividend=(maxGain-minGain);
    if (dividend != 0) {
        double theoreticalBet = mathematicalformula::MathematicalFormula::calculateProportionalValue(minGain, winningChances, maxGain, minTheoreticalBet, maxTheoreticalBet);
        return theoreticalBet;
    }
    else {
        return -1;
    }
}

double CalculDonneesProfilage::miseTheorique(const double chancesGain) {
    //Variable qui contiendront les données des paliers : 
    double gainMin, gainMax, miseTheoriqueMin, miseTheoriqueMax;

    //On regarde si le pourcentage de chances de gain est dans le premier palier : 
    if(chancesGain >= RATIONALITE::PALIER1::DEBUT_GAIN
            && chancesGain <= RATIONALITE::PALIER1::FIN_GAIN) {
        gainMin = RATIONALITE::PALIER1::DEBUT_GAIN;
        gainMax = RATIONALITE::PALIER1::FIN_GAIN;
        miseTheoriqueMin = RATIONALITE::PALIER1::DEBUT_MISE_THEORIQUE;
        miseTheoriqueMax = RATIONALITE::PALIER1::FIN_MISE_THEORIQUE;
    }
    //chancesGains palier 2
    else if(chancesGain > RATIONALITE::PALIER1::FIN_GAIN
            && chancesGain <= RATIONALITE::PALIER2::FIN_GAIN) {
        gainMin = RATIONALITE::PALIER2::DEBUT_GAIN;
        gainMax = RATIONALITE::PALIER2::FIN_GAIN;
        miseTheoriqueMin = RATIONALITE::PALIER2::DEBUT_MISE_THEORIQUE;
        miseTheoriqueMax = RATIONALITE::PALIER2::FIN_MISE_THEORIQUE;
    }
    //chancesGains palier 3
    else if(chancesGain > RATIONALITE::PALIER2::FIN_GAIN
            && chancesGain <= RATIONALITE::PALIER3::FIN_GAIN) {
        gainMin = RATIONALITE::PALIER3::DEBUT_GAIN;
        gainMax = RATIONALITE::PALIER3::FIN_GAIN;
        miseTheoriqueMin = RATIONALITE::PALIER3::DEBUT_MISE_THEORIQUE;
        miseTheoriqueMax = RATIONALITE::PALIER3::FIN_MISE_THEORIQUE;
    }
    //chancesGains palier 4
    else if(chancesGain > RATIONALITE::PALIER3::FIN_GAIN
            && chancesGain <= RATIONALITE::PALIER4::FIN_GAIN) {
        gainMin = RATIONALITE::PALIER4::DEBUT_GAIN;
        gainMax = RATIONALITE::PALIER4::FIN_GAIN;
        miseTheoriqueMin = RATIONALITE::PALIER4::DEBUT_MISE_THEORIQUE;
        miseTheoriqueMax = RATIONALITE::PALIER4::FIN_MISE_THEORIQUE;
    }

    return CalculDonneesProfilage::calculateTheoreticalBet(gainMax, gainMin, chancesGain, miseTheoriqueMin, miseTheoriqueMax);
}

double CalculDonneesProfilage::rationalite(const double chancesGain, const double totalMises) {
    double miseTh = miseTheorique(chancesGain);

    if (miseTh != 0) {
        return 100 - abs(miseTh - totalMises);
    }
    else {
        return -1;
    }
}

double CalculDonneesProfilage::calculateAgressiveness(const double betsRatio, const double rateBets, const double rateHighestBet,
                                                      const double highestBet, const double maxAgressiveness, const double totalBets) {
    double x = betsRatio * rateBets + rateHighestBet * highestBet;
    double y = x * ((maxAgressiveness - totalBets) / 100.0);
    double agressiveness = totalBets + y;

    return agressiveness;
}

double CalculDonneesProfilage::agressivite(const double miseLaPlusHaute, const double tauxMises, const double totalMises) {
    double totalMisesMin, totalMisesMax, agressiviteMin, agressiviteMax;
    double ratioMises, ratioMisePlusHaute;

    //miseLaPlusHaute dans PALIER1 ?
    if (totalMises >= AGRESSIVITE::PALIER1::DEBUT_MISE_TOTALE
            && totalMises <= AGRESSIVITE::PALIER1::FIN_MISE_TOTALE) {
        
        totalMisesMin = AGRESSIVITE::PALIER1::DEBUT_MISE_TOTALE;
        totalMisesMax = AGRESSIVITE::PALIER1::FIN_MISE_TOTALE;
        
        agressiviteMin = AGRESSIVITE::PALIER1::DEBUT_AG_THEORIQUE;
        agressiviteMax = AGRESSIVITE::PALIER1::FIN_AG_THEORIQUE;

        ratioMises = static_cast<double>(AGRESSIVITE::PALIER1::RATIO_NB_MISES_DIVISE) / AGRESSIVITE::PALIER1::RATIO_NB_MISES_DIVISEUR;
        ratioMisePlusHaute = static_cast<double>(AGRESSIVITE::PALIER1::RATIO_MPH_DIVISE) / AGRESSIVITE::PALIER1::RATIO_MPH_DIVISEUR;
    }
    //miseLaPlusHaute dans PALIER2 ?
    else if(totalMises > AGRESSIVITE::PALIER1::FIN_MISE_TOTALE
            && totalMises <= AGRESSIVITE::PALIER2::FIN_MISE_TOTALE) {
        
        totalMisesMin = AGRESSIVITE::PALIER2::DEBUT_MISE_TOTALE;
        totalMisesMax = AGRESSIVITE::PALIER2::FIN_MISE_TOTALE;
        
        agressiviteMin = AGRESSIVITE::PALIER2::DEBUT_AG_THEORIQUE;
        agressiviteMax = AGRESSIVITE::PALIER2::FIN_AG_THEORIQUE;

        ratioMises = static_cast<double>(AGRESSIVITE::PALIER2::RATIO_NB_MISES_DIVISE) / AGRESSIVITE::PALIER2::RATIO_NB_MISES_DIVISEUR;
        ratioMisePlusHaute = static_cast<double>(AGRESSIVITE::PALIER2::RATIO_MPH_DIVISE) / AGRESSIVITE::PALIER2::RATIO_MPH_DIVISEUR;
    }
    //miseLaPlusHaute dans PALIER3 ?
    else if(totalMises > AGRESSIVITE::PALIER2::FIN_MISE_TOTALE
            && totalMises < AGRESSIVITE::PALIER3::FIN_MISE_TOTALE) {
        
        totalMisesMin = AGRESSIVITE::PALIER3::DEBUT_MISE_TOTALE;
        totalMisesMax = AGRESSIVITE::PALIER3::FIN_MISE_TOTALE;
        
        agressiviteMin = AGRESSIVITE::PALIER3::DEBUT_AG_THEORIQUE;
        agressiviteMax = AGRESSIVITE::PALIER3::FIN_AG_THEORIQUE;

        ratioMises = static_cast<double>(AGRESSIVITE::PALIER3::RATIO_NB_MISES_DIVISE) / AGRESSIVITE::PALIER3::RATIO_NB_MISES_DIVISEUR;
        ratioMisePlusHaute = static_cast<double>(AGRESSIVITE::PALIER3::RATIO_MPH_DIVISE) / AGRESSIVITE::PALIER3::RATIO_MPH_DIVISEUR;
    }
    //tapis
    else {
        return 100.0;
    }

     return CalculDonneesProfilage::calculateAgressiveness(ratioMises, tauxMises, ratioMisePlusHaute,
                                                           miseLaPlusHaute, agressiviteMax, totalMises);
}

double CalculDonneesProfilage::bluff(const double rationalite) {
    return (100.0 - rationalite);
}

double CalculDonneesProfilage::passivite(const double tauxSuivis, const double tauxChecks) {
    return (tauxChecks + tauxSuivis);
}

double CalculDonneesProfilage::taux(const double valeur, const double valeurReference) {
    if (valeurReference == 0) {
        return -1;
    }
    else {
        return ((valeur * 100.0) / valeurReference);
    }   
}
