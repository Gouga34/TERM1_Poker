/*========================================================================
Nom: MainJoueur.h       Auteur: Morgane Vidal
Maj: 04/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe correspondant à la main d'un joueur, 
				avec ses cartes et celles du milieu
=========================================================================*/

#ifndef MAINJOUEUR_H
#define MAINJOUEUR_H

#include "../Jeu/Carte.h"
#include <iostream>
#include <vector>

class CartesJoueur{

	private: 
		/** 
			Les quatre premières lignes correspondent aux quatre couleurs,
			La dernière ligne au nombre d'occurences de chaque carte, toutes couleurs confondues,
			Les 14 premières colonnes correspondent aux cartes (de 1 à AS, en comptant l'AS deux fois)
			La dernière colonne correspond au nombre de cartes d'une même couleurs.
		*/
		int occurrences[15][5];

		//Combinaison la plus forte de la main du joueur.
		int combinaison; 

		//Poids de la main
		int poids;

		/**
		 * @param combi combinaison
		 * @action modifie la combinaison avec une nouvelle combinaison
		 */
		void setCombinaison(int combi);

		/**
		 * @param p poids
		 * @action modifie le poids avec un nouveau poids
		 */
		void setPoids(int p);

        /**
         * @brief initialiserOccurrences initialise le tableau des occurrences à 0.
         */
        void initialiserOccurrences();

		/**
		 * @param table cartes de la table
		 * @param mainJoueur cartes du joueur
	     * @action rempli occurrences
	     */
		void remplirTableau(std::vector<Carte> table, std::vector<Carte> mainJoueur);

        /**
         * @brief enregistrerOccurrencesCartes enregistre les occurrences des cartes dans le tableau occurrences.
         * @param listeCartes
         */
        void enregistrerOccurrencesCartes(std::vector<Carte> listeCartes);


		/**
		 * @param depart case à partir de laquelle on va commencer
		 * @param ligne ligne que l'on va traiter
		 * @action regarde si on a une suite sur la ligne, à partir du départ (suite de 5 cartes minimum)
		 * @return true si on a une suite, false sinon
		 */
		bool estSuite(int depart, int ligne) const;
 
        /**
		 * @param ligne ligne qui sera traitée
		 * @return true si la ligne contient une suite
		 */
		bool contientSuite(int ligne);

		/**
		 * @param nb nombre de cartes identiques cherchées
		 * @param nbfois nombre de fois où l'on veut nb cartes identiques, 1 par défaut
		 * @return la carte si on a nb cartes identiques nbfois fois, -1 sinon
		 */
		int cartesIdentiques(int nb, int nbfois = 1) const;

		/**
		 * @action regarde si on a un flush (5 cartes de la même couleur)
		 * @return true si on a un flush
		 */
		bool flush() ;

        /**
         * @brief quinteFlushRoyale regarde si on a une quinte flush royale et set la combinaison si c'est le cas
         */
        void quinteFlushRoyale();

        /**
         * @brief quinteFlush regarde si on a une quinte flush et set la combinaison si c'est le cas
         */
        void quinteFlush();

        /**
         * @brief carre regarde si y a un carré
         * @return la carte du carré, -1 si pas de carré
         */
        int carre();

        /**
         * @brief full regarde si y a un full
         * @return la carte du full, 0 si pas de full
         */
        int full();

        /**
         * @brief brelan regarde si y a un brelan et set la combi à brelan si y en a un
         * @return 0 si brelan, -1 sinon
         */
        int brelan();

        /**
         * @brief doublePaire regarde s'il y a une double paire et set la combinaison si trouvée
         * @return 0 si double paire, -1 sinon
         */
        int doublePaire();

        /**
         * @brief paire regarde si paire et si c'est le cas, set la combinaison
         */
        void paire();

		/**
		 * @action calcule la combinaison la plus haute
		 * @return la valeur de la combinaison de la main (de 1 (carte haute) à 10 (quinte flush royale))
		 */
		void calculCombinaison();

        /**
         * @brief setPoidsCarteLaPlusHaute set le poids de la carte la plus haute. Pour couleur et carte_haute
         */
        void setPoidsCarteLaPlusHaute();

		/**
		 *@action calcule le poids calculé de la carte la plus haute (si pas d'autre combinaison)
		 */
		void calculerPoidsBasique();




	public: 

		CartesJoueur(std::vector<Carte> table, std::vector<Carte> mainJoueur);
		~CartesJoueur();

		/**
		 * @return la combinaison la plus forte des cartes du Joueur 
		 */
		 int getCombinaison() const;

		 /**
		  * @return le poids des cartes du joueur
		  */
		 int getPoids();

		 /**
		  * @param main2 la main à comparer avec this 
		  * @return 0 si la main1 perd face à la main2, 1 si elle gagne et 2 si égalité
		  * @action prend en compte que this et main2 ont la même combinaison 
		  * et calcule laquelle a les cartes les plus fortes
		  */
         RESULTAT_PARTIE comparerCombinaisonsEgales(CartesJoueur main2);

};

#endif
