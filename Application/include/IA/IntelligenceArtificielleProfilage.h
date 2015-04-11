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

        Profil profilJoueur;

        Profilage *profilage;

        ScenariosDeTests *scenario;

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
};

#endif // INTELLIGENCEARTIFICIELLEPROFILAGE_H
