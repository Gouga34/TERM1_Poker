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
		 * @param table cartes de la table
		 * @param mainJoueur cartes du joueur
	     * @action rempli occurrences
	     */
		void remplirTableau(std::vector<Carte> table, std::vector<Carte> mainJoueur);

		/**
		 * @param depart case à partir de laquelle on va commencer
		 * @param ligne ligne que l'on va traiter
		 * @action regarde si on a une suite sur la ligne, à partir du départ (suite de 5 cartes minimum)
		 * @return true si on a une suite, false sinon
		 */
		bool estSuite(int depart, int ligne) const;
 
		/*
		 * @param ligne ligne qui sera traitée
		 * @return true si la ligne contient une suite
		 */
		bool contientSuite(int ligne) const;

		/**
		 * @param nb nombre de cartes identiques cherchées
		 * @param nbfois nombre de fois où l'on veut nb cartes identiques, 1 par défaut
		 * @return true si on a nb cartes identiques nbfois fois
		 */
		bool cartesIdentiques(int nb, int nbfois = 1) const;

		/**
		 * @action regarde si on a un flush (5 cartes de la même couleur)
		 * @return true si on a un flush
		 */
		bool flush() const;

		/**
		 * @action calcule la combinaison la plus haute
		 * @return la valeur de la combinaison de la main (de 1 (carte haute) à 10 (quinte flush royale))
		 */
		void calculCombinaison();

		/**
		 *@action calcule le poids calculé de la carte la plus haute (si pas d'autre combinaison)
		 */
		void calculerPoids();




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










};

#endif