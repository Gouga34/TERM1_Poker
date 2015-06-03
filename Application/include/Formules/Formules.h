#ifndef FORMULES_H
#define FORMULES_H

class Formules{

public:
    /**
     * @return le taux de valeur par rapport à la valeur de référence.
     * Exemple : 20 sur 180 donne 11.11%
     * /!\ retourne -1 sur valeurReference == 0
    **/
    static double taux(const double valeur, const double valeurReference);

    /**
     * @return la variation entre deux valeurs, en pourcentage.
     * exemple : variation entre 100 et 50 donne -50%
     *
     **/
    static double variation(const double valeur1, const double valeur2);

    /**
     * @return le pourcentage de la valeur.
     * 	Exemple : 20% de 550 donne 110%
    **/
    static double pourcentageValeur(const double pourcentage, const double valeur);

    /**
     *
     *@return valeur proportionnelle
    **/
    static double calculValeurProportionnelle(double minVal1, double val1, double maxVal1, double minVal2, double maxVal2);


};

#endif
