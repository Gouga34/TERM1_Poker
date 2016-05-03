/*========================================================================
Nom: MainJoueur.h       Auteur: Morgane Vidal
Maj: 04/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe correspondant à la main d'un joueur, 
				avec ses cartes et celles du milieu
=========================================================================*/

#ifndef PLAYERCARDS_H
#define PLAYERCARDS_H

#include "../Game/Card.h"
#include <iostream>
#include <vector>
#include <array>

namespace assessor {

    class PlayerCards {

        private:
            /**
                Les quatre premières lignes correspondent aux quatre couleurs,
                La dernière ligne au nombre d'occurences de chaque carte, toutes couleurs confondues,
                Les 14 premières colonnes correspondent aux cartes (de 1 à AS, en comptant l'AS deux fois)
                La dernière colonne correspond au nombre de cartes d'une même couleurs.
            */

            static const int m_nbRows = 5;
            static const int m_nbLines = 15;

            std::array< std::array< int, m_nbRows>, m_nbLines> m_occurrences;

            //Combinaison la plus forte de la main du joueur.
            HAND_STRENGHT m_handCategory;

            //Poids de la main
            int m_weight;

            /**
             * @param combi combinaison
             * @action modifie la combinaison avec une nouvelle combinaison
             */
            virtual void setHandCategory(HAND_STRENGHT handCategory);

            /**
             * @param p poids
             * @action modifie le poids avec un nouveau poids
             */
            virtual void setWeight(int weight);

            /**
             * @brief initializeOccurrences initialise le tableau des occurrences à 0.
             */
            virtual void initializeOccurrences();

            /**
             * @param table cartes de la table
             * @param mainJoueur cartes du joueur
             * @action rempli occurrences
             */
            virtual void fillTab(std::vector<game::Card> table, std::vector<game::Card> playersHand);

            /**
             * @brief registerCardsOccurrences enregistre les occurrences des cartes dans le tableau occurrences.
             * @param listeCartes
             */
            virtual void registerCardsOccurrences(std::vector<game::Card> cardsList);

            /**
             * @param depart case à partir de laquelle on va commencer
             * @param ligne ligne que l'on va traiter
             * @action regarde si on a une suite sur la ligne, à partir du départ (suite de 5 cartes minimum)
             * @return true si on a une suite, false sinon
             */
            virtual bool isStraight(int start, int line) const;

            /**
             * @param ligne ligne qui sera traitée
             * @return true si la ligne contient une suite
             */
            virtual bool containsStraight(int line);

            /**
             * @param nb nombre de cartes identiques cherchées
             * @param nbfois nombre de fois où l'on veut nb cartes identiques, 1 par défaut
             * @return la carte si on a nb cartes identiques nbfois fois, -1 sinon
             */
            virtual int identicalCards(int nb, int nbTimes = 1) const;

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
            virtual void calculatesHandCategory();

            /**
             * @brief setPoidsCarteLaPlusHaute set le poids de la carte la plus haute. Pour couleur et carte_haute
             */
            virtual void setHighestCardWeight();

            /**
             *@action calcule le poids calculé de la carte la plus haute (si pas d'autre combinaison)
             */
            virtual void calculateBasicWeight();

            /**
             * @brief compareWeight
             * @param hand2
             * @return RESULTAT_PARTIE::PERDU si this a un poids inférieur au poids de main2
             *         RESULTAT_PARTIE::GAGNE si this a un poids supérieur au poids de main2
             *         RESULTAT_PARTIE::EGALITE sinon
             */
            virtual GAME_RESULT compareWeights(PlayerCards hand2);

            /**
             * @brief compareEqualFourOfAKind compare deux carrés égaux
             * (compare les 5m cartes des deux mains pour savoir laquelle est la plus haute)
             * @param hand2
             * @return RESULTAT_PARTIE::PERDU si this a perdu face à main2
             *         RESULTAT_PARTIE::GAGNE si this a gagné face à main2
             *         RESULTAT_PARTIE::EGALITE sinon
             */
            virtual GAME_RESULT compareEqualFourOfAKind(PlayerCards hand2);

            /**
             * @brief compareEqualFullHouse compare deux full dont les trios sont les mêmes.
             * (compare les valeurs des deux paires)
             * @param hand2
             * @return RESULTAT_PARTIE::PERDU si this a perdu face à main2
             *         RESULTAT_PARTIE::GAGNE si this a gagné face à main2
             *         RESULTAT_PARTIE::EGALITE sinon
             */
            virtual GAME_RESULT compareEqualFullHouse(PlayerCards hand2);

            /**
             * @brief compareEqualFlush compare deux couleurs en comparant les cartes une à une
             * @param hand2
             * @return RESULTAT_PARTIE::PERDU si this a perdu face à main2
             *         RESULTAT_PARTIE::GAGNE si this a gagné face à main2
             *         RESULTAT_PARTIE::EGALITE sinon
             */
            virtual GAME_RESULT compareEqualFlush(PlayerCards hand2);

            /**
             * @brief compareThreeOfAKind compare deux brelans dont les valeurs des triplets sont les mêmes.
             * (compare les autres cartes)
             * @param hand2
             * @return RESULTAT_PARTIE::PERDU si this a perdu face à main2
             *         RESULTAT_PARTIE::GAGNE si this a gagné face à main2
             *         RESULTAT_PARTIE::EGALITE sinon
             */
            virtual GAME_RESULT compareThreeOfAKind(PlayerCards hand2);

            /**
             * @brief compareTwoPair compare deux double paires égales.
             * @param hand2
             * @return RESULTAT_PARTIE::PERDU si this a perdu face à main2
             *         RESULTAT_PARTIE::GAGNE si this a gagné face à main2
             *         RESULTAT_PARTIE::EGALITE sinon
             */
            virtual GAME_RESULT compareTwoPair(PlayerCards hand2);

            /**
             * @brief comparePair compare deux paires égales (compare autres cartes)
             * @param hand2
             * @return RESULTAT_PARTIE::PERDU si this a perdu face à main2
             *         RESULTAT_PARTIE::GAGNE si this a gagné face à main2
             *         RESULTAT_PARTIE::EGALITE sinon
             */
            virtual GAME_RESULT comparePair(PlayerCards hand2);

            /**
             * @brief compareHightCard compare la carte la plus haute des mains
             * @param hand2
             * @return RESULTAT_PARTIE::PERDU si this a perdu face à main2
             *         RESULTAT_PARTIE::GAGNE si this a gagné face à main2
             *         RESULTAT_PARTIE::EGALITE sinon
             */
            virtual GAME_RESULT compareHightCard(PlayerCards hand2);

        public:

            PlayerCards(std::vector<game::Card> table, std::vector<game::Card> playersHand);
            virtual ~PlayerCards();

            /**
             * @return la combinaison la plus forte des cartes du Joueur
             */
             virtual HAND_STRENGHT getHandCategory() const;

             /**
              * @return le poids des cartes du joueur
              */
             virtual int getWeight() const;

             /**
              * @param main2 la main à comparer avec this
              * @return 0 si la main1 perd face à la main2, 1 si elle gagne et 2 si égalité
              * @action prend en compte que this et main2 ont la même combinaison
              * et calcule laquelle a les cartes les plus fortes
              */
             virtual GAME_RESULT compareEqualHandCategories(PlayerCards hand2);

    };
}
#endif
