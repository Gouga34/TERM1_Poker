#ifndef IntelligenceArtificielle_h
#define IntelligenceArtificielle_h

#include "Joueur.h"
#include "Jeu.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class IntelligenceArtificielle : public Joueur {

	private:
		//Attributs
		std::vector<Carte> table;
		
		//Méthodes
		
		/**
		*@action : Permet d'obtenir la probabilité pre-Flop
		*@return : La probabilitee pre-Flop de la main courant
		**/
		double calculProba();	
	
	
	public :
		//Constructeur et destructeur
		
		/**
		*@action : Constructeur de la classe Intelligence Artificielle
		*@param  : Un boolean indiquant si le jouer est le dealer, sa cave de depart et sa position
		**/
		IntelligenceArtificielle(bool estDealer, int jetons, int position);
		
		/**
		*@action : Constructeur de la classe Intelligence Artificielle par copie de la classe Joueur
		*@param  : L'instance de la classe Joueur utilisee pour la copie
		**/		
		IntelligenceArtificielle(Joueur joueur);
		
		/**
		*@action : Destructeur de la classe Intelligence Artificielle
		**/
		~IntelligenceArtificielle();
		
		//Mutateur
		
		/**
		*@action : Permet d'obtenir les carte communes
		*@param  : Un ensemble de carte representant les cartes communes
		**/
		void setTable(std::vector<Carte> tab);
		
		//Methodes
	
		/**
		*@action : Pemet a l'IA de jouer
		**/
        void jouer();
	
};

#endif
