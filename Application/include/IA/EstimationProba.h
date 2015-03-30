#ifndef EstimationProba_h
#define EstimationProba_h

#include "../Jeu/Jeu.h"
#include "../Jeu/Joueur.h"
#include "../Evaluateur/Evaluateur.h"

class EstimationProba{

    public:
	
		/**
		*@action : Creer un nouveau deck prive des carte du joueur courant
		*@return  : Le deck des cartes restantes
		**/
       static  std::vector<Carte> nouveauDeck(Joueur* joueurCourant);
		
		/**
		*@action : Melange le jeu de carte
		*@param  : Le jeu a melanger
		**/
        static void melange(std::vector<Carte>& deck);
		
		
		/**
		*@action : Ordonne la table par ordre decroissant
		*@param  : L'ensemble de carte representant la table a ordonner
		**/
        static void rangeTable(std::vector<Carte>& table);
		
		
		/**
		*@action : Permet de connaitre la probabilitee de gagner a un instant 't' pour le joueur courant
		*@return  : La probabilite de gagner
		**/
        static double estimation(Jeu* jeuCourant, Joueur* joueurCourant);
};


#endif
