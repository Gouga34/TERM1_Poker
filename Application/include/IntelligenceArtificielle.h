#ifndef IntelligenceArtificielle_h
#define IntelligenceArtificielle_h

#include "Joueur.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class IntelligenceArtificielle : public Joueur {

	//Attributs
	private:
		std::vector<Carte> table;
		double calculProba();	
	
	//Constructeur et destructeur
	public :
		IntelligenceArtificielle(bool estDealer, int jetons);
		~IntelligenceArtificielle();
		
	//Mutateur
		void setTable(std::vector<Carte> tab);
		
	//Methodes
		void jouer();		
	
};

#endif
