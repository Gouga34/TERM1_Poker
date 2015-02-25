#ifndef CommandeAbstraite_h
#define CommandeAbstraite_h

#include "../Joueur.h"
#include "../Jeu.h"


class CommandeAbstraite{

	protected:
		Joueur* joueur;
		Jeu*	jeu;

	public:
		CommandeAbstraite(Joueur* joueur, Jeu* j){
			this->joueur = joueur;
			this->jeu = jeu;
		
		};
    		virtual void execute() = 0;
};

#endif
