#ifndef Joueur_h
#define Joueur_h

#include <vector>
#include "Carte.h"

class Jeu;

class Joueur{

	//Attributs
	protected:
		bool 			dealer;
		std::vector<Carte>	main;
		int			cave;
		Jeu*			jeu;
		int			position;
		int			miseJoueur;
		
	//Constructeur et destructeur
	public:
		Joueur(bool estDealer, int jetons, int position);
		~Joueur();
		
	//Accesseur
		int  			getCave() const;
		std::vector<Carte>	getMain() const;
		void 			setJeu(Jeu* j);
		Jeu*			getJeu();
		int			getPosition() const;
		int			getMiseJoueur();
		void			setMiseJoueur(int jetons);
		
	//Methodes
		void ajouteJetons(int jetons);
		void retireJetons(int jetons);
		void ajouteCarte(Carte carte);
		bool estDealer();
		void resetMain();
		void executeCommande();
		void changeDealer();
		void videMain();
		
};

#endif
