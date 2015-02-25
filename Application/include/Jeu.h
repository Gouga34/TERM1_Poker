#ifndef Jeu_h
#define Jeu_h

#include <vector>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include "Carte.h"
#include "Joueur.h"
#include "IntelligenceArtificielle.h"

class Joueur;
class IntelligenceArtificielle;

namespace TYPES
{
    enum ACTION_LIST
    {
        MISER,
        RELANCER,
        SUIVRE,
        CHECKER,
        SE_COUCHER,
        EN_ATTENTE
    };
};


class Jeu{

	//Liste des attributs
	private :
		std::vector<Joueur> 			positionnement;
		std::vector<Carte>    			deck;
		std::vector<Carte> 			table;
		std::vector<TYPES::ACTION_LIST>		actions;
		int 					blind;
		int 					joueurCourant;
		int 					pot;
		int 					nombreDeCoup;
		int 					mise;
		int					dealer;
		
	//Constructeur et destructeur
	public:
		Jeu(int nbJoueur, int blindDepart, int cave, int typeIA);
		~Jeu();
		
	//Accesseur
		int 			getBlind() const;
		int 			getJoueurCourant() const;
        	Joueur& 		getJoueur(int i);
		void 			setJoueur(Joueur joueur);
		std::vector<Carte>	getTable() const;
		int			getPot() const;
		void			setPot(int jetons);
		TYPES::ACTION_LIST	getAction() const;
		int 			getDealer();
		int			getMise();

	
	//Methodes	
	//private:
		void distributionMain();
		void distributionFlop();
		void distributionTurn();
		void distributionRiver();
		void distributionBlind();
		void miseAJourBlind();
		void melange();
		void initialisationTable(int nbJoueur, int cave);
		std::vector<Carte> nouveauDeck();
		bool peutChecker();
		
		void miser(int posJoueur, int jetons);
		void relancer(int posJoueur, int jetons);
		void suivre(int posJoueur);
		void checker(int posJoueur);
       		void seCoucher(int posJoueur);
        bool prochainJoueur();
		bool debutTour();
		bool finDuTour();
		void resetActions();
		
	
		
			
};

#endif
