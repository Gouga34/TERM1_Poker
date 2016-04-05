#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <stdlib.h>

#include "Card.h"
#include "../Constantes.h"
#include "../Assessor/Assessor.h"
#include "../Profiling/CalculateProfilingData.h"
#include "../Profiling/Profiling.h"
#include "../AI/WinningChancesEstimator.h"
#include "../AI/ArtificialIntelligence.h"
#include "../Interface/ChoixOptionsDialog.h"

class ArtificialIntelligence;

namespace game {

    class Player;

    class Game {

        private :
            std::vector<Player*>                m_players;
            std::vector<game::Card>             m_deck;
            std::vector<game::Card>             m_table;
            std::vector<std::vector<ACTION>>    m_actions;        // Liste d'actions par joueur
            int                                 m_blind;
            int                                 m_currentPlayer;
            int                                 m_pot;

            /** Correspond aux jetons mis en jeu à un instant t de la partie par le joueur
             *  précédent celui qui doit jouer **/
            int                                 m_currentBet;

            /** Correspond à la somme des mises et relances d'une étape. C'est la somme des
             *  jetons qu'un joueur doit mettre pour rester en jeu **/
            int                                 m_accumulatedBetsAndRaises;
            int                                 m_dealer;
            bool                                m_isGameOver;
            ETAPE_JEU                           m_currentStep;
            RESULTAT_PARTIE                     m_gameResult;
            std::vector<game::Card>             m_tableTmp;

            OptionsJeu                          m_options;


            /**
             * @brief Méthode qui permet à un joueur de jouer de l'argent (mise, relancer, suivi...)
             *        setMiseCourante, setMiseTotale, setMisePlusHaute (avec vérif),
             *        Incrémentation pot, Décrémentation cave
             * @param posJoueur Position du joueur
             * @param jetons Nombre de jetons à jouer
             */
            virtual void putMoneyIntoGame(int playerPosition, int tokens);

            /**
            *@action : Commande permettant a un joueur de miser
            *@param  : La position du joueur effectuant l'action ainsi que le montant de la mise
            **/
            virtual void bet(int playerPosition, int tokens);

            /**
            *@action : Commande permettant a un joueur de relancer
            *@param  : La position du joueur effectuant l'action ainsi que le montant de la relance
            **/
            virtual void raise(int playerPosition, int tokens);

            /**
            *@action : Commande permettant a un joueur de faire "tapis"
            *@param  : La position du joueur effectuant l'action
            * @param action effetuée (mise, relance, suivi)
            **/
            virtual void allIn(int playerPostion, ACTION action);

            /**
            *@action : Commande permettant a un joueur de suivre
            *@param  : La position du joueur effectuant l'action
            **/
            virtual void call(int playerPosition);

            /**
            *@action : Commande permettant a un joueur de checker
            *@param  : La position du joueur effectuant l'action
            **/
            virtual void check(int playerPosition);

            /**
            *@action : Commande permettant a un joueur de se coucher
            *@param  : Le joueur effectuant l'action
            **/
            virtual void fold(int playerPosition);

            /**
             * @brief Calcule les chances de gain des deux ia
             */
            virtual void calculateWinningChances();

        public:
            /**
            *@param  : Le nombre de joueur, le montant de la blind de depart et les options
            *@action : Initialise un nouveau jeu
            **/
            Game(int nbPlayers, int blind, OptionsJeu options);

            virtual ~Game();


            virtual OptionsJeu getOptions() const;
            virtual RESULTAT_PARTIE getGameResult() const;
            virtual int	getBlind() const;
            virtual int getCurrentPlayer() const;
            virtual Player* getPlayer(int i);
            virtual std::vector<game::Card>	getTable() const;
            virtual int getPot() const;
            virtual int	getDealer() const;
            virtual int	getCurrentBet() const;
            virtual int getAccumulatedBetsAndRaises() const;
            virtual std::vector<game::Card>	getDeck() const;
            virtual std::vector<ACTION> getPlayersActions(int playerPosition) const;
            virtual ETAPE_JEU getStep() const;

            virtual void setPot(int pot);

            /**
             * @brief Détermine la position du joueur adverse au joueur passé en paramètre
             * @param joueur Position du joueur dont on veut l'adversaire
             * @return Position du joueur adverse
             */
            virtual int getOpponentsPosition(int player) const;

