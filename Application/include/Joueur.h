#ifndef Joueur_h
#define Joueur_h

#include <vector>
#include "../include/Carte.h"

class Joueur{

	//Attributs
	protected:
		bool 			dealer;
		std::vector<Carte>	main;
		int			cave;
		
	//Constructeur et destructeur
	public:
		Joueur(bool estDealer, int jetons);
		~Joueur();
		
	//Accesseur
		int  			getCave() const;
		std::vector<Carte>	getMain() const;
		
	//Methodes
		void ajouteJetons(int jetons);
		void retireJetons(int jetons);
		void ajouteCarte(Carte carte);
		bool estDealer();
		void resetMain();
		
};

#endif
