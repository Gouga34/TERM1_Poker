/*========================================================================
Nom: PhaseJeu.h       Auteur: Manuel CHATAIGNER
Maj: 20/05/2015          Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe décrivant les résultats d'une phase de jeu (profilage/gains).
=========================================================================*/

#ifndef GAMEPHASE_H
#define GAMEPHASE_H

namespace profiling {

    class GamePhase
    {
        private:

            int m_nbPartsDone;
            int m_nbMinParts;
            int m_gains;

        public:

            GamePhase();
            virtual ~GamePhase();

            virtual int getNbPartsDone() const;
            virtual int getNbMinParts() const;
            virtual int getGains() const;

            /**
             * @brief Réinitialise la phase de jeu avec un nouvelle nombre de parties à réaliser
             * @param nbParts Nombre de parties minimum à réaliser
             */
            virtual void newPhase(int nbParts);

            /**
             * @brief Ajoute une nouvelle partie réalisée pour la phase de jeu
             * @param partGains Gains de la partie terminée
             */
            virtual void finishedPart(int partGains);
    };
}

#endif // GAMEPHASE_H
