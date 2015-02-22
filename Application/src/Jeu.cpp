#include "../include/Jeu.h"
#include "../include/Joueur.h"
#include "../include/IntelligenceArtificielle.h"

/**
*@param  : Le nombre de joueur, le montant de la blind de depart, la cave de depart des joueurs et le type de proffiling de l'IA
*@action : Initialise un nouveau jeu
**/
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

/**
*@action : Distribue a chaque joueur ses cartes
**/
void Jeu::distributionMain(){

	int position;
	
	this->resetActions();
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
	
	this->resetActions();
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
	
	this->resetActions();
	position = rand() % deck.size();
	this->table.push_back(this->deck.at(position) );
	this->deck.erase(this->deck.begin() + position);

}

/**
*@action : Distribue la cinquieme carte : la River, tiree aleatoirement dans le deck
**/
void Jeu::distributionRiver(){

	int position;
	
	this->resetActions();
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
*@action : Permet d'obtenir le joueur en i-eme position
*@return : Le joueur en i-eme position
**/
Joueur& Jeu::getJoueur(int i){
	return this->positionnement.at(i);
}

/**
*@action : Permet d'ajouter un joueur a la partie
*@param  : Le joueur a ajouter a la partie
**/
void Jeu::setJoueur(Joueur joueur){
	this->positionnement.push_back(joueur);
}

/**
*@action : Permet d'obtenir le pot de la partie
*@return : Le pot de la partie en cours
**/
int Jeu::getPot() const{
	return this->pot;
}

/**
*@action : Permet de modifier le pot de la partie courante
*@param  : Un entier representant la nouvelle valeur du pot
**/
void Jeu::setPot(int jetons){
	this->pot = jetons;
}


/**
*@action : Commande permettant a un joueur de miser
*@param  : Le joueur effectuant l'action ainsi que le montant de la mise
**/
void Jeu::miser(int posJoueur, int jetons){
	this->setPot(this->getPot() + jetons);
	this->getJoueur(posJoueur).retireJetons(jetons);
	this->mise = jetons;
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::MISER;
}

/**
*@action : Commande permettant a un joueur de relancer
*@param  : Le joueur effectuant l'action ainsi que le montant de la relance
**/
void Jeu::relancer(int posJoueur, int jetons){
	this->setPot(this->getPot() + jetons);
	this->getJoueur(posJoueur).retireJetons(jetons);
	this->mise = jetons;
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::RELANCER;
}

/**
*@action : Commande permettant a un joueur de suivre
*@param  : Le joueur effectuant l'action
**/
void Jeu::suivre(int posJoueur){
	this->setPot(this->getPot() +  this->mise);
    this->getJoueur(posJoueur).retireJetons(this->mise);
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::SUIVRE;
}

/**
*@action : Commande permettant a un joueur de checker
*@param  : Le joueur effectuant l'action
**/
void Jeu::checker(int posJoueur){
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::CHECKER;
}

/**
*@action : Commande permettant a un joueur de se coucher
*@param  : Le joueur effectuant l'action
**/
void Jeu::seCoucher(int posJoueur){
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::SE_COUCHER;
}

/**
*@action  : Permet de savoir si l'on est en début de tour
*@return  : Vrai si l'on se trouve en debut de tour, faux sinon
**/
bool Jeu::debutTour(){
	if(this->actions.size() == 0){
		return true;
	}else{
		return false;
	}
}

/**
*@action  : Permet de savoit si le tour est terminé
*@return  : Vrai si le tour est termnié, faux sinon
**/
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

/**
*@action  : Permet de connaitre l'action effectué par le joueur courant
*@return  : L'action effectué par le joueur courant
**/
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

