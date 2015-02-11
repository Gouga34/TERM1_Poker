#include "../include/Carte.h"


/**
*@param  : La couleur et le rang de la carte
*@action : Initialise une nouvelle carte
**/
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

/**
*@action : Destructeur de la classe Carte
**/
Carte::~Carte(){

}

/**
*@action : Permet d'obtenir la couleur de la carte
*@return : La couleur de la carte
**/
int Carte::getCouleur() const{
	return ((this->id - ((this->id % 13) + 1)) / 10);
}

/**
*@action : Permet d'obtenir le rang de la carte
*@return : Le rang de la carte
**/
int Carte::getRang() const{
	return ((this->id % 13) + 1);
}

/**
*@action : Permet d'obtenir l'id de la carte
*@return : L'id de la carte
**/
int Carte::getId() const{
	return this->id;
}
