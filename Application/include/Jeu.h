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
        EN_ATTENTE,
        PETITE_BLIND,
        GROSSE_BLIND
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
		/**
		*@param  : Le nombre de joueur, le montant de la blind de depart, la cave de depart des joueurs et le type de proffiling de l'IA
		*@action : Initialise un nouveau jeu
		**/
		Jeu(int nbJoueur, int blindDepart, int cave, int typeIA);
		
		/**
		*@action : Destructeur de la classe Jeu
		**/
		~Jeu();
		
	//Accesseur
	
		/**
		*@action : Permet d'obtenir le montant de la petite blind
		*@return : Le montant de la petite blind
		**/
		int 			getBlind() const;
		
		/**
		*@action : Permet d'obtenir le joueur devant jouer
		*@return : Le joueur courant
		**/
		int 			getJoueurCourant() const;
		
		/**
		*@action : Permet d'obtenir le joueur en i-eme position
		*@return : Le joueur en i-eme position
		**/
        	Joueur& 		getJoueur(int i);
        	
        	/**
		*@action : Permet d'ajouter un joueur a la partie
		*@param  : Le joueur a ajouter a la partie
		**/
		void 			setJoueur(Joueur joueur);
		
		/**
		*@action : Permet d'obtenir les carte communes
		*@return : L'ensemble des cartes commune
		**/
		std::vector<Carte>	getTable() const;
		
		/**
		*@action : Permet d'obtenir le pot de la partie
		*@return : Le pot de la partie en cours
		**/
		int			getPot() const;
		
		/**
		*@action : Permet de modifier le pot de la partie courante
		*@param  : Un entier representant la nouvelle valeur du pot
		**/
		void			setPot(int jetons);
		
		/**
		*@action  : Permet de connaitre l'action effectué par le joueur courant
		*@return  : L'action effectué par le joueur courant
		**/
		TYPES::ACTION_LIST	getAction() const;
		
		/**
		*@action : Permet d'obtenir la position du dealer
		*@return : La position du dealer
		**/
		int 			getDealer();
		
		/**
		*@action : Permet d'obtenir la mise de la partie courante
		*@return : Un entier représentant la mise courante
		**/
		int			getMise();

	
	//Methodes	
	//private:
		/**
		*@action : Distribue a chaque joueur ses cartes
		**/
		void distributionMain();
		
		/**
		*@action : Distribue les trois premieres carte commune : le Flop, tirees aleatoirement dans le deck
		**/
		void distributionFlop();
		
		/**
		*@action : Distribue la quatrieme carte : le Turn, tiree aleatoirement dans le deck
		**/
		void distributionTurn();
		
		/**
		*@action : Distribue la cinquieme carte : la River, tiree aleatoirement dans le deck
		**/
		void distributionRiver();
		
		/**
		*@action : Distribue les blinds en debut de partie
		**/
		void distributionBlind();
		
		/**
		*@action : Augmente le montant de la petite blind
		**/
		void miseAJourBlind();
		
		/**
		*@action : Melange le jeu de carte
		**/
		void melange();
		
		/**
		*@param  : Le nombre de joueur et le montant de depart d leur cave
		*@action : Cree les joueurs et les affectent au jeu
		*@return : L'ensemble des joueurs de la partie
		**/
		void initialisationTable(int nbJoueur, int cave);
		
		/**
		*@action : Cree l'ensemble des cartes utilisees dans le jeu
		*@return : Le "deck", l'ensemble du jeu de carte
		**/
		std::vector<Carte> nouveauDeck();
		
		/**
		*@action : Permet de savoir si le joueur a la possibilite checker
		*@return : vrai si le joueur peut checker, faux sinon 
		**/
		bool peutChecker();
		
		/**
		*@action : Commande permettant a un joueur de miser
		*@param  : Le joueur effectuant l'action ainsi que le montant de la mise
		**/
		void miser(int posJoueur, int jetons);
		
		/**
		*@action : Commande permettant a un joueur de relancer
		*@param  : Le joueur effectuant l'action ainsi que le montant de la relance
		**/
		void relancer(int posJoueur, int jetons);
		
		/**
		*@action : Commande permettant a un joueur de suivre
		*@param  : Le joueur effectuant l'action
		**/
		void suivre(int posJoueur);
		
		/**
		*@action : Commande permettant a un joueur de checker
		*@param  : Le joueur effectuant l'action
		**/
		void checker(int posJoueur);
		
		/**
		*@action : Commande permettant a un joueur de se coucher
		*@param  : Le joueur effectuant l'action
		**/
       		void seCoucher(int posJoueur);
       		
       		/**
		*@action  : Affecte le joueur suivant en tant que joueur courant et renvoi vrai s'il existe
		*@return : vrai s'il y a un joueur suivant, faux sinon
		**/
		bool prochainJoueur();
		
		/**
		*@action  : Permet de savoir si l'on est en début de tour
		*@return  : Vrai si l'on se trouve en debut de tour, faux sinon
		**/
		bool debutTour();
		
	
		/**
		*@action  : Permet de savoit si le tour est terminé
		*@return  : Vrai si le tour est termnié, faux sinon
		**/
		bool finDuTour();
		
		/**
		*@action  : Permet de reinitialiser le tableau des actions
		**/		
		void resetActions();
		
		/**
		*@action  : Relance une nouvelle main
		**/			
		void nouvelleMain(int posJoueur);
		
	
		
			
};

#endif
