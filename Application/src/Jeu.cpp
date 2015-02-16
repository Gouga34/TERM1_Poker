#include "../include/Jeu.h"

/**
*@param  : Le nombre de joueur, le montant de la blind de depart, la *	cave de depart des joueurs et le type de proffiling de l'IA
*@action : Initialise un nouveau jeu
**/
Jeu::Jeu(int nbJoueur, int blindDepart, int cave, int typeIA){
	srand((unsigned)time(0));
	this->initialisationTable(nbJoueur, cave);	
	this->deck = nouveauDeck(); 
	this->melange();
	this->blind = blindDepart;
	this->joueurCourant = 0;
	this->pot = 0;
	this->nombreDeCoup = 0;
}

/**
*@action : Destructeur de la classe Jeu
**/
Jeu::~Jeu(){

}

/**
*@param  : Le nombre de joueur et le montant de depart d leur cave
*@action : Cree les joueurs et les affectent au jeu
*@return : L'ensemble des joueurs de la partie
**/
void Jeu::initialisationTable(int nbJoueur, int cave){
	
	for(int i=0; i<nbJoueur; i++){
		Joueur joueur(false,cave);
		this->positionnement.push_back(joueur);
	}
}

/**
*@action : Distribue a chaque joueur ses cartes
**/
void Jeu::distributionMain(){

	int position;
	
	for(int i =0; i< 2*this->positionnement.size(); i++){
		position = rand() % deck.size();
		this->positionnement.at(i % this->positionnement.size()).ajouteCarte(this->deck.at(position));
		this->deck.erase(this->deck.begin() + position);
	}
}

/**
*@action : Distribue les trois premieres carte commune : le Flop, tirees aleatoirement dans le deck
**/
void Jeu::distributionFlop(){
	
	int position;
	
	for(int i=0; i<3; i++){
		position = rand() % deck.size();
		this->table.push_back(this->deck.at(position) );
		this->deck.erase(this->deck.begin() + position);
	}	
}

/**
*@action : Distribue la quatrieme carte : le Turn, tiree aleatoirement dans le deck
**/
void Jeu::distributionTurn(){

	int position;
	
	position = rand() % deck.size();
	this->table.push_back(this->deck.at(position) );
	this->deck.erase(this->deck.begin() + position);

}

/**
*@action : Distribue la cinquieme carte : la River, tiree aleatoirement dans le deck
**/
void Jeu::distributionRiver(){

	int position;
	
	position = rand() % deck.size();
	this->table.push_back(this->deck.at(position) );
	this->deck.erase(this->deck.begin() + position);

}

/**
*@action : Augmente le montant de la petite blind
**/
void Jeu::miseAJourBlind(){
	this->blind = this->blind * 2;
}

/**
*@action : Cree l'ensemble des cartes utilisees dans le jeu
*@return : Le "deck", l'ensemble du jeu de carte
**/
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
	

/**
*@action : Melange le jeu de carte
**/	
void Jeu::melange(){
	std::random_shuffle(this->deck.begin(), this->deck.end());
}

/**
*@action : Permet d'obtenir le montant de la petite blind
*@return : Le montant de la petite blind
**/
int Jeu::getBlind() const{
	return this->blind;
}

/**
*@action : Permet d'obtenir le joueur devant jouer
*@return : Le joueur courant
**/
int Jeu::getJoueurCourant() const{
	return this->joueurCourant;
}

/**
*@action : 
*@return : 
**/
Joueur Jeu::getJoueur(int i) const{
	return this->positionnement.at(i);
}

/**
*@action : 
*@return : 
**/
void Jeu::setJoueur(Joueur joueur){
	this->positionnement.push_back(joueur);
}
			
