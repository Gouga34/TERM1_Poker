/*========================================================================
Nom: IntelligenceArtificielleProfilage.h       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015          Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Classe d'une IA qui profile.
=========================================================================*/

#ifndef INTELLIGENCEARTIFICIELLEPROFILAGE_H
#define INTELLIGENCEARTIFICIELLEPROFILAGE_H

#include "IntelligenceArtificielle.h"
#include "../Profilage/Profilage.h"
#include "../Profilage/ScenariosDeTests.h"

class IntelligenceArtificielleProfilage : public IntelligenceArtificielle
{
    private:

        bool continuerProfilage;         // Vrai si l'IA profile, faux si elle utilise les résultats pour gagner

        Profil profilJoueur;

        Profilage *profilage;

        ScenariosDeTests *scenario;

        double calculValeurProportionnelle(double minVal1, double val1, double maxVal1, double minVal2, double maxVal2) const;

    public:

        IntelligenceArtificielleProfilage(bool estDealer, int jetons, int position);
        IntelligenceArtificielleProfilage(const IntelligenceArtificielle& joueur);
        virtual ~IntelligenceArtificielleProfilage();


        /**
         * @return Profilage du joueur par l'IA
         */
        Profilage* getProfilage() const;

        /**
         * @action Modifie le pseudo du joueur profilé
         * @param pseudo Pseudo du joueur
         */
        void setPseudoJoueurProfile(std::string pseudo);

        /**
         * @action Remplit l'objet contenant les données de profilage du joueur
         */
        void remplissageDonneesProfilage();

        /**
         * @action Calcule les données globales de profilage et sauvegarde les données de la partie
         */
        void calculProfilGlobalJoueur();

        /**
         * @action Ecrit les résultats des scénarios de tests pour la partie courante
         */
        void ecritureScenariosDeTests();

        /**
         * @action Modifie la calibrage de l'ia si elle n'est pas en train de profiler pour gagner
         */
        void setCalibragePourJouer();

        /**
         * @action Ajoute une ligne d'analyse des résultats des gains pour le calibrage courant
         */
        void ecritureAnalyseDesGains();
};

#endif // INTELLIGENCEARTIFICIELLEPROFILAGE_H
