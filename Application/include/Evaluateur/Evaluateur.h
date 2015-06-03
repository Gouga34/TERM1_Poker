/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 04/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe correspondant l'évaluateur de main, compare 
				deux mains et donne celle ayant le poids le plus fort
=========================================================================*/

#ifndef EVALUATEUR_H
#define EVALUATEUR_H

#include "CartesJoueur.h"
#include "../Jeu/Carte.h"
#include <iostream>
#include <vector>

class Evaluateur{
	public:
		/**
		* @action compare les deux mains pour voir laquelle a le poids le plus fort
		* @return 0 si main1 a gagné, 1 si elle a perdu et 2 si égalité
		*/
        static RESULTAT_PARTIE comparerMains(std::vector<Carte> table, std::vector<Carte> mainJoueur1, std::vector<Carte> mainJoueur2);

};

#endif
