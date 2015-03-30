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

class Profilage
{
    private:

        std::string nomJoueur;

    public:

        int typeJoueur[PROFIL_JOUEUR::NB_PROFILS];

        struct Profil
        {
            bool couche;                    // vrai si le joueur s'est couché

            double probaGainAdversaire;     // % de chance de gagner du joueur (-1 si cartes inconnues)
            double pot;                     // % du pot par rapport à la cave du joueur

            /* Type du joueur (somme des taux = 100%) */
            double tauxAgressivite;         // Agressivité du joueur (mise la plus haute, mise totale, nb checks)
            double tauxRationnalite;        // Rationnalite du joueur (mise totale, s'est couché ?)
            double tauxBluff;               // Bluff du joueur (mise totale, s'est couché ?)
            double tauxPassivite;           // Passivite du joueur (suivis, checks)

            /* Taux en % des différentes actions effectuées par le joueur (les relances sont incluses dans les mises) */
            double tauxSuivis;
            double tauxChecks;
            double tauxMises;

            /* Taux en % des mises en fonction des jetons du joueur */
            double misePlusHaute;           // Mise plus haute du joueur
            double miseTotaleJoueur;        // Mise totale du joueur
            double miseTotaleIA;            // Mise totale de l'IA
        };

        bool partieGagnee;              // vrai si l'IA a gagné la partie

        Profil profil[ETAPE_JEU::NB_ETAPES];

        double agressiviteGlobale;
        double bluffGlobal;
        double rationaliteGlobale;
        double passiviteGlobale;



        Profilage(std::string joueur);
        ~Profilage();

        /**
         * @action Remplit les données de profilage avec le contenu du fichier correspondant au joueur
         */
        void charger();

        /**
         * @action Ajoute les données de profilage de la partie dans le fichier correspondant au joueur
         */
        void sauvegarder() const;

        /**
        * @action : Vide le tableau
        */
        void clear();

        /**
        * @action : Supprime les valeurs negatives du tableau
        * @param  : L'etape a verifier
        */
        void correction(ETAPE_JEU etape);

};

#endif // PROFILAGE_H
