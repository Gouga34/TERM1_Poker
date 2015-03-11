#include "../../include/Jeu/Jeu.h"
#include "../../include/Jeu/Joueur.h"
#include "../../include/Jeu/IntelligenceArtificielle.h"

#include <iterator>

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
	
	this->distributionBlind();
	
	for(int i =0; i< (int) (2*this->positionnement.size()); i++){
       if(this->positionnement.at(i % this->positionnement.size()).getMain().size() != 2){
            position = rand() % deck.size();
            this->positionnement.at(i % this->positionnement.size()).ajouteCarte(this->deck.at(position));
            this->deck.erase(this->deck.begin() + position);
        }
	}	

    std::cout << this->deck.size() << std::endl;
}

void Jeu::distributionFlop(){
	
	int position;
	
	this->mise = 0;
	
	for(int i=0; i< (int)this->positionnement.size(); i++){
		this->getJoueur(i).setMiseJoueur(0);
	}
	
	this->resetActions();
	for(int i=0; i<3; i++){
		position = rand() % deck.size();
		this->table.push_back(this->deck.at(position) );
		this->deck.erase(this->deck.begin() + position);
	}	
}


void Jeu::distributionTurn(){

	int position;
	
	this->mise = 0;
	
	for(int i=0; i< (int)this->positionnement.size(); i++){
		this->getJoueur(i).setMiseJoueur(0);
	}
	
	this->resetActions();
	position = rand() % deck.size();
	this->table.push_back(this->deck.at(position) );
	this->deck.erase(this->deck.begin() + position);

}


void Jeu::distributionRiver(){

	int position;
	
	this->mise = 0;
	
	for(int i=0; i< (int)this->positionnement.size(); i++){
		this->getJoueur(i).setMiseJoueur(0);
	}
	
	this->resetActions();
	position = rand() % deck.size();
	this->table.push_back(this->deck.at(position) );
	this->deck.erase(this->deck.begin() + position);

}


