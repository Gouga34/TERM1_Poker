#include "../../include/Jeu/Jeu.h"

#include <iterator>

Jeu::Jeu(int nbJoueur, int blindDepart, int cave, double agressivite, double rationalite) : actions(nbJoueur,TYPES::ACTION_LIST::EN_ATTENTE){
	srand((unsigned)time(0));
	this->initialisationTable(nbJoueur, cave);	
	this->deck = nouveauDeck(); 
	this->melange();
	this->blind = blindDepart;
	this->joueurCourant = 0;
	this->pot = 0;
	this->nombreDeCoup = 0;
	this->dealer = 0;
    this->agressiviteIA = agressivite;
   	this->rationaliteIA = rationalite;
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
}

void Jeu::distributionFlop(){
	

    int position;
	
	this->mise = 0;
	
	for(int i=0; i< (int)this->positionnement.size(); i++){
        this->getJoueur(i).setMisePlusHauteJoueur(0);
        this->getJoueur(i).resetCompteurActions();
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
        this->getJoueur(i).setMisePlusHauteJoueur(0);
        this->getJoueur(i).resetCompteurActions();
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
        this->getJoueur(i).setMisePlusHauteJoueur(0);
        this->getJoueur(i).resetCompteurActions();
	}
	
	this->resetActions();
	position = rand() % deck.size();
	this->table.push_back(this->deck.at(position) );
	this->deck.erase(this->deck.begin() + position);

}


void Jeu::distributionBlind(){

	this->miser((this->getDealer() + 1) % this->positionnement.size(), this->getBlind());
	this->actions[(this->getDealer() + 1) % this->positionnement.size()] = TYPES::ACTION_LIST::PETITE_BLIND;
	
    this->relancer((this->getDealer() + 2) % this->positionnement.size(),this->getBlind() );
	this->actions[(this->getDealer() + 2) % this->positionnement.size()] = TYPES::ACTION_LIST::GROSSE_BLIND;
	
	this->joueurCourant = (this->getDealer() + 3)  % this->positionnement.size();

    for(int i=0; i<this->positionnement.size(); i++){
        this->getJoueur(i).resetCompteurActions();
    }
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
        this->getJoueur(posJoueur).setMisePlusHauteJoueur(jetons);
        this->getJoueur(posJoueur).setMiseTotaleJoueur(this->getJoueur(posJoueur).getMiseTotaleJoueur() + jetons);
		this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::MISER;
	
        for(int i=1; i <= (int) this->positionnement.size() - 1 ; i++){
            this->getJoueur( (posJoueur + i) % this->positionnement.size()).setMisePlusHauteJoueur(0);
		}

        this->getJoueur(posJoueur).getCompteurActions()[0]++;
		
		return true;
	}
	
	return false;
	
}

bool Jeu::tapis(int posJoueur){

	this->setPot(this->getPot() + this->getJoueur(posJoueur).getCave());
	
	if(this->getJoueur(posJoueur).getCave() > this->mise){
		this->mise = this->getJoueur(posJoueur).getCave();
	}
	
    if(this->getJoueur(posJoueur).getCave() > this->getJoueur(posJoueur).getMisePlusHauteJoueur()){
            this->getJoueur(posJoueur).setMisePlusHauteJoueur(this->getJoueur(posJoueur).getCave());
    }

    this->getJoueur(posJoueur).setMiseTotaleJoueur(this->getJoueur(posJoueur).getMiseTotaleJoueur() + this->getJoueur(posJoueur).getCave());
	
    this->getJoueur(posJoueur).retireJetons(this->getJoueur(posJoueur).getCave());

	this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::TAPIS;

    this->getJoueur(posJoueur).getCompteurActions()[0]++;

	
	return true;
}


bool Jeu::relancer(int posJoueur, int jetons){
	
	if(this->getJoueur(posJoueur).getCave() >= jetons){

        this->setPot(this->getPot() +  (this->getMise() - this->getJoueur(posJoueur).getMisePlusHauteJoueur()));
        this->getJoueur(posJoueur).setMiseTotaleJoueur(this->getJoueur(posJoueur).getMiseTotaleJoueur() + (this->getMise() - this->getJoueur(posJoueur).getMisePlusHauteJoueur()));

        this->getJoueur(posJoueur).retireJetons(this->getMise() - this->getJoueur(posJoueur).getMisePlusHauteJoueur());

		this->setPot(this->getPot() + jetons);
		this->getJoueur(posJoueur).retireJetons(jetons);

        //this->mise = this->getJoueur(posJoueur).getMiseJoueur() + jetons;
        this->mise = this->getMise() + jetons;
        this->getJoueur(posJoueur).setMiseTotaleJoueur(this->getJoueur(posJoueur).getMiseTotaleJoueur() + jetons);
        this->getJoueur(posJoueur).setMisePlusHauteJoueur(jetons);
		this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::RELANCER;
        this->getJoueur(posJoueur).getCompteurActions()[0]++;
		return true;	
	}
	
	return false;

}


