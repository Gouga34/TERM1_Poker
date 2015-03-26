/*========================================================================
Nom: CalculDonneesProfilage.h       Auteur: Morgane Vidal
Maj: 26/03/2015          Creation: 25/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe permettant de calculer les données de profilage.
=========================================================================*/

#ifndef CALCUL_DONNEES_PROFILAGE_H
#define CALCUL_DONNEES_PROFILAGE_H

class CalculDonneesProfilage
{

public:

    /*
     * @param chancesGain pourcentage de chances de gagner du joueur
     * @param totalMises total des mises effectuées par le joueur. Il est exprimé en pourcentage par rapport aux jetons du joueur.
     * @return retourne le pourcentage de rationalité du joueur. Plus ce pourcentage est proche de 100, plus le joueur est rationnel.
    */
    static double rationalite(const double chancesGain, const double totalMises);

    /*
     * @param miseLaPlusHaute mise la plus haute effectuée. Il est exprimé en pourcentage par rapport aux jetons du joueur au départ
     * @param tauxMises pourcentage du nombre de mises effectuées
     * @param totalMises total des mises effectuées par le joueur. Il est exprimé en pourcentage par rapport aux jetons du joueur.
     * @return le pourcentage d'agressivité du joueur calculé en fonction des données fournies en paramètre
    */
    static double agressivite(const double miseLaPlusHaute, const double tauxMises, const double totalMises);

    /*
     * @param rationalite pourcentage de rationalité du joueur.
     * @return le pourcentage de bluff du joueur, calculé en fonction des données fournies en paramètre
    */
    static double bluff(const double rationalite);

    /*
     * @param tauxSuivis pourcentage du nombre de suivis effectués
     * @param tauxChecks pourcentage du nompbre de checks effectués
     * @return le pourcentage de passivité du joueur.
    */
    static double passivite(const double tauxSuivis, const double tauxChecks);

    /*
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
// CALCUL_DONNEES_PROFILAGE_H
