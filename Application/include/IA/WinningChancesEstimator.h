#ifndef WINNINGCHANCESESTIMATOR_H
#define WINNINGCHANCESESTIMATOR_H

#include "../Jeu/Jeu.h"
#include "../Jeu/Joueur.h"
#include "../Assessor/Assessor.h"
#include <QThread>

class WinningChancesEstimator : public QThread {

    private:

        // Nombre de tests à effectuer
        int m_testsNumber;

        // Résultat de l'estimation lancée dans le thread
        double m_estimateResult;

        Jeu *m_game;
        Joueur *m_player;


        /**
        *@action : Creer un nouveau deck prive des carte du joueur courant
        *@return  : Le deck des cartes restantes
        **/
        std::vector<game::Card> newDeck();

       /**
       *@action : Melange le jeu de carte
       *@param  : Le jeu a melanger
       **/
       void shuffle(std::vector<game::Card>& deck);

    protected:

        /**
         * @brief Lance l'estimation à partir des attributs dans un thread séparé
         */
        virtual void run();

    public:

        WinningChancesEstimator(Jeu *game, Joueur *player, int testsNumber = NOMBRE_DE_TESTS);
        virtual ~WinningChancesEstimator();

        virtual double getEstimateResult() const;

        virtual void setTestsNumber(int testsNumber);
        virtual void setGame(Jeu *game);
        virtual void setPlayer(Joueur *player);

		/**
        *@action : Calcule les chances de gagner du joueurCourant en fonction du jeu
		**/
        virtual void estimateWinningChances();
};

#endif //WINNINGCHANCESESTIMATOR_H
