#ifndef IntelligenceArtificielle_h
#define IntelligenceArtificielle_h

#include "../Jeu/Joueur.h"
#include "../Jeu/Jeu.h"
#include "Resolveur.h"
#include "EstimationProba.h"
#include "../Profilage/Profil.h"

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

		//Méthodes
		
		/**
		*@action : Permet d'obtenir la probabilité pre-Flop
		*@return : La probabilitee pre-Flop de la main courant
		**/
        double calculProba();
	
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
        Profil* getCalibrage();

        /**
         * @action Modifie les valeurs de calibrage de l'IA
         * @param profil Profil de l'IA
         */
        void setCalibrage(Profil profil);
		
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
         * @action Calcule les chances de gain en fonction des cartes connues
         */
        void estimationChancesDeGain();
	
		/**
		*@action : Pemet a l'IA de jouer
		**/
        virtual Action jouer();
	
};

#endif
