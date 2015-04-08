#ifndef IntelligenceArtificielle_h
#define IntelligenceArtificielle_h

#include "../Jeu/Joueur.h"
#include "../Jeu/Jeu.h"
#include "Resolveur.h"
#include "../Profilage/Profil.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>


class Resolveur;
class IntelligenceArtificielle : public Joueur {

    protected:
        Resolveur *resolveur; //Cerveau de l'intelligence artificielle

    private:
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
		IntelligenceArtificielle(Joueur joueur);
		
		/**
		*@action : Destructeur de la classe Intelligence Artificielle
		**/
        virtual ~IntelligenceArtificielle();
		
		//Mutateur

        /**
         * @action Modifie les valeurs de calibrage de l'IA
         * @param profil Profil de l'IA
         */
        void setCalibrage(Profil profil);
		
        //Methodes

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