bool Jeu::suivre(int posJoueur){
	
	if(this->getJoueur(posJoueur).getCave() >= this->mise){
        this->setPot(this->getPot() + (this->mise - this->getJoueur(posJoueur).getMisePlusHauteJoueur()));
        this->getJoueur(posJoueur).setMiseTotaleJoueur(this->getJoueur(posJoueur).getMiseTotaleJoueur() + (this->mise - this->getJoueur(posJoueur).getMisePlusHauteJoueur()));
        this->getJoueur(posJoueur).retireJetons(this->mise - this->getJoueur(posJoueur).getMisePlusHauteJoueur());
        this->getJoueur(posJoueur).setMisePlusHauteJoueur(this->mise);
		this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::SUIVRE;
        this->getJoueur(posJoueur).getCompteurActions()[1]++;
		return true;
	}
	
	return this->tapis(posJoueur);
}


bool Jeu::checker(int posJoueur){

    if(this->peutChecker(posJoueur)){
        this->actions[this->getJoueur(posJoueur).getPosition()] = TYPES::ACTION_LIST::CHECKER;
        this->getJoueur(posJoueur).getCompteurActions()[2]++;
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


//TODO :: faire une methode remplissant le tableau apres la river

void Jeu::remplissageTableau(std::vector<Carte> mainJoueur, std::vector<Carte> table){


}

bool Jeu::prochainJoueur(){
	
    this->joueurCourant = (this->joueurCourant + 1) % this->positionnement.size();

    int nbTotalActions = 0;

    for(int i = 0; i<3; i++){
        nbTotalActions += this->getJoueur(0).getCompteurActions()[i];
    }

    Profilage profilJoueur = *this->getJoueur(0).getProfil();

    if(this->finDuTour() && this->table.size() == 5){

        profilJoueur.charger();

        profilJoueur.profil[profilJoueur.RIVER].probaGainAdversaire = 100 * EstimationProba::estimation(this, &this->getJoueur(0));
        profilJoueur.profil[profilJoueur.RIVER].tauxMises = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[0],nbTotalActions);
        profilJoueur.profil[profilJoueur.RIVER].tauxSuivis = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[1],nbTotalActions);
        profilJoueur.profil[profilJoueur.RIVER].tauxChecks = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[2],nbTotalActions);

        profilJoueur.profil[profilJoueur.RIVER].misePlusHaute = CalculDonneesProfilage::taux(this->getJoueur(0).getMisePlusHauteJoueur(),this->getJoueur(0).getCave());
        profilJoueur.profil[profilJoueur.RIVER].miseTotaleJoueur = CalculDonneesProfilage::taux(this->getJoueur(0).getMiseTotaleJoueur(),this->getJoueur(0).getCave());

        profilJoueur.profil[profilJoueur.RIVER].tauxAgressivite = CalculDonneesProfilage::agressivite(profilJoueur.profil[profilJoueur.PREFLOP].misePlusHaute,profilJoueur.profil[profilJoueur.PREFLOP].tauxMises,profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur);
        profilJoueur.profil[profilJoueur.RIVER].tauxRationnalite = CalculDonneesProfilage::rationalite(profilJoueur.profil[profilJoueur.PREFLOP].probaGainAdversaire,profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur);
        profilJoueur.profil[profilJoueur.RIVER].tauxPassivite = CalculDonneesProfilage::passivite( profilJoueur.profil[profilJoueur.PREFLOP].tauxSuivis, profilJoueur.profil[profilJoueur.PREFLOP].tauxChecks);
        profilJoueur.profil[profilJoueur.RIVER].tauxBluff = CalculDonneesProfilage::bluff(profilJoueur.profil[profilJoueur.PREFLOP].tauxRationnalite);

        profilJoueur.profil[profilJoueur.RIVER].pot = this->getPot();
        profilJoueur.sauvegarder();

        return false;
    }

    if(this->finDuTour() && this->table.size() != 5){

		this->joueurCourant = this->dealer;
		if(this->table.size() == 0 ){
            profilJoueur.charger();

            profilJoueur.profil[profilJoueur.PREFLOP].probaGainAdversaire = 100 * EstimationProba::estimation(this, &this->getJoueur(0));

            profilJoueur.profil[profilJoueur.PREFLOP].tauxMises = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[0],nbTotalActions);
            profilJoueur.profil[profilJoueur.PREFLOP].tauxSuivis = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[1],nbTotalActions);
            profilJoueur.profil[profilJoueur.PREFLOP].tauxChecks = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[2],nbTotalActions);

            profilJoueur.profil[profilJoueur.PREFLOP].misePlusHaute = CalculDonneesProfilage::taux(this->getJoueur(0).getMisePlusHauteJoueur(),this->getJoueur(0).getCave());
            profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur = CalculDonneesProfilage::taux(this->getJoueur(0).getMiseTotaleJoueur(),this->getJoueur(0).getCave());

            profilJoueur.profil[profilJoueur.PREFLOP].tauxAgressivite = CalculDonneesProfilage::agressivite(profilJoueur.profil[profilJoueur.PREFLOP].misePlusHaute,profilJoueur.profil[profilJoueur.PREFLOP].tauxMises,profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur);
            profilJoueur.profil[profilJoueur.PREFLOP].tauxRationnalite = CalculDonneesProfilage::rationalite(profilJoueur.profil[profilJoueur.PREFLOP].probaGainAdversaire,profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur);
            profilJoueur.profil[profilJoueur.PREFLOP].tauxPassivite = CalculDonneesProfilage::passivite( profilJoueur.profil[profilJoueur.PREFLOP].tauxSuivis, profilJoueur.profil[profilJoueur.PREFLOP].tauxChecks);
            profilJoueur.profil[profilJoueur.PREFLOP].tauxBluff = CalculDonneesProfilage::bluff(profilJoueur.profil[profilJoueur.PREFLOP].tauxRationnalite);

            profilJoueur.profil[profilJoueur.PREFLOP].pot = this->getPot();
            profilJoueur.sauvegarder();

			this->distributionFlop();
		}else if (this->table.size() == 3){
            profilJoueur.charger();

            profilJoueur.profil[profilJoueur.FLOP].probaGainAdversaire = 100 * EstimationProba::estimation(this, &this->getJoueur(0));
            profilJoueur.profil[profilJoueur.FLOP].tauxMises = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[0],nbTotalActions);
            profilJoueur.profil[profilJoueur.FLOP].tauxSuivis = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[1],nbTotalActions);
            profilJoueur.profil[profilJoueur.FLOP].tauxChecks = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[2],nbTotalActions);

            profilJoueur.profil[profilJoueur.FLOP].misePlusHaute = CalculDonneesProfilage::taux(this->getJoueur(0).getMisePlusHauteJoueur(),this->getJoueur(0).getCave());
            profilJoueur.profil[profilJoueur.FLOP].miseTotaleJoueur = CalculDonneesProfilage::taux(this->getJoueur(0).getMiseTotaleJoueur(),this->getJoueur(0).getCave());

            profilJoueur.profil[profilJoueur.FLOP].tauxAgressivite = CalculDonneesProfilage::agressivite(profilJoueur.profil[profilJoueur.PREFLOP].misePlusHaute,profilJoueur.profil[profilJoueur.PREFLOP].tauxMises,profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur);
            profilJoueur.profil[profilJoueur.FLOP].tauxRationnalite = CalculDonneesProfilage::rationalite(profilJoueur.profil[profilJoueur.PREFLOP].probaGainAdversaire,profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur);
            profilJoueur.profil[profilJoueur.FLOP].tauxPassivite = CalculDonneesProfilage::passivite( profilJoueur.profil[profilJoueur.PREFLOP].tauxSuivis, profilJoueur.profil[profilJoueur.PREFLOP].tauxChecks);
            profilJoueur.profil[profilJoueur.FLOP].tauxBluff = CalculDonneesProfilage::bluff(profilJoueur.profil[profilJoueur.PREFLOP].tauxRationnalite);

            profilJoueur.profil[profilJoueur.FLOP].pot = this->getPot();

            profilJoueur.sauvegarder();

			this->distributionTurn();
        }else if (this->table.size() == 4){

            profilJoueur.charger();

            profilJoueur.profil[profilJoueur.TURN].probaGainAdversaire = 100 * EstimationProba::estimation(this, &this->getJoueur(0));

            profilJoueur.profil[profilJoueur.TURN].tauxMises = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[0],nbTotalActions);
            profilJoueur.profil[profilJoueur.TURN].tauxSuivis = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[1],nbTotalActions);
            profilJoueur.profil[profilJoueur.TURN].tauxChecks = CalculDonneesProfilage::taux(this->getJoueur(0).getCompteurActions()[2],nbTotalActions);

            profilJoueur.profil[profilJoueur.TURN].misePlusHaute = CalculDonneesProfilage::taux(this->getJoueur(0).getMisePlusHauteJoueur(),this->getJoueur(0).getCave());
            profilJoueur.profil[profilJoueur.TURN].miseTotaleJoueur = CalculDonneesProfilage::taux(this->getJoueur(0).getMiseTotaleJoueur(),this->getJoueur(0).getCave());

            profilJoueur.profil[profilJoueur.TURN].probaGainAdversaire = 100 * EstimationProba::estimation(this, &this->getJoueur(0));

            profilJoueur.profil[profilJoueur.TURN].tauxAgressivite = CalculDonneesProfilage::agressivite(profilJoueur.profil[profilJoueur.PREFLOP].misePlusHaute,profilJoueur.profil[profilJoueur.PREFLOP].tauxMises,profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur);
            profilJoueur.profil[profilJoueur.TURN].tauxRationnalite = CalculDonneesProfilage::rationalite(profilJoueur.profil[profilJoueur.PREFLOP].probaGainAdversaire,profilJoueur.profil[profilJoueur.PREFLOP].miseTotaleJoueur);
            profilJoueur.profil[profilJoueur.TURN].tauxPassivite = CalculDonneesProfilage::passivite( profilJoueur.profil[profilJoueur.PREFLOP].tauxSuivis, profilJoueur.profil[profilJoueur.PREFLOP].tauxChecks);
            profilJoueur.profil[profilJoueur.TURN].tauxBluff = CalculDonneesProfilage::bluff(profilJoueur.profil[profilJoueur.PREFLOP].tauxRationnalite);

            profilJoueur.profil[profilJoueur.TURN].pot = this->getPot();

            profilJoueur.sauvegarder();

			this->distributionRiver();
        }
    }

    return true;
}

