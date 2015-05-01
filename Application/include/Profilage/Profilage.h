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

class Profilage
{
    private:

        Profil *profilIA;

    public:

        Profil *profilJoueur;
        RESULTAT_PARTIE partieGagneeIAQuiProfile;
        bool scenarioDeTest;


        int nombreParties;
        int partiesGagneesParIAQuiProfile;

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


        Profilage(Profil *calibrageIA, Profil *profil);
        ~Profilage();

        /**
         * @action Remplit les données de profilage avec le contenu du fichier correspondant au joueur
         */
        //void charger();

        /**
         * @action Ajoute les données de profilage de la partie dans le fichier correspondant au joueur
         */
        void sauvegarder();

        /**
         * @action : Vide le tableau
         */
        void clear();


};

#endif // PROFILAGE_H