            /**
                * @action Permet de connaitre la dernière action effectué par le joueur passé en paramètre
                * @param posJoueur Joueur dont on veut connaitre l'action
                * @return L'action effectué par le joueur courant
            **/
            virtual ACTION getPlayersLastAction(int playerPosition) const;

            /**
            *@action : Permet d'ajouter un joueur a la partie
            *@param  : Le joueur a ajouter a la partie
            **/
            virtual void addPlayer(Player *player);

            /**
             * @brief Lance une nouvelle partie
             */
            virtual void newGame();

            /**
             * @brief Réinitialise les caves des joueurs
             */
            virtual void resetCaves();

            /**
            *@action : Distribue a chaque joueur ses cartes
            **/
            virtual void handDistribution();

            /**
             * @action Distribue les nouvelles cartes de l'étape passée en paramètre
             * @param etape Nouvelle étape courante
             */
            virtual void newStep(ETAPE_JEU currentStep);

            /**
             * @brief distributionCartes distribue nbCartesADistribuer sur la table
             * @param nbCartesADistribuer
             */
            virtual void tableCardsDistribution(int cardsToDealNumber);

            /**
            *@action : Distribue les blinds en debut de partie
            **/
            virtual void blindDistribution();

            /**
            *@action : Melange le jeu de carte
            **/
            virtual void shuffleDeck();

            /**
            *@action : Cree l'ensemble des cartes utilisees dans le jeu
            *@return : Le "deck", l'ensemble du jeu de carte
            **/
            std::vector<game::Card> newDeck();

            /**
            *@action : Permet de savoir si le joueur a la possibilite checker
            *@param  : la position du joueur dont on veut savoir s'il peut checker
            *@return : vrai si le joueur peut checker, faux sinon
            **/
            virtual bool canCheck(int playerPosition);

            /**
            *@action : Permet de savoir si le joueur a la possibilite de relancer
            *@param  : la position du joueur dont on veut savoir s'il peut relancer et montant de la relance
            *@return : vrai si le joueur peut relancer, faux sinon
            **/
            virtual bool canRaise(int playerPosition, int tokens);

            /**
            *@action : Permet de savoir si le joueur a la possibilite de miser
            *@param  : la position du joueur dont on veut savoir s'il peut miser et montant de la mise
            *@return : vrai si le joueur peut miser, faux sinon
            **/
            virtual bool canBet(int playerPosition, int tokens);

            /**
            *@action : Permet de savoir si le joueur a la possibilite de suivre
            *@param  : la position du joueur dont on veut savoir s'il peut suivre
            *@return : vrai si le joueur peut suivre, faux sinon
            **/
            virtual bool canCall(int playerPosition);

            /**
             * @param posJoueur Position du joueur à tester
             * @return Vrai si le joueur est couché
             */
            virtual bool isFolded(int playerPosition) const;

            /**
             * @action Exécute l'action du joueur (check, mise...)
             * @param posJoueur Position du joueur qui exécute l'action
             * @param action Action à exécuter
             * @param mise Montant des jetons associés à l'action (-1 si check, se couche, suivre)
             */
            virtual void executeAction(int playerPosition, Action action);

            /**
            *@action  : Affecte le joueur suivant en tant que joueur courant et renvoi vrai s'il existe
            *@return : vrai s'il y a un joueur suivant, faux sinon
            **/
            virtual bool nextPlayer();

            /**
            *@action  : Permet de savoir si l'on est en début de tour
            *@return  : Vrai si l'on se trouve en debut de tour, faux sinon
            **/
            virtual bool isRoundBeginning();

            /**
            *@action  : Permet de savoit si le tour est terminé
            *@return  : Vrai si le tour est termnié, faux sinon
            **/
            virtual bool isRoundEnd();

            /**
            *@action  : Permet de reinitialiser le tableau des actions
            **/
            virtual void resetActions();

            /**
             * @action Effectue l'ensemble des actions de fin de partie
             */
            virtual void partEnd();

            /**
            *@action  : Relance une nouvelle main
            **/
            virtual void newHand();

            /**
            *@action  : Affecte les cartes choisies via l'interface
            *@param   : Un vecteur d'entier correspondant a la liste des ids des cartes
            **/
            virtual void affectsCards(std::vector<int> cardsIds);

            /**
             * @action Démarre la boucle de jeu principale
             */
            virtual void launchPart();

            /**
            *@action  : Permet de savoir si un Joueur a fait Tapis
            *@return  : Vrai si un joueur a fait tapis, faux sinon
            **/
            virtual bool isPlayerAllIn();
    };
}

#endif //GAME_H
