#ifndef Jeu_h
#define Jeu_h

#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include "Carte.h"
#include "../Constantes.h"
#include "../Evaluateur/Evaluateur.h"
#include "../Profilage/CalculDonneesProfilage.h"
#include "../Profilage/Profilage.h"
#include "../IA/EstimationProba.h"
#include "../IA/IntelligenceArtificielle.h"

class Joueur;
class IntelligenceArtificielle;

class Jeu{

	//Liste des attributs
	private :
        std::vector<Joueur*>                listeJoueurs;
        std::vector<Carte>                  deck;
        std::vector<Carte>                  table;
        std::vector<std::vector<ACTION>>    actions;        // Liste d'actions par joueur
        int                                 blind;
        int                                 joueurCourant;
        int                                 pot;
        int                                 nombreDeCoup;

        /** Correspond aux jetons mis en jeu à un instant t de la partie par le joueur
         *  précédent celui qui doit jouer **/
        int                                 miseCourante;

        /** Correspond à la somme des mises et relances d'une étape. C'est la somme des
         *  jetons qu'un joueur doit mettre pour rester en jeu **/
        int                                 cumulMisesEtRelances;
        int                                 dealer;
        bool                                partieFinie;
        ETAPE_JEU                           etape;
        int                                 resultatPartie;
        std::vector<Carte>                  tableTmp;



        /**
         * @brief Méthode qui permet à un joueur de jouer de l'argent (mise, relancer, suivi...)
         *        setMiseCourante, setMiseTotale, setMisePlusHaute (avec vérif),
         *        Incrémentation pot, Décrémentation cave
         * @param posJoueur Position du joueur
         * @param jetons Nombre de jetons à jouer
         */
        void jouerArgent(int posJoueur, int jetons);

        /**
        *@action : Commande permettant a un joueur de miser
        *@param  : La position du joueur effectuant l'action ainsi que le montant de la mise
        **/
        void miser(int posJoueur, int jetons);

        /**
        *@action : Commande permettant a un joueur de relancer
        *@param  : La position du joueur effectuant l'action ainsi que le montant de la relance
        **/
        void relancer(int posJoueur, int jetons);

        /**
        *@action : Commande permettant a un joueur de faire "tapis"
        *@param  : La position du joueur effectuant l'action
        * @param action effetuée (mise, relance, suivi)
        **/
        void tapis(int posJoueur, ACTION action);

        /**
        *@action : Commande permettant a un joueur de suivre
        *@param  : La position du joueur effectuant l'action
        **/
        void suivre(int posJoueur);

        /**
        *@action : Commande permettant a un joueur de checker
        *@param  : La position du joueur effectuant l'action
        **/
        void checker(int posJoueur);

        /**
        *@action : Commande permettant a un joueur de se coucher
        *@param  : Le joueur effectuant l'action
        **/
        void seCoucher(int posJoueur);
		
	//Constructeur et destructeur
	public:
		/**
		*@param  : Le nombre de joueur, le montant de la blind de depart, la cave de depart des joueurs et le type de proffiling de l'IA
		*@action : Initialise un nouveau jeu
		**/
        Jeu(int nbJoueur, int blindDepart, int cave);
		
		/**
		*@action : Destructeur de la classe Jeu
		**/
		~Jeu();
		
    //Accesseurs
	
        /**
         * @return Gagnant de la partie (GAGNE, PERDU, EGALITE)
         */
        int             getResultatPartie() const;

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
         * @brief Détermine la position du joueur adverse au joueur passé en paramètre
         * @param joueur Position du joueur dont on veut l'adversaire
         * @return Position du joueur adverse
         */
        int             getPositionJoueurAdverse(int joueur) const;
		
		/**
		*@action : Permet d'obtenir le joueur en i-eme position
		*@return : Le joueur en i-eme position
		**/
        Joueur* 		getJoueur(int i);
        	
