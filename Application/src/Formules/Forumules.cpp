#include "../../include/Formules/Formules.h"


double Formules::taux(const double valeur, const double valeurReference){
    if(valeurReference==0){
        return -1;
    }
    else{
        return ((valeur*100.0)/valeurReference);
    }
}

double Formules::variation(const double valeur1, const double valeur2){
    if(valeur1==0){
        return -1;
    }
    else{
        return (valeur2-valeur1)/valeur1*100;
    }
}

double Formules::pourcentageValeur(const double pourcentage, const double valeur){
    return (valeur*pourcentage)/100;
}

double Formules::calculValeurProportionnelle(double minVal1, double val1, double maxVal1, double minVal2, double maxVal2) {
    return ((val1 - minVal1) * ((maxVal2 - minVal2) / (maxVal1 - minVal1)) + minVal2);
}

