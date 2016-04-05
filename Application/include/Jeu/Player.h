#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "Card.h"
#include "Action.h"

class Jeu;

class Player {

	protected:
        bool                m_isDealer;
        std::vector<game::Card>	m_hand;
        int*                m_actionsCounter;
        int                 m_cave;
        int                 m_startingCave;
        Jeu*                m_game;
        int                 m_position;
        int                 m_currentBet;
        int                 m_highestBet;
        int                 m_accumulatedBetsAndRaises;
        double              m_winningChances;
        int                 m_totalBet;

	public:
	
        /**
		*@param  : Un boolean indiquant si le joueur est le dealer virtuel
        *@param  : Le nombre de jetons dont il dispose au debut de la partie
        *@param  : Sa position
		**/
        Player(bool isDealer, int cave, int m_position);
		
        virtual ~Player();

        virtual double getWinningChances() const;
        virtual int getCave() const;
        virtual int getStartingCave() const;
        virtual Jeu* getGame() const;
        virtual int	getPosition() const;
        virtual int getCurrentBet() const;
        virtual int	getHighestBet() const;
        virtual int* getActionsCounter() const;
        virtual int getAccumulatedBetsAndRaises() const;
        virtual int getTotalBet() const;
        virtual std::vector<game::Card>	getHand() const;

        virtual void setWinningChances(double winningChances);
        virtual void setCave(int cave);
        virtual void setStartingCave(int m_cave);
        virtual void setGame(Jeu* game);
        virtual void setCurrentBet(int currentBet);
        virtual void setHighestBet(int highestBet);
        virtual void setAccumulatedBetsAndRaises(int accumulatedBetsAndRaises);
        virtual void setTotalBet (int totalBets);

        /**
         * @return true si c'est un joueur humain, false si c'est une IA
         */
        virtual bool isHumain() const = 0;
	
		/**
		*@param  : Le nombre de jetons a ajouter
		*@action : Ajoute des jetons a la cave d'un joueur
		**/
        virtual void addTokens(int tokens);
		
		/**
		*@param  : Le nombre de jetons a retirer
		*@action : Retire des jetons a la cave d'un joueur
		**/
        virtual void removesTokens(int tokens);
		
		/**
		*@param  : La carte a ajouter a la main du joueur
		*@action : Ajoute une carte a la main du joueur
		**/
        virtual void addCard(game::Card card);
		
		/**
		*@action : Permet de savoir si un joueur est le dealer virtuel
		*@return : Un boolean indiquant si le joueur est considere comme le 
		*		dealer
		**/
        virtual bool isDealer();
		
		/**
		*@action : Reinitialise la main du joueur
		**/
        virtual void resetHand();
		
		/**
		*@action : Modifie l'attribut dealer du joueur
		**/		
        virtual void changeDealer();
		
		/**
		*@action : Vide la main du joueur
		**/		
        virtual void clearHand();

        /**
        *@action : Remet a zero les compteurs comptabilisant les actions du joueur
        **/
        virtual void resetActionsCounter();

        /**
         * @action Fait jouer le joueur lorsque c'est son tour
         */
        virtual Action play() = 0;
};

#endif //PLAYER_H
