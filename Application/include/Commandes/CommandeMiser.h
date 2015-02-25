#ifndef CommandeMiser_h
#define CommandeMiser_h

#include "CommandeAbstraite.h"

class CommandeMiser : public CommandeAbstraite {

	private:
		int mise;

	public:
	
		CommandeMiser(Joueur* joueur, Jeu* jeu, int mise) : CommandeAbstraite(joueur, jeu){
			this->mise = mise;
		}
	
		virtual void execute(){
			this->jeu->miser(joueur->getPosition(), this->mise);
		}

};

#endif