void Jeu::distributionBlind(){

	this->miser((this->getDealer() + 1) % this->positionnement.size(), this->getBlind());
	this->actions[(this->getDealer() + 1) % this->positionnement.size()] = TYPES::ACTION_LIST::PETITE_BLIND;
	
	this->relancer((this->getDealer() + 2) % this->positionnement.size(), 2 * this->getBlind() );
	this->actions[(this->getDealer() + 2) % this->positionnement.size()] = TYPES::ACTION_LIST::GROSSE_BLIND;
	
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

    for(int i = COULEUR_CARTE::PIQUE; i <= COULEUR_CARTE::CARREAU; i++ ){
        for(int j = RANG_CARTE::AS; j<=RANG_CARTE::K; j++){
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

std::vector<Carte> Jeu::getDeck(){
    return this->deck;
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

bool Jeu::miser(int posJoueur, int jetons){

	if(this->getJoueur(posJoueur).getCave() >= jetons){
		this->setPot(this->getPot() + jetons);
		this->getJoueur(posJoueur).retireJetons(jetons);
		this->mise = jetons;
		this->getJoueur(posJoueur).setMiseJoueur(jetons);
		this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::MISER;
	
        for(int i=1; i <= (int) this->positionnement.size() - 1 ; i++){
			this->getJoueur( (posJoueur + i) % this->positionnement.size()).setMiseJoueur(0);
		}
		
		return true;
	}
	
	return false;
	
}

bool Jeu::tapis(int posJoueur){

	this->setPot(this->getPot() + this->getJoueur(posJoueur).getCave());
	
	if(this->getJoueur(posJoueur).getCave() > this->mise){
		this->mise = this->getJoueur(posJoueur).getCave();
	}
	
	this->getJoueur(posJoueur).retireJetons(this->getJoueur(posJoueur).getCave());
	this->getJoueur(posJoueur).setMiseJoueur(this->getJoueur(posJoueur).getCave());
	
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::TAPIS;
	
	return true;
}


bool Jeu::relancer(int posJoueur, int jetons){
	
	if(this->getJoueur(posJoueur).getCave() >= jetons){
		this->setPot(this->getPot() + jetons);
		this->getJoueur(posJoueur).retireJetons(jetons);
        this->mise = this->getJoueur(posJoueur).getMiseJoueur() + jetons;
		this->getJoueur(posJoueur).setMiseJoueur(jetons);
		this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::RELANCER;
		return true;	
	}
	
	return false;

}


bool Jeu::suivre(int posJoueur){
	
	if(this->getJoueur(posJoueur).getCave() >= this->mise){
		this->setPot(this->getPot() + (this->mise - this->getJoueur(posJoueur).getMiseJoueur()));
        this->getJoueur(posJoueur).retireJetons(this->mise - this->getJoueur(posJoueur).getMiseJoueur());
	  	this->getJoueur(posJoueur).setMiseJoueur(this->mise);
		this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::SUIVRE;
		
		return true;
	}
	
	return this->tapis(posJoueur);
}


bool Jeu::checker(int posJoueur){

    if(this->peutChecker(posJoueur)){
        this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::CHECKER;
        return true;
    }else{
        return false;
    }

}

bool Jeu::seCoucher(int posJoueur){
	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::SE_COUCHER;
	return true;
}    

bool Jeu::debutTour(){

    for(int i=0; i< (int) this->actions.size(); i++){
        if(this->actions.at(i) != TYPES::ACTION_LIST::EN_ATTENTE){
            return false;
        }
    }
        return true;

}


bool Jeu::finDuTour(){

	int i = 1;

	while(  i <= (int) this->positionnement.size() - 1){
		if( this->actions.at( (this->getJoueurCourant() + i) % this->positionnement.size() ) != TYPES::ACTION_LIST::CHECKER 
		&&  this->actions.at( (this->getJoueurCourant() + i) % this->positionnement.size() ) != TYPES::ACTION_LIST::SUIVRE
		&&  this->actions.at( (this->getJoueurCourant() + i) % this->positionnement.size() ) != TYPES::ACTION_LIST::SE_COUCHER){
			return false;
		}
		
		i++;
	}
	
    return (this->actions.at(this->getJoueurCourant()) != TYPES::ACTION_LIST::EN_ATTENTE && this->actions.at(this->getJoueurCourant()) != TYPES::ACTION_LIST::GROSSE_BLIND);
}


TYPES::ACTION_LIST Jeu::getAction() const{
	return this->actions.at(this->getJoueurCourant());
}

bool Jeu::prochainJoueur(){
	
    this->joueurCourant = (this->joueurCourant + 1) % this->positionnement.size();
    
    if(this->finDuTour() && this->table.size() == 5){
        return false;
    }

    if(this->finDuTour() && this->table.size() != 5){
		this->joueurCourant = this->dealer;
		if(this->table.size() == 0 ){
			this->distributionFlop();
		}else if (this->table.size() == 3){
			this->distributionTurn();
        }else if (this->table.size() == 4){
			this->distributionRiver();
        }
    }

    return true;
}

void Jeu::resetActions(){
	for(int i=0; i< (int) this->actions.size(); i++){
        	this->actions.at(i) = TYPES::ACTION_LIST::EN_ATTENTE;
        	this->getJoueur(i).setMiseJoueur(0);
	}
}

std::vector<Carte> Jeu::getTable() const{
    return this->table;
}

std::vector<TYPES::ACTION_LIST>  Jeu::getListeActions() const{
    return this->actions;
}


int Jeu::getMise(){
	return this->mise;
}

void Jeu::nouvelleMain(int posJoueur){

    std::vector<Joueur> joueurRestant;

    for(Joueur joueur : this->positionnement){
        if(this->getListeActions().at(joueur.getPosition()) != TYPES::ACTION_LIST::SE_COUCHER){
            joueurRestant.push_back(joueur);
        }
    }

    if(joueurRestant.size() != 1){

        if(Evaluateur::comparerMains(this->getTable(), this->getJoueur(0).getMain(), this->getJoueur(1).getMain()) == GAGNE){
            this->getJoueur(0).ajouteJetons(this->getPot());
        }else if(Evaluateur::comparerMains(this->getTable(), this->getJoueur(0).getMain(), this->getJoueur(1).getMain()) == PERDU){
            this->getJoueur(1).ajouteJetons(this->getPot());
        }else{
            if(this->getPot() % 2 == 0){
                this->getJoueur(0).ajouteJetons(this->getPot() / 2 );
                this->getJoueur(1).ajouteJetons(this->getPot() / 2 );
            }else{
                this->setPot(this->getPot() -1);
                this->getJoueur(0).ajouteJetons(this->getPot() / 2 );
                this->getJoueur(1).ajouteJetons(this->getPot() / 2 );
            }
        }
    }else{
        this->getJoueur(joueurRestant.at(0).getPosition()).ajouteJetons(this->getPot());
    }

	this->setPot(0);
	this->table.clear();
	
    for(int i =0; i< (int) this->positionnement.size(); i++){
		this->getJoueur(i).videMain();
	}
	
	this->deck = nouveauDeck();
	
	this->getJoueur(this->dealer).changeDealer();
	this->dealer = (this->dealer + 1) % this->positionnement.size();
	this->getJoueur(this->dealer).changeDealer();
	
}


bool Jeu::peutChecker(int posJoueur){

	for(int i=1; i<= (int) this->actions.size() - 1; i++){
		if(this->actions[(posJoueur + i) % this->actions.size() ] == TYPES::ACTION_LIST::MISER || this->actions[(posJoueur + i) % this->actions.size()] == TYPES::ACTION_LIST::RELANCER || this->actions[(posJoueur + i) % this->actions.size()] == TYPES::ACTION_LIST::GROSSE_BLIND){ 
			return false;
		}
	}
	
	return true;
}

bool Jeu::peutMiser(int posJoueur){

    for(int i=1; i<= (int) this->actions.size() - 1; i++){
        if(this->actions[(posJoueur + i) % this->actions.size() ] == TYPES::ACTION_LIST::MISER ||
           this->actions[(posJoueur + i) % this->actions.size()] == TYPES::ACTION_LIST::RELANCER ||
           this->actions[(posJoueur + i) % this->actions.size()] == TYPES::ACTION_LIST::GROSSE_BLIND ||
           this->actions[(posJoueur + i) % this->actions.size()] == TYPES::ACTION_LIST::TAPIS){
            return false;
        }
    }

    return true;
}


bool Jeu::peutRelancer(int posJoueur){

    for(int i=1; i<= (int) this->actions.size() - 1; i++){
        if(this->actions[(posJoueur + i) % this->actions.size() ] != TYPES::ACTION_LIST::MISER ||
            this->actions[(posJoueur + i) % this->actions.size()] != TYPES::ACTION_LIST::RELANCER ||
            this->actions[(posJoueur + i) % this->actions.size()] != TYPES::ACTION_LIST::GROSSE_BLIND ){
                return false;
        }
    }

    return true;
}

bool Jeu::peutSuivre(int posJoueur){

    for(int i=1; i<= (int) this->actions.size() - 1; i++){
        if(this->actions[(posJoueur + i) % this->actions.size() ] != TYPES::ACTION_LIST::MISER ||
            this->actions[(posJoueur + i) % this->actions.size()] != TYPES::ACTION_LIST::RELANCER ||
            this->actions[(posJoueur + i) % this->actions.size()] != TYPES::ACTION_LIST::GROSSE_BLIND ||
            this->actions[(posJoueur + i) % this->actions.size()] != TYPES::ACTION_LIST::SUIVRE  ){
                return false;
        }
    }

    return true;
}


void Jeu::affecteMainIA(std::vector<int> listeId){

    int pos = 0;

    for(Carte carte : this->getDeck()){
        if(carte.getId() == listeId.at(0) || carte.getId() == listeId.at(1)){
            this->positionnement.at(1).ajouteCarte(this->deck.at(pos));
            this->deck.erase(this->deck.begin() + pos);
            pos--;
        }

        pos++;

    }
}






























