#include "../include/Jeu.h"
#include "../include/Joueur.h"
#include "../include/IntelligenceArtificielle.h"

Jeu::Jeu(int nbJoueur, int blindDepart, int cave, int typeIA) : actions(nbJoueur,TYPES::ACTION_LIST::EN_ATTENTE){
	srand((unsigned)time(0));
	this->initialisationTable(nbJoueur, cave);	
	this->deck = nouveauDeck(); 
	this->melange();
	this->blind = blindDepart;
	this->joueurCourant = 0;
	this->pot = 0;
	this->nombreDeCoup = 0;
	this->dealer = 0;
}

Jeu::~Jeu(){

}

void Jeu::initialisationTable(int nbJoueur, int cave){
	
	for(int i=0; i<nbJoueur; i++){
		if( i == 0){
			Joueur joueur(true,cave,i);
			joueur.setJeu(this);
			this->positionnement.push_back(joueur);
		}else{
			Joueur joueur(false,cave,i);
			joueur.setJeu(this);
			this->positionnement.push_back(joueur);
		}
	}
}


void Jeu::distributionMain(){

	int position;
	
	this->resetActions();
	for(int i =0; i< 2*this->positionnement.size(); i++){
		position = rand() % deck.size();
		this->positionnement.at(i % this->positionnement.size()).ajouteCarte(this->deck.at(position));
		this->deck.erase(this->deck.begin() + position);
	}
}

void Jeu::distributionFlop(){
	
	int position;
	
	this->resetActions();
	for(int i=0; i<3; i++){
		position = rand() % deck.size();
		this->table.push_back(this->deck.at(position) );
		this->deck.erase(this->deck.begin() + position);
	}	
}


void Jeu::distributionTurn(){

	int position;
	
	this->resetActions();
	position = rand() % deck.size();
	this->table.push_back(this->deck.at(position) );
	this->deck.erase(this->deck.begin() + position);

}


void Jeu::distributionRiver(){

	int position;
	
	this->resetActions();
	position = rand() % deck.size();
	this->table.push_back(this->deck.at(position) );
	this->deck.erase(this->deck.begin() + position);

}


void Jeu::distributionBlind(){

	this->getJoueur( (this->getDealer() + 1) % this->positionnement.size() ).retireJetons(this->getBlind());
	this->setPot(this->getBlind());
	this->getJoueur( (this->getDealer() + 2) % this->positionnement.size() ).retireJetons(2 * this->getBlind());
	this->setPot(this->getPot() + 2*this->getBlind());
	this->joueurCourant = (this->getDealer() + 3)  % this->positionnement.size();
}


int Jeu::getDealer(){
	return this->dealer;
}


void Jeu::miseAJourBlind(){
	this->blind = this->blind * 2;
}


std::vector<Carte> Jeu::nouveauDeck(){
	std::vector<Carte> deck;
	
	for(int i = 0; i<4; i++){
		for(int j= 1; j<14; j++){
			Carte carte(j,i);
			deck.push_back(carte);
		}
	}
	return deck;
}
	
	
void Jeu::melange(){
	std::random_shuffle(this->deck.begin(), this->deck.end());
}


int Jeu::getBlind() const{
	return this->blind;
}


int Jeu::getJoueurCourant() const{
	return this->joueurCourant;
}


Joueur& Jeu::getJoueur(int i){
	return this->positionnement.at(i);
}


void Jeu::setJoueur(Joueur joueur){
	this->positionnement.push_back(joueur);
}


int Jeu::getPot() const{
	return this->pot;
}


void Jeu::setPot(int jetons){
	this->pot = jetons;
}



void Jeu::miser(int posJoueur, int jetons){
	this->setPot(this->getPot() + jetons);
	this->getJoueur(posJoueur).retireJetons(jetons);
	this->mise = jetons;
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::MISER;
}


void Jeu::relancer(int posJoueur, int jetons){
	this->setPot(this->getPot() + jetons);
	this->getJoueur(posJoueur).retireJetons(jetons);
	this->mise = jetons;
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::RELANCER;
}


void Jeu::suivre(int posJoueur){
	this->setPot(this->getPot() +  this->mise);
    this->getJoueur(posJoueur).retireJetons(this->mise);
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::SUIVRE;
}


void Jeu::checker(int posJoueur){
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::CHECKER;
}


void Jeu::seCoucher(int posJoueur){
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::SE_COUCHER;
}


bool Jeu::debutTour(){
	if(this->actions.size() == 0){
		return true;
	}else{
		return false;
	}
}


bool Jeu::finDuTour(){
	int i = 1;
	while( i <= this->positionnement.size() - 1){
		if( this->actions.at( (this->getJoueurCourant() + i) % this->positionnement.size() ) != TYPES::ACTION_LIST::CHECKER 
		&&  this->actions.at( (this->getJoueurCourant() + i) % this->positionnement.size() ) != TYPES::ACTION_LIST::SUIVRE
		&&  this->actions.at( (this->getJoueurCourant() + i) % this->positionnement.size() ) != TYPES::ACTION_LIST::SE_COUCHER){
			return false;
		}
		
		i++;
	}
	
	return true;
}


TYPES::ACTION_LIST Jeu::getAction() const{
	return this->actions.at(this->getJoueurCourant());
}

void Jeu::prochainJoueur(){
	if(this->finDuTour()){
		this->joueurCourant = this->dealer;
		if(this->table.size() == 0 ){
			this->distributionFlop();
		}else if (this->table.size() == 3){
			this->distributionTurn();
		}else{
			this->distributionRiver();
		}
	}else{
		this->joueurCourant = (this->joueurCourant + 1) % this->positionnement.size();
	}
}

void Jeu::resetActions(){
	for(int i=0; i<this->actions.size(); i++){
        this->actions.at(i) = TYPES::ACTION_LIST::EN_ATTENTE;
	}
}

std::vector<Carte> Jeu::getTable() const{
    return this->table;
}


int Jeu::getMise(){
	return this->mise;
}


bool Jeu::peutChecker(){

	for(int i=0; i<this->actions.size(); i++){
	
		if(this->actions.at(i) == TYPES::ACTION_LIST::MISER || this->actions.at(i) == TYPES::ACTION_LIST::RELANCER){
			return false;
		}
	
	}
	
	return true;
}