void Jeu::resetActions(){
	for(int i=0; i< (int) this->actions.size(); i++){
        	this->actions.at(i) = TYPES::ACTION_LIST::EN_ATTENTE;
            this->getJoueur(i).setMisePlusHauteJoueur(0);
        this->getJoueur(i).setMiseTotaleJoueur(0);
	}
}

std::vector<Carte> Jeu::getTable() const{
    return this->table;
}

std::vector<TYPES::ACTION_LIST>  Jeu::getListeActions() const{
    return this->actions;
}

double Jeu::getAgressiviteIA() const{
    return this->agressiviteIA;
}

double Jeu::getRationaliteIA() const{
    return this->rationaliteIA;
}

void Jeu::setAgressiviteIA(double agressivite){
    this->agressiviteIA = agressivite;
}

void Jeu::setRationaliteIA(double rationalite){
    this->rationaliteIA = rationalite;
}


int Jeu::getMise(){
	return this->mise;
}

void Jeu::nouvelleMain(){

    std::vector<Joueur> joueurRestant;

    for(Joueur joueur : this->positionnement){
        if(this->getListeActions().at(joueur.getPosition()) != TYPES::ACTION_LIST::SE_COUCHER){
            joueurRestant.push_back(joueur);
        }
    }

    if(joueurRestant.size() != 1){

        if(Evaluateur::comparerMains(this->getTable(), this->getJoueur(0).getMain(), this->getJoueur(1).getMain()) == GAGNE){
            std::cout << "Humain gagne " << std::endl;
            this->getJoueur(0).ajouteJetons(this->getPot());
        }else if(Evaluateur::comparerMains(this->getTable(), this->getJoueur(0).getMain(), this->getJoueur(1).getMain()) == PERDU){
            std::cout << "IA gagne " << std::endl;
            this->getJoueur(1).ajouteJetons(this->getPot());
        }else{
            std::cout << "Egalite " << std::endl;
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
           this->actions[(posJoueur + i) % this->actions.size()] == TYPES::ACTION_LIST::SUIVRE||
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


void Jeu::affectationCarte(std::vector<int> listeId){

    int pos = 0;

    for(int i=0; i< (int) listeId.size(); i++){
        pos = 0;
        for(Carte carte : this->getDeck()){
            if(carte.getId() == listeId.at(i)){
                if(i<2){
                    this->positionnement.at(1).ajouteCarte(this->deck.at(pos));
                    this->deck.erase(this->deck.begin() + pos);
                    pos--;
                }else if (i<4){
                    this->positionnement.at(0).ajouteCarte(this->deck.at(pos));
                    this->deck.erase(this->deck.begin() + pos);
                    pos--;
                }else{
                    this->table.push_back(this->deck.at(pos));
                    this->deck.erase(this->deck.begin() + pos);
                    pos--;
                }
            }

            pos++;
        }
    }
}

void Jeu::setPseudo(std::string pseudo){
    this->getJoueur(0).setProfil(pseudo);
}





























