#ifndef IntelligenceArtificielle_h
#define IntelligenceArtificielle_h

#include "../Jeu/Joueur.h"
#include "../Jeu/Jeu.h"
#include "Resolveur.h"
#include "EstimationProba.h"
#include "../Profilage/Profile.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>


class Resolveur;
class EstimationProba;

class IntelligenceArtificielle : public Joueur {

    protected:
        Resolveur *resolveur; //Cerveau de l'intelligence artificielle

    private:

        std::vector<EstimationProba*> estimateurs;
		
	public :
		//Constructeur et destructeur
		
		/**
		*@action : Constructeur de la classe Intelligence Artificielle
		*@param  : Un boolean indiquant si le jouer est le dealer, sa cave de depart et sa position
		**/
		IntelligenceArtificielle(bool estDealer, int jetons, int position);
		
		/**
		*@action : Constructeur de la classe Intelligence Artificielle par copie de la classe Joueur
		*@param  : L'instance de la classe Joueur utilisee pour la copie
		**/		
        IntelligenceArtificielle(const IntelligenceArtificielle& joueur);
		
		/**
		*@action : Destructeur de la classe Intelligence Artificielle
		**/
        virtual ~IntelligenceArtificielle();
		
        virtual bool estHumain() const;

        /**
         * @return Calibrage de l'IA
         */
        Profile* getCalibrage();

        /**
         * @action Modifie les valeurs de calibrage de l'IA
         * @param profil Profil de l'IA
         */
        void setCalibrage(Profile profil);

        /**
         * @brief Attribue un nouveau calibrage tiré aléatoirement pour l'IA
         */
        void nouveauCalibrage();
		
        //Methodes

        /**
         * @action Lance une estimation des chances de gain dans plusieurs threads simultanément
         * @param nbTests Nombre de tests à lancer au total
         * @param nbThreads Nombre de threads à lancer
         */
        void lancerEstimationChancesDeGain(int nbTests, int nbThreads = 1);

        /**
         * @action Attend que l'ensemble des threads soit terminé
         */
        void attendreResultatEstimation();
	
		/**
		*@action : Pemet a l'IA de jouer
		**/
        virtual Action jouer();
	
};

#endif
