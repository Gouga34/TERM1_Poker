#ifndef MATHEMATICALFORMULA_H
#define MATHEMATICALFORMULA_H

namespace mathematicalformula {

    class MathematicalFormula {

    public:
        /**
         * @return le taux de valeur par rapport à la valeur de référence.
         * Exemple : 20 sur 180 donne 11.11%
         * /!\ retourne -1 sur valeurReference == 0
        **/
        static double rate(const double value, const double referenceValue);

        /**
         * @return la variation entre deux valeurs, en pourcentage.
         * exemple : variation entre 100 et 50 donne -50%
         *
         **/
        static double variation(const double value1, const double value2);

        /**
         * @return le pourcentage de la valeur.
         * 	Exemple : 20% de 550 donne 110%
        **/
        static double valuePercentage(const double percentage, const double value);

        /**
         *
         *@return valeur proportionnelle
        **/
        static double calculateProportionalValue(const double minVal1, const double val1, const double maxVal1, const double minVal2, const double maxVal2);
    };
}

#endif
