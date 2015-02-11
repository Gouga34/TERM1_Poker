#ifndef Jeu_h
#define Jeu_h

#include <vector>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include "../include/Carte.h"
#include "../include/Joueur.h"


class Jeu{

	//Liste des attributs
	private :
		std::vector<Joueur> 	positionnement;
		std::vector<Carte>    	deck;
		std::vector<Carte> 	table;
		int 			blind;
		int 			joueurCourant;
		int 			pot;
		int 			nombreDeCoup;
		
		
	//Constructeur et destructeur
	public:
		Jeu(int nbJoueur, int blindDepart, int cave, int typeIA);
		~Jeu();
		
	//Accesseur
		int getBlind() const;
		int getJoueurCourant() const;
	
	//Methodes	
	private:
		void distributionMain();
		void distributionFlop();
		void distributionTurn();
		void distributionRiver();
		void miseAJourBlind();
		void melange();
		void initialisationTable(int nbJoueur, int cave);
		std::vector<Carte> nouveauDeck();
		
			
};

#endif
