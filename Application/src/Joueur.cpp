#include "../include/Joueur.h"

/**
*@param  : Un boolean indiquant si le joueur est le dealer virtuel
*		ainsi que le nombre de jetons dont il dispose au
*		debut de la partie
*@action : Cree un nouveau joueur
**/
Joueur::Joueur(bool estDealer, int jetons){
	this->dealer = estDealer;
	this->main = *(new std::vector<Carte>());
	this->cave = jetons;
}

/**
*@action : Destructeur de la classe Joueur
**/
Joueur::~Joueur(){

}


/**
*@action : Permet d'obtenir le nombre de jeton dont dispose un joueur
*@return : La cave d'un joueur
**/
int Joueur::getCave() const{
	return this->cave;
}

/**
*@action : 
*@return : 
**/
std::vector<Carte> Joueur::getMain() const{
	return this->main;
}

/**
*@param  : Le nombre de jetons a ajouter
*@action : Ajoute des jetons a la cave d'un joueur
**/
void Joueur::ajouteJetons(int jetons){
	this->cave = this->cave + jetons;
}

/**
*@param  : Le nombre de jetons a retirer
*@action : Retire des jetons a la cave d'un joueur
**/
void Joueur::retireJetons(int jetons){
	this->cave = this->cave - jetons;
}

/**
*@param  : La carte a ajouter a la main du joueur
*@action : Ajoute une carte a la main du joueur
**/
void Joueur::ajouteCarte(Carte carte){
	this->main.push_back(carte);
}

/**
*@action : Permet de savoir si un joueur est le dealer virtuel
*@return : Un boolean indiquant si le joueur est considere comme le 
*		dealer
**/
bool Joueur::estDealer(){
	return this->dealer;
}
/**
*@action : Reinitialise la main du joueur
**/
void Joueur::resetMain(){
	this->main.clear();
}
