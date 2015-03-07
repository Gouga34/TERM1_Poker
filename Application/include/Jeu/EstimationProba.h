#ifndef EstimationProba_h
#define EstimationProba_h

#include "Jeu.h"
#include "Joueur.h"

class EstimationProba{

	//Attributs
	private :
		Joueur* joueurCourant;
		Jeu*	jeuCourant;
		
		
	//Methodes
	
		/**
		*@action : Creer un nouveau deck prive des carte du joueur courant
		*@return  : Le deck des cartes restantes
		**/
		std::vector<Carte> nouveauDeck();
		
		/**
		*@action : Melange le jeu de carte
		*@param  : Le jeu a melanger
		**/
		void melange(std::vector<Carte>& deck);
		
	//Constructeur et destructeur
	public:
		/**
		*@action : Constructeur de la classe estimationProba
		*@param  : Un pointeur sur le jeu courant ainsi qu'un pointeur sur le joueur dont on veut connaitre la probabilité de gagner
		**/
		EstimationProba(Jeu* jeu, Joueur* joueur);
		
		/**
		*@action : Destructeur de la classe estimationProba
		**/
		~EstimationProba();
		
		/**
		*@action : Ordonne la table par ordre decroissant
		*@param  : L'ensemble de carte representant la table a ordonner
		**/
		void rangeTable(std::vector<Carte>& table);
		
		
	//Methodes
		
		/**
		*@action : Permet de connaitre la probabilitee de gagner a un instant 't' pour le joueur courant
		*@return  : La probabilite de gagner
		**/
		double estimation();
		
	


};


#endif
