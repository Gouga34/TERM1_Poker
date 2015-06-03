/*========================================================================
Nom: PhaseJeu.h       Auteur: Manuel CHATAIGNER
Maj: 20/05/2015          Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe décrivant les résultats d'une phase de jeu (profilage/gains).
=========================================================================*/

#ifndef PHASEJEU_H
#define PHASEJEU_H

class PhaseJeu
{
    private:

        int nbPartiesRealisees;
        int nbPartiesMinimum;
        int gains;

    public:

        PhaseJeu();

        int getNbPartiesRealisees() const;

        int getNbPartiesMinimum() const;

        int getGains() const;

        /**
         * @brief Réinitialise la phase de jeu avec un nouvelle nombre de parties à réaliser
         * @param nbParties Nombre de parties minimum à réaliser
         */
        void nouvellePhase(int nbParties);

        /**
         * @brief Ajoute une nouvelle partie réalisée pour la phase de jeu
         * @param gainsPartie Gains de la partie terminée
         */
        void partieTerminee(int gainsPartie);
};


#endif // PHASEJEU_H
