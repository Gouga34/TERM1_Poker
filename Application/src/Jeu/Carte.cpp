#include "../../include/Jeu/Carte.h"


Carte::Carte(int rang, int couleur){
	
    if(rang < 1){
        rang = 1;
    }else if(rang >  13){
        rang = 13;
	}

	if(couleur < 0){
		couleur = 0;
	}else if(couleur > 3){
		couleur = 3;
	}

    this->id = ( (couleur * 13) + (rang - 1) );
	
}


Carte::~Carte(){

}


int Carte::getCouleur() const{
	return ((this->id - ((this->id % 13) + 1)) / 10);
}


int Carte::getRang() const{
	return ((this->id % 13) + 1);
}


int Carte::getId() const{
	return this->id;
}
