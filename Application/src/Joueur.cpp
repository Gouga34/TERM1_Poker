#include "../include/Joueur.h"
#include "../include/Jeu.h" 

/**
*@param  : Un boolean indiquant si le joueur est le dealer virtuel
*		ainsi que le nombre de jetons dont il dispose au
*		debut de la partie
*@action : Cree un nouveau joueur
**/
Joueur::Joueur(bool estDealer, int jetons, int position){
	this->dealer = estDealer;
	this->cave = jetons;
	this->position = position;
	this->miseJoueur = 0;
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
*@action : Permet d'obtenir la position du joueur
*@return : La position du joueur
**/
int Joueur::getPosition() const{
	return this->position;
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

/**
*@action : Permet de relier le jeu et le joueur
*@param  : L'instance du jeu
**/
void Joueur::setJeu(Jeu* j){
	this->jeu = j;
}

/**
*@action : Permet d'obtenir un pointeur sur l'instance du jeu courant
*@return : Un pointeur sur l'instance de Jeu
**/
Jeu* Joueur::getJeu(){
	return this->jeu;
}

/**
*@action : Execute les actions 
**/
void Joueur::executeCommande(){
    //jeu->miser((*this), 10);
}

void Joueur::changeDealer(){
	if(this->dealer){
		this->dealer = false;
	}else{
		this->dealer = true;
	}
}

int Joueur::getMiseJoueur(){
	return this->miseJoueur;
}

void Joueur::setMiseJoueur(int jetons){
	this->miseJoueur = jetons;
}

void Joueur::videMain(){
	this->main.clear();
}