        	/**
		*@action : Permet d'ajouter un joueur a la partie
		*@param  : Le joueur a ajouter a la partie
		**/
        void 			setJoueur(Joueur *joueur);
		
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
            * @action Permet de connaitre la dernière action effectué par le joueur passé en paramètre
            * @param posJoueur Joueur dont on veut connaitre l'action
            * @return L'action effectué par le joueur courant
		**/
        ACTION          getLastAction(int posJoueur) const;
		
		/**
		*@action : Permet d'obtenir la position du dealer
		*@return : La position du dealer
		**/
		int 			getDealer();
		
		/**
        *@action : Permet d'obtenir la mise courante du tour de table
		*@return : Un entier représentant la mise courante
		**/
        int			getMiseCourante();

        /**
         * @brief getCumulMisesEtRelances
         * @return cumulMisesEtRelances
         */
        int         getCumulMisesEtRelances();

		/**
		*@action : Permet d'obtenir le deck
		*@return : Un vecteur représentant le deck
		**/
		std::vector<Carte>			getDeck();
		
		/**
		*@action : Permet d'obtenir l'ensemble des actions
        *@param posJoueur Position du joueur dont on veut la liste
		*@return : L'ensemble des actions
		**/
        std::vector<ACTION> getListeActions(int posJoueur) const;

        /**
        *@action : Permet d'obtenir l'etape courante
        *@return : L'etape courante
        **/
        ETAPE_JEU getEtape() const;


	
	//Methodes	

        void addJoueur(Joueur *joueur);

        /**
		*@action : Distribue a chaque joueur ses cartes
		**/
		void distributionMain();
		
        /**
         * @action Distribue les nouvelles cartes de l'étape passée en paramètre
         * @param etape Nouvelle étape courante
         */
        void nouvelleEtape(ETAPE_JEU etape);

        /**
         * @brief distributionCartes distribue nbCartesADistribuer sur la table
         * @param nbCartesADistribuer
         */
        void distributionCartesTable(int nbCartesADistribuer);
		
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
		*@param  : la position du joueur dont on veut savoir s'il peut checker
		*@return : vrai si le joueur peut checker, faux sinon 
		**/
		bool peutChecker(int posJoueur);

		/**
		*@action : Permet de savoir si le joueur a la possibilite de relancer
        *@param  : la position du joueur dont on veut savoir s'il peut relancer et montant de la relance
		*@return : vrai si le joueur peut relancer, faux sinon
		**/
        bool peutRelancer(int posJoueur, int jetons);

		/**
		*@action : Permet de savoir si le joueur a la possibilite de miser
        *@param  : la position du joueur dont on veut savoir s'il peut miser et montant de la mise
		*@return : vrai si le joueur peut miser, faux sinon
		**/
        bool peutMiser(int posJoueur, int jetons);


		/**
		*@action : Permet de savoir si le joueur a la possibilite de suivre
		*@param  : la position du joueur dont on veut savoir s'il peut suivre
		*@return : vrai si le joueur peut suivre, faux sinon
		**/
		bool peutSuivre(int posJoueur);

        /**
         * @param posJoueur Position du joueur à tester
         * @return Vrai si le joueur est couché
         */
        bool estCouche(int posJoueur) const;

        /**
         * @action Exécute l'action du joueur (check, mise...)
         * @param posJoueur Position du joueur qui exécute l'action
         * @param action Action à exécuter
         * @param mise Montant des jetons associés à l'action (-1 si check, se couche, suivre)
         */
        void executerAction(int posJoueur, Action a);
       		
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
         * @action Effectue l'ensemble des actions de fin de partie
         */
        void finPartie();
		
		/**
        *@action  : Relance une nouvelle main
		**/			
        void nouvelleMain();

		/**
		*@action  : Affecte les cartes choisies via l'interface
		*@param   : Un vecteur d'entier correspondant a la liste des ids des cartes
		**/
		void affectationCarte(std::vector<int> listeId);

        /**
         * @action Démarre la boucle de jeu principale
         */
        void lancer();

        /**
        *@action  : Permet de savoir si un Joueur a fait Tapis
        *@return  : Vrai si un joueur a fait tapis, faux sinon
        **/
        bool aFaitTapis();

        bool peutJouer(int posJoueur);

};

#endif
