#include "../../include/Jeu/Jeu.h"
#include "../../include/IA/IntelligenceArtificielleProfilage.h"
#include "../../include/Interface/Logger.h"

#include <iterator>

Jeu::Jeu(int nbJoueur, int blindDepart, int cave) : actions(nbJoueur,ACTION::PAS_ENCORE_D_ACTION){
	srand((unsigned)time(0));
    this->initialisationTable(nbJoueur, cave);
    this->deck = nouveauDeck();
	this->melange();
	this->blind = blindDepart;
	this->joueurCourant = 0;
	this->pot = 0;
	this->nombreDeCoup = 0;
	this->dealer = 0;
    this->etape = ETAPE_JEU::PREFLOP;
    this->partieFinie = false;
}

Jeu::~Jeu(){
    for (int i = 0; i < listeJoueurs.size(); i++) {
        delete listeJoueurs.at(i);
    }
}

void Jeu::initialisationTable(int nbJoueur, int cave){

	for(int i=0; i<nbJoueur; i++){
		if( i == 0){
            Joueur *joueur = new IntelligenceArtificielle(true,cave,i);
            joueur->setJeu(this);
            this->listeJoueurs.push_back(joueur);
		}else{
            Joueur *ia = new IntelligenceArtificielleProfilage(false,cave,i);
            ia->setJeu(this);
            this->listeJoueurs.push_back(ia);
		}
	}
}


void Jeu::distributionMain(){

    this->partieFinie = false;

    this->resetActions();
	this->distributionBlind();

    for(int i =0; i< (int) (2*this->listeJoueurs.size()); i++){
       if(this->listeJoueurs.at(i % this->listeJoueurs.size())->getMain().size() != 2){
            int position = rand() % deck.size();
            this->listeJoueurs.at(i % this->listeJoueurs.size())->ajouteCarte(this->deck.at(position));
            this->deck.erase(this->deck.begin() + position);
        }
    }

    this->nouvelleEtape(ETAPE_JEU::PREFLOP);
}

void Jeu::nouvelleEtape(ETAPE_JEU etape){

    if(etape != ETAPE_JEU::PREFLOP){

    this->joueurCourant = (this->dealer + 1) % this->listeJoueurs.size();
    this->miseCourante = 0;
    cumulMisesEtRelances = 0;

    this->resetActions();

    int nbCartes = 0;
    if (etape == ETAPE_JEU::FLOP) {
        nbCartes = 3;
    }
    else if (etape == ETAPE_JEU::TURN || etape == ETAPE_JEU::RIVER) {
        nbCartes = 1;
    }

    if(this->tableTmp.empty()){
        for(int i=0; i<nbCartes; i++){
            if(deck.size() > 0){
                int position = rand() % deck.size();
                this->table.push_back(this->deck.at(position) );
                this->deck.erase(this->deck.begin() + position);
            }
        }
    }else{
        for(int i=0; i<nbCartes; i++){
                this->table.push_back(this->tableTmp.at(0));
                this->deck.erase(this->tableTmp.erase(this->tableTmp.begin()));
        }
    }
      }

    IntelligenceArtificielle *ia = static_cast<IntelligenceArtificielle*>(this->getJoueur(0));
    ia->estimationChancesDeGain();

    IntelligenceArtificielle *ia2 = static_cast<IntelligenceArtificielle*>(this->getJoueur(1));
    ia2->estimationChancesDeGain();
}

void Jeu::distributionBlind(){

    executerAction((this->getDealer() + 2) % this->listeJoueurs.size(), Action(MISER, getBlind()));
    //this->miser((this->getDealer() + 1) % this->listeJoueurs.size(), this->getBlind());
    this->actions[(this->getDealer() + 1) % this->listeJoueurs.size()] = ACTION::PETITE_BLIND;
	
    executerAction((this->getDealer() + 2) % this->listeJoueurs.size(), Action(RELANCER, getBlind()));
    //this->relancer((this->getDealer() + 2) % this->listeJoueurs.size(),this->getBlind() );
    this->actions[(this->getDealer() + 2) % this->listeJoueurs.size()] = ACTION::GROSSE_BLIND;
	
    this->joueurCourant = (this->getDealer() + 2)  % this->listeJoueurs.size();

    for(int i=0; i<this->listeJoueurs.size(); i++){
        this->getJoueur(i)->resetCompteurActions();
    }
}

