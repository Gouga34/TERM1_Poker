#ifndef CALCULPROFILAGE_H
#define CALCULPROFILAGE_H


/**
*@action : Permet de recuperer les bornes des paliers de la rationalite en fonction de la probabilite de gain
*@param  : La probabilite de gain
*@return : Un vecteur contenant : debut_gain, fin_gain, debut_ra_theorique, fin_ra_theorique
**/
std::vector<int> getBornesRationalite(double estimation);

/**
*@action : Permet de calculer la mise theorique en fonction de la probabilite de gain
*@param  : La probabilite de gain
*@return : La mise theorique
**/
double calculMiseTheorique(double estimation);


#endif // CALCULPROFILAGE_H
