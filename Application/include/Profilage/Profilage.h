/*========================================================================
Nom: Profilage.h       Auteur: Manuel CHATAIGNER
Maj: 04/03/2015          Creation: 04/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe contenant l'écriture et la lecture
               des données de profilage.
=========================================================================*/


#ifndef PROFILAGE_H
#define PROFILAGE_H

#include <string>
#include <QString>
#include "../Constantes.h"
#include "Profil.h"
#include "PhaseJeu.h"

class Profilage
{
    private:

        Profil *profilIA;

    public:

        Profil *profilJoueur;

        RESULTAT_PARTIE partieGagneeIAQuiProfile;
        int nbJetonsGagnesIAQuiProfile;
        bool scenarioDeTest;


        struct EtapePartie
        {
            bool couche;                    // vrai si le joueur s'est couché

            double probaGainAdversaire;     // % de chance de gagner du joueur (-1 si cartes inconnues)

            /* Type du joueur (somme des taux = 100%) */
            double tauxAgressivite;         // Agressivité du joueur (mise la plus haute, mise totale, nb checks)
            double tauxRationnalite;        // Rationalite du joueur (mise totale, s'est couché ?)
            double tauxBluff;               // Bluff du joueur (mise totale, s'est couché ?)
            double tauxPassivite;           // Passivite du joueur (suivis, checks)

            /* Taux en % des différentes actions effectuées par le joueur (les relances sont incluses dans les mises) */
            double tauxSuivis;
            double tauxChecks;
            double tauxMises;

            /* Taux en % des mises en fonction des jetons du joueur */
            double misePlusHaute;           // Mise plus haute du joueur
            double miseTotaleJoueur;        // Mise totale du joueur

            int nbTotalActions;
        };

        // Une ligne par étape + 1 ligne pour le global
        EtapePartie etatPartie[ETAPE_JEU::NB_ETAPES + 1];


        // Données des phases de profilage et de gains en cours
        PhaseJeu phaseProfilageCourante;
        PhaseJeu phaseJeuCourante;


        // Données globales au profilage
        int nombreParties;
        int nbPartiesGagneesProfilage;
        int nbPartiesProfilage;

        int nbPartiesGagneesJeu;
        int nbPartiesJeu;

        int gainsProfilage;
        int gainsJeu;


        Profilage(Profil *calibrageIA, Profil *profil);
        ~Profilage();

        /**
         * @action Efface les données de profilage de la partie enregistrée
         */
        void reinitialiser();

        /**
         * @action Ajoute les données de profilage de la partie dans le fichier correspondant au joueur
         */
        void sauvegarder();
};

#endif // PROFILAGE_H
