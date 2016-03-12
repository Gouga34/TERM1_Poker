#include "../../include/Jeu/Joueur.h"
#include "../../include/Jeu/Jeu.h" 

Joueur::Joueur(bool estDealer, int jetons, int position){
	this->dealer = estDealer;
	this->cave = jetons;
	this->position = position;
    this->miseCourante = 0;
    this->misePlusHaute = 0;
    this->miseTotale = 0;
    this->compteurActions = new int[3];
    for(int i =0; i<3;i++){
        this->compteurActions[i] = 0;
    }
}


Joueur::~Joueur(){

}

double Joueur::getChancesGain() const{
    return chancesGain;
}

void Joueur::setChancesGain(double nvChancesGain){
    chancesGain=nvChancesGain;
}

int Joueur::getCave() const{
	return this->cave;
}

int Joueur::getCaveDeDepart() const {
    return caveDeDepart;
}

void Joueur::setCave(int jetons){
    cave=jetons;
}

void Joueur::setCaveDeDepart(int cave) {
    caveDeDepart = cave;
}

int Joueur::getPosition() const{
	return this->position;
}


std::vector<game::Card> Joueur::getMain() const{
	return this->main;
}


void Joueur::ajouteJetons(int jetons){
	this->cave = this->cave + jetons;
}


void Joueur::retireJetons(int jetons){
	this->cave = this->cave - jetons;
}


void Joueur::ajouteCarte(game::Card carte){
	if(this->main.size() == 0){
		this->main.push_back(carte);
	}else{
        if(carte.getRank() == 1){
            game::Card c = this->main.at(0);
			this->main.pop_back();
			this->main.push_back(carte);
			this->main.push_back(c);
        }else if(this->main.at(0).getRank() == 1){
			this->main.push_back(carte);
        }else if(this->main.at(0).getRank() < carte.getRank()){
            game::Card c = this->main.at(0);
			this->main.pop_back();
			this->main.push_back(carte);
			this->main.push_back(c);
		}else{
			this->main.push_back(carte);
		}
	}
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

int Joueur::getMiseCourante() const{
    return this->miseCourante;
}

void Joueur::setMiseCourante(int jetons){
    this->miseCourante = jetons;
}

int Joueur::getMisePlusHaute() const{
    return this->misePlusHaute;
}

void Joueur::setMisePlusHaute(int jetons){
    this->misePlusHaute = jetons;
}

void Joueur::videMain(){
	this->main.clear();
}

void Joueur::resetCompteurActions(){
    for(int i = 0 ; i <3; i++){
        this->compteurActions[i] = 0;
    }
}

int* Joueur::getCompteurActions(){
    return this->compteurActions;
}

int Joueur::getCumulMisesEtRelances(){
    return this->cumulMisesEtRelances;
}

void Joueur::setCumulMisesEtRelances(int mise){
    this->cumulMisesEtRelances = mise;
}

int Joueur::getMiseTotale(){
    return this->miseTotale;
}

void Joueur::setMiseTotale(int mise){
    this->miseTotale = mise;
}
