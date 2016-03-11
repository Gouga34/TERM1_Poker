/*========================================================================
Nom: CalculDonneesProfilage.h       Auteur: Morgane Vidal
Maj: 26/03/2015          Creation: 25/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe permettant de calculer les données de profilage.
=========================================================================*/

#ifndef CALCULATE_PROFILING_DATA_H
#define CALCULATE_PROFILING_DATA_H

class CalculateProfilingData
{

private:

    /**
     * @brief calculateAggressiveness
     * @param betsRatio
     * @param rateBets
     * @param rateHighestBet
     * @param highestBet
     * @param maxAggressiveness
     * @param totalBets
     * @return l'agressivité
     */
    static double calculateAggressiveness(const double betsRatio, const double rateBets, const double ratioHighestBet,
                                         const double highestBet, const double maxAggressiveness, const double totalBets);

    /**
     * @brief calculateTheoreticalBet
     * @param maxGain
     * @param minGain
     * @param winningChances
     * @param minTheoreticalBet
     * @param maxTheoreticalBet
     * @return la mise théorique
     */
    static double calculateTheoreticalBet(const double maxGain, const double minGain, const double winningChances, const double minTheoreticalBet, const double maxTheoreticalBet);
public:

    /**
     * @param winningChances pourcentage de chances de gain du joueur
     * @return la mise théorique qui devrait être faite.
    */
    static double theoreticalBet(const double winningChances);

    /**
     * @param winningChances pourcentage de chances de gagner du joueur
     * @param totalBets total des mises effectuées par le joueur. Il est exprimé en pourcentage par rapport aux jetons du joueur.
     * @return retourne le pourcentage de rationalité du joueur. Plus ce pourcentage est proche de 100, plus le joueur est rationnel.
    */
    static double rationality(const double winningChances, const double totalBets);

    /**
     * @param highestBet mise la plus haute effectuée. Il est exprimé en pourcentage par rapport aux jetons du joueur au départ
     * @param rateBets pourcentage du nombre de mises effectuées
     * @param totalBets total des mises effectuées par le joueur. Il est exprimé en pourcentage par rapport aux jetons du joueur.
     * @return le pourcentage d'agressivité du joueur calculé en fonction des données fournies en paramètre
    */
    static double aggressiveness(const double highestBet, const double rateBets, const double totalBets);

    /**
     * @param rationality pourcentage de rationalité du joueur.
     * @return le pourcentage de bluff du joueur, calculé en fonction des données fournies en paramètre
    */
    static double bluff(const double rationality);

    /**
     * @param rateCall pourcentage du nombre de suivis effectués
     * @param rateChecks pourcentage du nompbre de checks effectués
     * @return le pourcentage de passivité du joueur.
    */
    static double passivity(const double rateCall, const double rateChecks);

    /**
     * @param valeur valeur à comparer à la valeur de référence
     * @param valeurReference valeur de référence
     * @return le pourcentage des actions effectuées en fonction de nombreTours
     *          Sera utilisé pour les calculs de :
     *              - suivis,
     *              - mises,
     *              - miseLaPlusHaute,
     *              - miseTotaleIA,
     *              - miseTotaleJoueur,
     *              - pot.
    */
    static double taux(const double valeur, const double valeurReference);
};

#endif 
// CALCULATE_PROFILING_DATA_H
