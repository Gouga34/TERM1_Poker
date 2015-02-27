#include "../include/Joueur.h"
#include "../include/Jeu.h" 

Joueur::Joueur(bool estDealer, int jetons, int position){
	this->dealer = estDealer;
	this->cave = jetons;
	this->position = position;
	this->miseJoueur = 0;
}


Joueur::~Joueur(){

}

int Joueur::getCave() const{
	return this->cave;
}


int Joueur::getPosition() const{
	return this->position;
}


std::vector<Carte> Joueur::getMain() const{
	return this->main;
}


void Joueur::ajouteJetons(int jetons){
	this->cave = this->cave + jetons;
}


void Joueur::retireJetons(int jetons){
	this->cave = this->cave - jetons;
}


void Joueur::ajouteCarte(Carte carte){
	this->main.push_back(carte);
}


bool Joueur::estDealer(){
	return this->dealer;
}

void Joueur::resetMain(){
	this->main.clear();
}


void Joueur::setJeu(Jeu* j){
	this->jeu = j;
}


Jeu* Joueur::getJeu(){
	return this->jeu;
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



