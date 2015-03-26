#ifndef Joueur_h
#define Joueur_h

#include <vector>
#include "Carte.h"

class Jeu;

class Joueur{

	//Attributs
	protected:
        bool                dealer;
		std::vector<Carte>	main;
        int*                compteurActions;
        int                 cave;
        Jeu*                jeu;
        int                 position;
        int                 miseJoueur;
		
	//Constructeur et destructeur
	public:
	
		/**
		*@param  : Un boolean indiquant si le joueur est le dealer virtuel
		*		ainsi que le nombre de jetons dont il dispose au
		*		debut de la partie
		*@action : Cree un nouveau joueur
		**/
		Joueur(bool estDealer, int jetons, int position);
		
		/**
		*@action : Destructeur de la classe Joueur
		**/
		~Joueur();
		
	//Accesseur
	
		/**
		*@action : Permet d'obtenir le nombre de jeton dont dispose un joueur
		*@return : La cave d'un joueur
		**/
		int  			getCave() const;	
		
		/**
		*@action : Permet d'obtenir la main du joueur
		*@return : La main du joueur
		**/
		std::vector<Carte>	getMain() const;
		
		/**
		*@action : Permet de relier le jeu et le joueur
		*@param  : L'instance du jeu
		**/		
		void 			setJeu(Jeu* j);
		
		/**
		*@action : Permet d'obtenir un pointeur sur l'instance du jeu courant
		*@return : Un pointeur sur l'instance de Jeu
		**/
		Jeu*			getJeu();
		
		/**
		*@action : Permet d'obtenir la position du joueur
		*@return : La position du joueur
		**/
		int			getPosition() const;
		
		/**
		*@action : Permet d'obtenir la mise du joueur
		*@return : La mise du joueur
		**/		
		int			getMiseJoueur();
		
		/**
		*@action : Modifie la mise du joueur
		*@param :  La nouvelle mise
		**/		
		void			setMiseJoueur(int jetons);


        /**
        *@action : Permet d'obtenir le compteur des actions du joueur
        *@return : Les compteurs d'actions du joueur
        **/
        int*            getCompteurActions();
		
	//Methodes
	
		/**
		*@param  : Le nombre de jetons a ajouter
		*@action : Ajoute des jetons a la cave d'un joueur
		**/
		void ajouteJetons(int jetons);
		
		/**
		*@param  : Le nombre de jetons a retirer
		*@action : Retire des jetons a la cave d'un joueur
		**/
		void retireJetons(int jetons);
		
		/**
		*@param  : La carte a ajouter a la main du joueur
		*@action : Ajoute une carte a la main du joueur
		**/
		void ajouteCarte(Carte carte);
		
		/**
		*@action : Permet de savoir si un joueur est le dealer virtuel
		*@return : Un boolean indiquant si le joueur est considere comme le 
		*		dealer
		**/
		bool estDealer();
		
		/**
		*@action : Reinitialise la main du joueur
		**/
		void resetMain();
		
		/**
		*@action : Modifie l'attribut dealer du joueur
		**/		
		void changeDealer();
		
		/**
		*@action : Vide la main du joueur
		**/		
		void videMain();

        /**
        *@action : Remet a zero les compteurs comptabilisant les actions du joueur
        **/
        void resetCompteurActions();
		
};

#endif
