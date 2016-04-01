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

            int getNbPartsDone() const;

            int getNbMinParts() const;

            int getGains() const;

            /**
             * @brief Réinitialise la phase de jeu avec un nouvelle nombre de parties à réaliser
             * @param nbParts Nombre de parties minimum à réaliser
             */
            void newPhase(int nbParts);

            /**
             * @brief Ajoute une nouvelle partie réalisée pour la phase de jeu
             * @param partGains Gains de la partie terminée
             */
            void finishedPart(int partGains);
    };
}

#endif // GAMEPHASE_H
