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
#include <array>

class CartesJoueur {

	private: 
		/** 
			Les quatre premières lignes correspondent aux quatre couleurs,
			La dernière ligne au nombre d'occurences de chaque carte, toutes couleurs confondues,
			Les 14 premières colonnes correspondent aux cartes (de 1 à AS, en comptant l'AS deux fois)
			La dernière colonne correspond au nombre de cartes d'une même couleurs.
		*/

        std::array< std::array< int, 5>, 15> m_occurrences;

		//Combinaison la plus forte de la main du joueur.
        FORCE_MAIN m_combinaison;

		//Poids de la main
        int m_poids;

		/**
		 * @param combi combinaison
		 * @action modifie la combinaison avec une nouvelle combinaison
		 */
        virtual void setCombinaison(FORCE_MAIN combi);

		/**
		 * @param p poids
		 * @action modifie le poids avec un nouveau poids
		 */
        virtual void setPoids(int p);

        /**
         * @brief initialiserOccurrences initialise le tableau des occurrences à 0.
         */
        virtual void initialiserOccurrences();

		/**
		 * @param table cartes de la table
		 * @param mainJoueur cartes du joueur
	     * @action rempli occurrences
	     */
        virtual void remplirTableau(std::vector<Carte> table, std::vector<Carte> mainJoueur);

        /**
         * @brief enregistrerOccurrencesCartes enregistre les occurrences des cartes dans le tableau occurrences.
         * @param listeCartes
         */
        virtual void enregistrerOccurrencesCartes(std::vector<Carte> listeCartes);

		/**
		 * @param depart case à partir de laquelle on va commencer
		 * @param ligne ligne que l'on va traiter
		 * @action regarde si on a une suite sur la ligne, à partir du départ (suite de 5 cartes minimum)
		 * @return true si on a une suite, false sinon
		 */
        virtual bool estSuite(int depart, int ligne) const;
 
        /**
		 * @param ligne ligne qui sera traitée
		 * @return true si la ligne contient une suite
		 */
        virtual bool contientSuite(int ligne);

		/**
		 * @param nb nombre de cartes identiques cherchées
		 * @param nbfois nombre de fois où l'on veut nb cartes identiques, 1 par défaut
		 * @return la carte si on a nb cartes identiques nbfois fois, -1 sinon
		 */
        virtual int cartesIdentiques(int nb, int nbfois = 1) const;

		/**
		 * @action regarde si on a un flush (5 cartes de la même couleur)
         * @return true si on a un flush, false sinon
		 */
        virtual bool checkIfFlushAndSetCombinaison();

        /**
         * @brief quinteFlushRoyale regarde si on a une quinte flush royale et set la combinaison si c'est le cas
         * @return true si quinte flush royale, false sinon
         */
        virtual bool checkIfQuinteFlushRoyaleAndSetCombinaison();

        /**
         * @brief quinteFlush regarde si on a une quinte flush et set la combinaison si c'est le cas
         * @return true si quinte flush, false sinon
         */
        virtual bool checkIfQuinteFlushAndSetCombinaison();

        /**
         * @brief carre regarde si y a un carré
         * @return true si carré, false sinon
         */
        virtual bool checkIfCarreAndSetCombinaison();

        /**
         * @brief full regarde si y a un full
         * @return true si full, false sinon
         */
        virtual bool checkIfFullAndSetCombinaison();

        /**
         * @brief brelan regarde si y a un brelan et set la combi à brelan si y en a un
         * @return true si brelan, false sinon
         */
        virtual bool checkIfBrelanAndSetCombinaison();

        /**
         * @brief doublePaire regarde s'il y a une double paire et set la combinaison si trouvée
         * @return true si double paire, false sinon
         */
        virtual bool checkIfDoublePaireAndSetCombinaison();

        /**
         * @brief paire regarde si paire et si c'est le cas, set la combinaison
         * @return true si paire, false sinon
         */
        virtual bool checkIfPaireAndSetCombinaison();

        /**
         * @brief checkIfSuiteAndSetCombinaison regarde si on a une suite. Si c'est le cas, set la combinaison
         * @return true si on a une suite, false sinon
         */
        virtual bool checkIfSuiteAndSetCombinaison();

		/**
		 * @action calcule la combinaison la plus haute
		 * @return la valeur de la combinaison de la main (de 1 (carte haute) à 10 (quinte flush royale))
		 */
        virtual void calculCombinaison();

        /**
         * @brief setPoidsCarteLaPlusHaute set le poids de la carte la plus haute. Pour couleur et carte_haute
         */
        virtual void setPoidsCarteLaPlusHaute();

		/**
		 *@action calcule le poids calculé de la carte la plus haute (si pas d'autre combinaison)
		 */
        virtual void calculerPoidsBasique();

	public: 

		CartesJoueur(std::vector<Carte> table, std::vector<Carte> mainJoueur);
        virtual ~CartesJoueur();

		/**
		 * @return la combinaison la plus forte des cartes du Joueur 
		 */
         virtual FORCE_MAIN getCombinaison() const;

		 /**
		  * @return le poids des cartes du joueur
		  */
         virtual int getPoids() const;

		 /**
		  * @param main2 la main à comparer avec this 
		  * @return 0 si la main1 perd face à la main2, 1 si elle gagne et 2 si égalité
		  * @action prend en compte que this et main2 ont la même combinaison 
		  * et calcule laquelle a les cartes les plus fortes
		  */
         virtual RESULTAT_PARTIE comparerCombinaisonsEgales(CartesJoueur main2);

};

#endif
