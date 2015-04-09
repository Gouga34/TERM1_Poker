#ifndef Joueur_h
#define Joueur_h

#include <vector>
#include "Carte.h"
#include "Action.h"

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
        int                 miseCourante;
        int                 misePlusHaute;
        int                 miseTotale;
        double              chancesGain;
        int                 misePartie;

		
	//Constructeur et destructeur
	public:
	
		/**
        *@action : Cree un nouveau joueur
		*@param  : Un boolean indiquant si le joueur est le dealer virtuel
        *@param  : Le nombre de jetons dont il dispose au debut de la partie
        *@param  : Sa position
		**/
        Joueur(bool estDealer, int jetons, int position);
		
		/**
		*@action : Destructeur de la classe Joueur
		**/
        virtual ~Joueur();
		
	//Accesseur
	
        /**
         * @brief getChancesGain
         * @return les chances de gain du joueur
         */
        double getChancesGain()const;

        /**
         * @brief setChancesGain
         * @param nvChancesGain les nouvelles chances de gain du joueur
         */
        void setChancesGain(double nvChancesGain);

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
         * @action Permet d'obtenir la mise courante du joueur
         * @return Mise courante du joueur
         */
        int         getMiseCourante() const;

        /**
         * @action Modifie la mise du joueur
         * @param jetons Mise du joueur
         */
        void        setMiseCourante(int jetons);
		
		/**
		*@action : Permet d'obtenir la mise du joueur
		*@return : La mise du joueur
		**/		
        int			getMisePlusHaute() const;
		
		/**
		*@action : Modifie la mise du joueur
		*@param :  La nouvelle mise
		**/		
        void			setMisePlusHaute(int jetons);


        /**
        *@action : Permet d'obtenir le compteur des actions du joueur
        *@return : Les compteurs d'actions du joueur
        **/
        int*            getCompteurActions();

        /**
        *@action : Permet d'obtenir combien a miser au total le joueur pendant l'etape
        *@return : La somme totale misee par le joueur
        **/
        int             getMiseTotale();

        /**
        *@action : Permet d''affecter combien a miser au total le joueur pendant l'etape
        *@param : La somme totale misee par le joueur
        **/
        void             setMiseTotale(int miseTotale);

        /**
        *@action : Permet d'obtenir la mise totale de la partie
        *@return : La somme totale misee par le joueur au cours de la partie
        **/
        int             getMisePartie();

        /**
        *@action : Permet d'affecter le total mise par le joueur
        *@param : La somme totale misee par le joueur au cours de la partie
        **/
        void            setMisePartie (int mise);

		
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

        /**
         * @action Fait jouer le joueur lorsque c'est son tour
         */
        virtual Action jouer();
};

#endif