int Jeu::getResultatPartie() const {
    return this->resultatPartie;
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

Joueur* Jeu::getJoueur(int i){
    return this->listeJoueurs.at(i);
}


void Jeu::setJoueur(Joueur *joueur){
    this->listeJoueurs.push_back(joueur);
}


int Jeu::getPot() const{
	return this->pot;
}


void Jeu::setPot(int jetons){
	this->pot = jetons;
}

void Jeu::jouerArgent(int posJoueur, int jetons) {

    setPot(getPot() + jetons);

    getJoueur(posJoueur)->setMiseCourante(jetons);
    miseCourante = getJoueur(posJoueur)->getMiseCourante();

    getJoueur(posJoueur)->setMiseTotale(getJoueur(posJoueur)->getMiseTotale() + jetons);

    cumulMisesEtRelances = getJoueur(posJoueur)->getMiseTotale();

    if (jetons > getJoueur(posJoueur)->getMisePlusHaute()) {
        getJoueur(posJoueur)->setMisePlusHaute(jetons);
    }

    getJoueur(posJoueur)->retireJetons(jetons);
}

void Jeu::miser(int posJoueur, int jetons){

    jouerArgent(posJoueur, jetons);
    this->actions[this->getJoueur(posJoueur)->getPosition()] = ACTION::MISER;
    this->getJoueur(posJoueur)->getCompteurActions()[0]++;
}

void Jeu::tapis(int posJoueur, ACTION action){

    jouerArgent(posJoueur, getJoueur(posJoueur)->getCave());
    this->actions[this->getJoueur(posJoueur)->getPosition()] = ACTION::TAPIS;

    if (action == MISER || action == RELANCER) {
        this->getJoueur(posJoueur)->getCompteurActions()[0]++;
    }
    else {
        this->getJoueur(posJoueur)->getCompteurActions()[1]++;
    }
}


void Jeu::relancer(int posJoueur, int jetons){

    //Si on fait pas tapis:
    if(jetons <= this->getJoueur(posJoueur)->getCave()){

        jouerArgent(posJoueur, jetons);
        this->actions[this->getJoueur(posJoueur)->getPosition()] = ACTION::RELANCER;
        this->getJoueur(posJoueur)->getCompteurActions()[0]++;

    }else{ //Sinon: tapis
        tapis(posJoueur, RELANCER);
    }
}


void Jeu::suivre(int posJoueur){

    // Le nombre de jetons à ajouter est le cumul moins le nombre de jetons déjà mis par le joueur
    int jetonsAAjouter = this->cumulMisesEtRelances - this->getJoueur(posJoueur)->getMiseTotale();

    // Si on a assez d'argent on suit
    if(this->getJoueur(posJoueur)->getCave() > jetonsAAjouter){

        jouerArgent(posJoueur, jetonsAAjouter);
        this->actions[this->getJoueur(posJoueur)->getPosition()] = ACTION::SUIVRE;
        this->getJoueur(posJoueur)->getCompteurActions()[1]++;
	}
    else{      // Sinon on fait tapis
        tapis(posJoueur, SUIVRE);
    }
}


void Jeu::checker(int posJoueur){

    this->actions[this->getJoueur(posJoueur)->getPosition()] = ACTION::CHECKER;
    this->getJoueur(posJoueur)->getCompteurActions()[2]++;
}

void Jeu::seCoucher(int posJoueur){

    this->actions[this->getJoueur(posJoueur)->getPosition()] = ACTION::SE_COUCHER;

    IntelligenceArtificielleProfilage *ia = static_cast<IntelligenceArtificielleProfilage*>(this->getJoueur(1));
    ia->remplissageDonneesProfilage();

    this->partieFinie = true;
    finPartie();
}    

bool Jeu::debutTour(){

    for(int i=0; i< (int) this->actions.size(); i++){
        if(this->actions.at(i) != ACTION::PAS_ENCORE_D_ACTION && this->actions.at(i) != ACTION::TAPIS ){
            return false;
        }
    }
        return true;
}


bool Jeu::finDuTour(){

	int i = 1;

    while(  i <= (int) this->listeJoueurs.size() - 1){

        if(this->actions.at( (this->getJoueurCourant() + i) % this->listeJoueurs.size() ) == ACTION::TAPIS
                && ( this->actions.at(this->getJoueurCourant() ) == ACTION::SUIVRE || this->actions.at(this->getJoueurCourant() ) == ACTION::TAPIS  ) ){
            return true;
        }

        if( this->actions.at( (this->getJoueurCourant() + i) % this->listeJoueurs.size() ) != ACTION::CHECKER
        &&  this->actions.at( (this->getJoueurCourant() + i) % this->listeJoueurs.size() ) != ACTION::SUIVRE
        &&  this->actions.at( (this->getJoueurCourant() + i) % this->listeJoueurs.size() ) != ACTION::SE_COUCHER){

			return false;
		}
		
		i++;
	}
	
    return (this->actions.at(this->getJoueurCourant()) != ACTION::PAS_ENCORE_D_ACTION && this->actions.at(this->getJoueurCourant()) != ACTION::GROSSE_BLIND);
}


ACTION Jeu::getAction() const{
    return this->actions.at(this->getJoueurCourant());
}

ETAPE_JEU Jeu::getEtape() const{
    return this->etape;
}

bool Jeu::prochainJoueur(){

    if(this->partieFinie){
        return false;
    }

    this->joueurCourant = (this->joueurCourant + 1) % this->listeJoueurs.size();

    if (this->finDuTour()) {

        this->getJoueur(0)->setMisePartie(this->getJoueur(0)->getMisePartie() + this->getJoueur(0)->getMiseTotale());
        this->getJoueur(1)->setMisePartie(this->getJoueur(1)->getMisePartie() + this->getJoueur(1)->getMiseTotale());

        IntelligenceArtificielleProfilage *ia = static_cast<IntelligenceArtificielleProfilage*>(this->getJoueur(1));

        ia->remplissageDonneesProfilage();


        // Fin de la partie
        if (this->getEtape() == ETAPE_JEU::RIVER || this->partieFinie) {
            this->partieFinie = true;
            finPartie();
            return false;
        } else {
            // On incrémente l'étape courante en passant par des entiers
            if (this->etape < ETAPE_JEU::NB_ETAPES) {
                this->etape = static_cast<ETAPE_JEU>(static_cast<int>(this->etape) + 1);
            }



            this->nouvelleEtape(this->getEtape());
        }
    }

    return true;
}

void Jeu::resetActions(){
    for(int i=0; i< (int) this->actions.size(); i++){

        if(this->actions.at(i) != ACTION::TAPIS){
            this->actions.at(i) = ACTION::PAS_ENCORE_D_ACTION;
        }

        this->getJoueur(i)->setMiseCourante(0);
        this->getJoueur(i)->setMisePlusHaute(0);
        this->getJoueur(i)->setMiseTotale(0);
        this->getJoueur(i)->resetCompteurActions();
    }
}

void Jeu::finPartie() {

    std::vector<Joueur*> joueursRestants;

    this->getJoueur(0)->setMisePartie(this->getJoueur(0)->getMisePartie() + this->getJoueur(0)->getMiseTotale());
    this->getJoueur(1)->setMisePartie(this->getJoueur(1)->getMisePartie() + this->getJoueur(1)->getMiseTotale());

    for(Joueur *joueur : this->listeJoueurs){
        if(this->estCouche(joueur->getPosition())){
            joueursRestants.push_back(joueur);
        }
    }

    int retour;
    //Si aucun des deux joueurs ne s'est couché:
    if(actions[0]!=ACTION::SE_COUCHER && actions[1]!=ACTION::SE_COUCHER){

        int comparaisonMains = Evaluateur::comparerMains(this->getTable(), this->getJoueur(0)->getMain(), this->getJoueur(1)->getMain());

        if(comparaisonMains == GAGNE){

            if(this->getJoueur(0)->getMisePartie() >= this->getJoueur(1)->getMisePartie()){
                 this->getJoueur(0)->ajouteJetons(this->getPot());
            }else{
                this->getJoueur(0)->ajouteJetons(this->getJoueur(0)->getMisePartie() *2);
                this->getJoueur(1)->ajouteJetons(this->getPot() - (this->getJoueur(0)->getMisePartie() *2) );
            }

            retour = GAGNE;
        }else if(comparaisonMains == PERDU){

            if(this->getJoueur(0)->getMisePartie() <= this->getJoueur(1)->getMisePartie() ){
                 this->getJoueur(1)->ajouteJetons(this->getPot());
            }else{
                this->getJoueur(1)->ajouteJetons(this->getJoueur(1)->getMisePartie() *2);
                this->getJoueur(0)->ajouteJetons(this->getPot() - (this->getJoueur(1)->getMisePartie() *2) );
            }
            retour = PERDU;
        }else{
            retour = EGALITE;

            this->getJoueur(0)->ajouteJetons(this->getJoueur(0)->getMisePartie());
            this->getJoueur(1)->ajouteJetons(this->getJoueur(1)->getMisePartie());
        }
    }else{ //Un joueur s'est couché
        if(actions[0]==ACTION::SE_COUCHER){
            retour = PERDU;
        }else{
            retour = GAGNE;
        }
        this->getJoueur(joueursRestants.at(0)->getPosition())->ajouteJetons(this->getPot());
    }

    resultatPartie = retour;

    IntelligenceArtificielleProfilage *ia = static_cast<IntelligenceArtificielleProfilage*>(this->getJoueur(1));
    ia->calculProfilGlobalJoueur();
    ia->ecritureScenariosDeTests();
    ia->getProfilage()->clear();
}

std::vector<Carte> Jeu::getTable() const{
    return this->table;
}

std::vector<ACTION>  Jeu::getListeActions() const{
    return this->actions;
}

int Jeu::getMiseCourante(){
    return this->miseCourante;
}

int Jeu::getCumulMisesEtRelances(){
    return cumulMisesEtRelances;
}

void Jeu::nouvelleMain(){

	this->setPot(0);
	this->table.clear();
    this->resetActions();
	
    for(int i =0; i< (int) this->listeJoueurs.size(); i++){
        this->getJoueur(i)->videMain();
        this->getJoueur(i)->setMisePartie(0);
	}
	
	this->deck = nouveauDeck();

    this->getJoueur(this->dealer)->changeDealer();
    this->dealer = (this->dealer + 1) % this->listeJoueurs.size();
    this->getJoueur(this->dealer)->changeDealer();

    this->etape = ETAPE_JEU::PREFLOP;
}


bool Jeu::peutChecker(int posJoueur){
    //On peut checker quand le joueur précédent a checké ou suivi.

    if(posJoueur==0){


        if(actions[1]==ACTION::TAPIS && this->debutTour()){
            return true;
        }

        //Si l'action de l'autre joueur est miser, relancer ou grosse blinde, on retourne false
        if(actions[1]==ACTION::MISER || actions[1]==ACTION::RELANCER || actions[1]==ACTION::GROSSE_BLIND){

            return false;
        }
    }
    else{

        if(actions[0]==ACTION::TAPIS && this->debutTour()){
            return true;
        }

        if(actions[0]==ACTION::MISER || actions[0]==ACTION::RELANCER || actions[0]==ACTION::GROSSE_BLIND){
            return false;
        }
    }
    return true;
}

bool Jeu::peutMiser(int posJoueur, int jetons){

    //On peut miser quand le joueur précédent a checké
    if(posJoueur==0){
        if(actions[1]==ACTION::MISER || actions[1]==ACTION::RELANCER || actions[1]==ACTION::GROSSE_BLIND || actions[1]==ACTION::SUIVRE || actions[1]==ACTION::TAPIS){
            return false;
        }
    }
    else{
        if(actions[0]==ACTION::MISER || actions[0]==ACTION::RELANCER || actions[0]==ACTION::GROSSE_BLIND || actions[0]==ACTION::SUIVRE || actions[0]==ACTION::TAPIS ){
            return false;
        }
    }

    if (jetons <= 0 || getJoueur(posJoueur)->getCave() < jetons) {
        return false;
    }

    return true;
}


bool Jeu::peutRelancer(int posJoueur, int jetons){

    //On peut pas relancer quand le joueur précédent a checké, n'as pas agit, a fait tapis ou a suivi.
    if(posJoueur==0){
        if(actions[1]==ACTION::CHECKER || actions[1]==ACTION::PAS_ENCORE_D_ACTION || actions[1]==ACTION::TAPIS || actions[1]==ACTION::SUIVRE){
            return false;
        }
    }else{
        if(actions[0]==ACTION::CHECKER || actions[0]==ACTION::PAS_ENCORE_D_ACTION || actions[0]==ACTION::TAPIS || actions[0]==ACTION::SUIVRE){
            return false;
        }
    }

    if(jetons>0 && getJoueur(posJoueur)->getCave()>=jetons){
        return true;
    }

    return false;
}

bool Jeu::peutSuivre(int posJoueur){

    //On peut suivre quand le joueur précédent a misé, relancé, grosse blind ou fait tapis

    if(posJoueur==0){
        if(actions[1]==ACTION::CHECKER || actions[1]==ACTION::PETITE_BLIND || actions[1]==ACTION::SUIVRE || actions[1]==ACTION::PAS_ENCORE_D_ACTION){
            return false;
        }
    }else{
        if(actions[0]==ACTION::CHECKER || actions[0]==ACTION::PETITE_BLIND || actions[0]==ACTION::SUIVRE || actions[0]==ACTION::PAS_ENCORE_D_ACTION){
            return false;
        }
    }

    return true;
}

bool Jeu::estCouche(int posJoueur) const {
    return (this->getListeActions().at(posJoueur) == ACTION::SE_COUCHER);
}

void Jeu::executerAction(int posJoueur, Action a){
    int relance;

    switch (a.getAction()) {
        case ACTION::CHECKER:
            if (peutChecker(posJoueur)) {
                checker(posJoueur);
            }
            break;

        case ACTION::MISER:
            if (peutMiser(posJoueur, a.getMontant())) {
                miser(posJoueur, a.getMontant());
            }
            break;

        case ACTION::SUIVRE:
            if (peutSuivre(posJoueur)) {
                suivre(posJoueur);
            }
            break;

        case ACTION::RELANCER:
        
            relance = (a.getMontant() < 2 * getMiseCourante()) ? 2 * getMiseCourante() : a.getMontant();

            if (peutRelancer(posJoueur, relance)) {
                relancer(posJoueur, relance);
            }
            break;

        case ACTION::SE_COUCHER:
            seCoucher(posJoueur);
            break;

        case ACTION::TAPIS:
            tapis(posJoueur, MISER);
            break;

        default:
            break;
    }
}

void Jeu::affectationCarte(std::vector<int> listeId){

    int pos = 0;

    for(int i=0; i< 9; i++){

        if(listeId.at(i) != -1){
            pos = 0;
            for(Carte carte : this->getDeck()){
                if(carte.getId() == listeId.at(i)){
                    if(i<2){
                        this->listeJoueurs.at(0)->ajouteCarte(this->deck.at(pos));
                        this->deck.erase(this->deck.begin() + pos);
                        pos--;
                    }else if (i<4){
                        this->listeJoueurs.at(1)->ajouteCarte(this->deck.at(pos));
                        this->deck.erase(this->deck.begin() + pos);
                        pos--;
                    }else{
                        this->tableTmp.push_back(this->deck.at(pos));
                        this->deck.erase(this->deck.begin() + pos);
                        pos--;
                    }
                }

                pos++;
            }
        }
    }
}

void Jeu::lancer()
{
    while (prochainJoueur()) {
        if (debutTour()) {
            Logger::getInstance()->ajoutLogs("--- Nouvelle Etape ---");
        }

        Action a = listeJoueurs.at(joueurCourant)->jouer();
        executerAction(joueurCourant, a);
    }
}

bool Jeu::aFaitTapis(){
    return actions[0] == ACTION::TAPIS || actions[1] == ACTION::TAPIS;
}

bool Jeu::peutJouer(int posJoueur){

    if(posJoueur == 0){
        if(this->actions[1] == ACTION::TAPIS && ( this->getJoueur(posJoueur)->getMiseTotale() > this->getJoueur(1)->getMiseTotale()) && this->actions[1] == ACTION::SUIVRE){
            return false;
        }else{
            return true;
        }
    }else{

        if(this->actions[0] == ACTION::TAPIS && ( this->getJoueur(posJoueur)->getMiseTotale() > this->getJoueur(0)->getMiseTotale()) && this->actions[0] == ACTION::SUIVRE){
            return false;
        }else{
            return true;
        }

    }

}

