#include "../../include/Jeu/Jeu.h"
#include "../../include/IA/IntelligenceArtificielleProfilage.h"
#include "../../include/Interface/Logger.h"

#include <iterator>

Jeu::Jeu(int nbJoueur, int blindDepart, OptionsJeu opt) : actions(nbJoueur) {
	srand((unsigned)time(0));

    options = opt;

    this->deck = nouveauDeck();
    this->melangeDeck();
	this->blind = blindDepart;
    this->joueurCourant = 0;
	this->pot = 0;
    this->dealer = 0;
}

Jeu::~Jeu(){
    for (unsigned int i = 0; i < listeJoueurs.size(); i++) {
        delete listeJoueurs.at(i);
    }
}

void Jeu::addJoueur(Joueur *joueur) {
    joueur->setJeu(this);
    listeJoueurs.push_back(joueur);
}

void Jeu::nouvellePartie() {
    reinitialisationCaves();
    distributionMain();
}

void Jeu::reinitialisationCaves() {

    setPot(0);

    for (unsigned int i = 0; i < listeJoueurs.size(); i++) {
        if (options.reinitialisationCaves) {
            getJoueur(i)->setCave(CAVE_JOUEURS);
        }
        else {
            if (getJoueur(i)->getCave() == 0) {
                getJoueur(i)->setCave(CAVE_JOUEURS);
            }
        }

        getJoueur(i)->setCaveDeDepart(getJoueur(i)->getCave());
    }
}

void Jeu::distributionMain(){

    etape = ETAPE_JEU::PREFLOP;
    partieFinie = false;

    this->resetActions();

    IntelligenceArtificielleProfilage *iaProfilage = static_cast<IntelligenceArtificielleProfilage*>(listeJoueurs.at(1));
    iaProfilage->determinerTypeDeJeu();

    getJoueur(0)->setCumulMisesEtRelances(0);
    getJoueur(1)->setCumulMisesEtRelances(0);



    for(int i =0; i< (int) (2*this->listeJoueurs.size()); i++){
       if(this->listeJoueurs.at(i % this->listeJoueurs.size())->getMain().size() != 2){
            int position = rand() % deck.size();
            this->listeJoueurs.at(i % this->listeJoueurs.size())->ajouteCarte(this->deck.at(position));
            this->deck.erase(this->deck.begin() + position);
        }
    }

    this->nouvelleEtape(ETAPE_JEU::PREFLOP);
    this->distributionBlind();
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

        distributionCartesTable(nbCartes);

        Logger::getInstance()->ajoutLogs("Ajout de cartes sur la table");
    }

    calculChancesDeGain();
}

void Jeu::calculChancesDeGain() {

    IntelligenceArtificielle *ia;
    IntelligenceArtificielle *ia2 = static_cast<IntelligenceArtificielle*>(this->getJoueur(1));

    if (!getJoueur(0)->estHumain()) {
        ia = static_cast<IntelligenceArtificielle*>(this->getJoueur(0));
        ia->lancerEstimationChancesDeGain(NOMBRE_DE_TESTS, 2);
    }

    ia2->lancerEstimationChancesDeGain(NOMBRE_DE_TESTS, 2);
    ia2->attendreResultatEstimation();

    if (!getJoueur(0)->estHumain()) {
        ia->attendreResultatEstimation();
    }
}

void Jeu::distributionCartesTable(int nbCartesADistribuer){

    if(this->tableTmp.empty()){
        for(int i=0; i<nbCartesADistribuer; i++){
            if(deck.size() > 0){
                int position = rand() % deck.size();
                this->table.push_back(this->deck.at(position) );
                this->deck.erase(this->deck.begin() + position);
            }
        }
    }else{
        for(int i=0; i<nbCartesADistribuer; i++){
                this->table.push_back(this->tableTmp.at(0));
                this->deck.erase(this->tableTmp.erase(this->tableTmp.begin()));
        }
    }
}

void Jeu::distributionBlind(){

    Logger::getInstance()->ajoutLogs("Joueur " + QString::number(getPositionJoueurAdverse(getDealer())) + " : petite blind");

    executerAction(getPositionJoueurAdverse(getDealer()), Action(MISER, getBlind()));
    actions[getPositionJoueurAdverse(getDealer())].back() = ACTION::PETITE_BLIND;

    Logger::getInstance()->ajoutLogs("Joueur " + QString::number(getDealer()) + " : grosse blind");

    executerAction(getDealer(), Action(RELANCER, getBlind()*2));
    actions[getDealer()].back() = ACTION::GROSSE_BLIND;

    this->joueurCourant = getDealer();

    for(unsigned int i=0; i<listeJoueurs.size(); i++){
        this->getJoueur(i)->resetCompteurActions();
    }
}

OptionsJeu Jeu::getOptions() const {
    return options;
}

RESULTAT_PARTIE Jeu::getResultatPartie() const {
    return this->resultatPartie;
}

int Jeu::getDealer(){
	return this->dealer;
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
	
	
void Jeu::melangeDeck(){
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

int Jeu::getPositionJoueurAdverse(int joueur) const{
    return (joueur == 0) ? 1 : 0;
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
    getJoueur(posJoueur)->setCumulMisesEtRelances(getJoueur(posJoueur)->getCumulMisesEtRelances() + jetons);

    if (jetons > getJoueur(posJoueur)->getMisePlusHaute()) {
        getJoueur(posJoueur)->setMisePlusHaute(jetons);
    }
    //std::cout<<"Joueur "<<posJoueur<<" - retrait de "<<jetons<<" jetons"<<std::endl;
    getJoueur(posJoueur)->retireJetons(jetons);
}

void Jeu::miser(int posJoueur, int jetons){

    //Si on fait pas tapis:
    if(jetons < this->getJoueur(posJoueur)->getCave()){
        jouerArgent(posJoueur, jetons);
        miseCourante = jetons;
        cumulMisesEtRelances = jetons;

        this->actions[this->getJoueur(posJoueur)->getPosition()].push_back(ACTION::MISER);
        this->getJoueur(posJoueur)->getCompteurActions()[0]++;
    }
    else{ //Sinon: tapis
        tapis(posJoueur, MISER);
    }
}

void Jeu::tapis(int posJoueur, ACTION action){

    jouerArgent(posJoueur, getJoueur(posJoueur)->getCave());
    this->actions[this->getJoueur(posJoueur)->getPosition()].push_back(ACTION::TAPIS);

    miseCourante = getJoueur(posJoueur)->getCave();
    cumulMisesEtRelances = getJoueur(posJoueur)->getCumulMisesEtRelances();

    if (action == MISER || action == RELANCER) {
        this->getJoueur(posJoueur)->getCompteurActions()[0]++;
    }
    else if (getLastAction(getPositionJoueurAdverse(posJoueur))==TAPIS) {
        this->getJoueur(posJoueur)->getCompteurActions()[1]++;
    }
}


void Jeu::relancer(int posJoueur, int jetons){

    //Si on fait pas tapis:
    if(jetons < this->getJoueur(posJoueur)->getCave()){

        jouerArgent(posJoueur, jetons);
        miseCourante = jetons;
        cumulMisesEtRelances = getJoueur(posJoueur)->getCumulMisesEtRelances();

        this->actions[this->getJoueur(posJoueur)->getPosition()].push_back(ACTION::RELANCER);
        this->getJoueur(posJoueur)->getCompteurActions()[0]++;
    }
    else{ //Sinon: tapis
        tapis(posJoueur, RELANCER);
    }
}


void Jeu::suivre(int posJoueur){

    // Le nombre de jetons à ajouter est le cumul moins le nombre de jetons déjà mis par le joueur
    int jetonsAAjouter = this->cumulMisesEtRelances - this->getJoueur(posJoueur)->getCumulMisesEtRelances();
   // std::cout<<"suivi - cumulMisesEtRelances : "<<cumulMisesEtRelances<<", miseTotJoueur: "<<getJoueur(posJoueur)->getCumulMisesEtRelances()<<"jetons : "<<jetonsAAjouter<<std::endl;

    // Si on a assez d'argent on suit
    if(this->getJoueur(posJoueur)->getCave() > jetonsAAjouter){
        jouerArgent(posJoueur,jetonsAAjouter);
        this->actions[this->getJoueur(posJoueur)->getPosition()].push_back(ACTION::SUIVRE);
        this->getJoueur(posJoueur)->getCompteurActions()[1]++;
    }
    else{      // Sinon on fait tapis
        tapis(posJoueur, SUIVRE);
    }
}


void Jeu::checker(int posJoueur){

    this->actions[this->getJoueur(posJoueur)->getPosition()].push_back(ACTION::CHECKER);
    this->getJoueur(posJoueur)->getCompteurActions()[2]++;
}

void Jeu::seCoucher(int posJoueur){

    this->actions[this->getJoueur(posJoueur)->getPosition()].push_back(ACTION::SE_COUCHER);

    this->getJoueur(0)->setMiseTotale(this->getJoueur(0)->getMiseTotale() + this->getJoueur(0)->getCumulMisesEtRelances());
    this->getJoueur(1)->setMiseTotale(this->getJoueur(1)->getMiseTotale() + this->getJoueur(1)->getCumulMisesEtRelances());

    IntelligenceArtificielleProfilage *ia = static_cast<IntelligenceArtificielleProfilage*>(this->getJoueur(1));
    ia->remplissageDonneesProfilage();

    finPartie();
}    

bool Jeu::debutTour(){

    for(int i=0; i< (int) this->actions.size(); i++){
        if(getLastAction(i) != ACTION::PAS_ENCORE_D_ACTION && getLastAction(i) != ACTION::TAPIS){
            return false;
        }
    }

    return true;
}


bool Jeu::finDuTour(){

    // Si un joueur n'a pas encore joué
    if (getLastAction(0)== PAS_ENCORE_D_ACTION || getLastAction(1) == PAS_ENCORE_D_ACTION) {
        return false;
    }

    // Si un joueur s'est couché
    if (getLastAction(0) == SE_COUCHER || getLastAction(1) == SE_COUCHER) {
        return true;
    }

    // Si tout le monde a checké
    if (getLastAction(0) == CHECKER && getLastAction(1) == CHECKER) {
        return true;
    }

    // Si un joueur a fait tapis et que l'adversaire a joué
    for (unsigned int i = 0; i < listeJoueurs.size(); i++) {
        if (getLastAction(i) == TAPIS) {

            // Si l'autre a suivi, on cherche si c'est avant ou après le tapis (suivi de grosse blind)
            if (getLastAction(getPositionJoueurAdverse(i)) == TAPIS
                    || (getLastAction(getPositionJoueurAdverse(i)) == SUIVRE && actions[i].at(actions[i].size()-2) != GROSSE_BLIND)) {
                return true;
            }
        }
    }

    // Si la suite de mises/relances est terminée (suivi)
    for (unsigned int i = 0; i < listeJoueurs.size(); i++) {
        if (getLastAction(i) == SUIVRE
                && (actions[i].at(actions[i].size()-2) != PETITE_BLIND
                    || (actions[i].at(actions[i].size()-2) == PETITE_BLIND
                        && getLastAction(getPositionJoueurAdverse(i)) == CHECKER))) {
            return true;
        }
    }

    return false;
}


ACTION Jeu::getLastAction(int posJoueur) const{
    return this->actions.at(posJoueur).back();
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

        this->getJoueur(0)->setMiseTotale(this->getJoueur(0)->getMiseTotale() + this->getJoueur(0)->getCumulMisesEtRelances());
        this->getJoueur(1)->setMiseTotale(this->getJoueur(1)->getMiseTotale() + this->getJoueur(1)->getCumulMisesEtRelances());

        IntelligenceArtificielleProfilage *ia = static_cast<IntelligenceArtificielleProfilage*>(this->getJoueur(1));

        ia->remplissageDonneesProfilage();

        // Fin de la partie
        if (this->getEtape() == ETAPE_JEU::RIVER || this->partieFinie || this->aFaitTapis()) {
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

        // Si le joueur n'a pas fait tapis
        if (std::find(actions.at(i).begin(), actions.at(i).end(), ACTION::TAPIS) == actions.at(i).end()) {
            actions.at(i).clear();
            actions.at(i).push_back(ACTION::PAS_ENCORE_D_ACTION);
        }

        this->getJoueur(i)->setMiseCourante(0);
        this->getJoueur(i)->setMisePlusHaute(0);
        this->getJoueur(i)->resetCompteurActions();
        getJoueur(i)->setCumulMisesEtRelances(0);
    }
}

void Jeu::finPartie() {
    partieFinie=true;

    std::vector<Joueur*> joueursRestants;

    for(Joueur *joueur : this->listeJoueurs){
        if(this->estCouche(joueur->getPosition())){
            joueursRestants.push_back(joueur);
        }
    }

    RESULTAT_PARTIE retour;
    //std::cout<<"pot : "<<getPot()<<" - jetons J1: "<<getJoueur(0)->getCave()<<" - jetons J2: "<<getJoueur(1)->getCave()<<std::endl;

    //Si aucun des deux joueurs ne s'est couché:
    if(!estCouche(0) && !estCouche(1)){

        if(getTable().size()<5){ //Dans le cas où il y a eu un tapis et que toutes les cartes ont pas été dévoilées
            distributionCartesTable(5-(getTable().size()));
            calculChancesDeGain();
        }

        RESULTAT_PARTIE comparaisonMains = Evaluateur::comparerMains(this->getTable(), this->getJoueur(0)->getMain(), this->getJoueur(1)->getMain());

        if(comparaisonMains == GAGNE){

            this->getJoueur(0)->ajouteJetons(this->getPot());
            retour = GAGNE;
        }else if(comparaisonMains == PERDU){
            this->getJoueur(1)->ajouteJetons(this->getPot());

            retour = PERDU;
        }else{
            retour = EGALITE;

            this->getJoueur(0)->ajouteJetons(getPot()/2);
            this->getJoueur(1)->ajouteJetons(getPot()/2);
        }
    }else{ //Un joueur s'est couché
        if(estCouche(0)){
            retour = PERDU;
            this->getJoueur(1)->ajouteJetons(this->getPot());
        }else{
            retour = GAGNE;
            this->getJoueur(0)->ajouteJetons(this->getPot());
        }
       // this->getJoueur(joueursRestants.at(0)->getPosition())->ajouteJetons(this->getPot());
    }

    resultatPartie = retour;

    // Si c'est un joueur humain, on calcule ses chances de gain
    if (getJoueur(0)->estHumain()) {

        int nbThreads = 4;
        double nbTestsParThread = static_cast<double>(NOMBRE_DE_TESTS) / nbThreads;
        std::vector<EstimationProba*> estimateurs;

        for (int i = 0; i < 4; i++) {
            EstimationProba *estimateur = new EstimationProba(this, getJoueur(0), nbTestsParThread);
            estimateurs.push_back(estimateur);
            estimateur->start();
        }

        double sommeEstimations = 0;

        for (unsigned int i = 0; i < estimateurs.size(); i++) {
            estimateurs[i]->wait();
            sommeEstimations += estimateurs[i]->getResultat();

            delete estimateurs[i];
        }

        getJoueur(0)->setChancesGain(100 * (sommeEstimations / estimateurs.size()));
        estimateurs.clear();
    }

    IntelligenceArtificielleProfilage *ia = static_cast<IntelligenceArtificielleProfilage*>(this->getJoueur(1));
    ia->ecritureResultatsPartie();
}

std::vector<Carte> Jeu::getTable() const{
    return this->table;
}

std::vector<ACTION>  Jeu::getListeActions(int posJoueur) const{
    return this->actions.at(posJoueur);
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
        this->getJoueur(i)->setMiseTotale(0);
	}
	
	this->deck = nouveauDeck();

    this->getJoueur(this->dealer)->changeDealer();
    this->dealer = (this->dealer + 1) % this->listeJoueurs.size();
    this->getJoueur(this->dealer)->changeDealer();

    this->etape = ETAPE_JEU::PREFLOP;
}


bool Jeu::peutChecker(int posJoueur){

    int posAdversaire = getPositionJoueurAdverse(posJoueur);

    //On peut checker quand le joueur précédent a checké ou suivi.

    if(getLastAction(posAdversaire)==ACTION::TAPIS && this->debutTour()){
        return true;
    }

    //Si l'action de l'autre joueur est miser, relancer ou grosse blinde, on retourne false
    if(getLastAction(posAdversaire)==ACTION::MISER || getLastAction(posAdversaire)==ACTION::RELANCER
            || getLastAction(posAdversaire)==ACTION::GROSSE_BLIND || getLastAction(posAdversaire)==ACTION::TAPIS){
        return false;
    }

    return true;
}

bool Jeu::peutMiser(int posJoueur, int jetons){

    int posAdversaire = getPositionJoueurAdverse(posJoueur);

    //On peut miser quand le joueur précédent a checké
    if(getLastAction(posAdversaire)==ACTION::MISER || getLastAction(posAdversaire)==ACTION::RELANCER
            || getLastAction(posAdversaire)==ACTION::GROSSE_BLIND || getLastAction(posAdversaire)==ACTION::TAPIS){

        if(getLastAction(posAdversaire)==ACTION::SUIVRE && getLastAction(posJoueur)==ACTION::GROSSE_BLIND){
            return true;
        }
        return false;
    }

    if (jetons <= 0 || getJoueur(posJoueur)->getCave() < jetons) {
        return false;
    }

    return true;
}


bool Jeu::peutRelancer(int posJoueur, int jetons){

    int posAdversaire = getPositionJoueurAdverse(posJoueur);

    //On peut pas relancer quand le joueur précédent a checké, n'as pas agit, a fait tapis ou a suivi.
    if(getLastAction(posAdversaire)==ACTION::CHECKER || getLastAction(posAdversaire)==ACTION::PAS_ENCORE_D_ACTION
            || getLastAction(posAdversaire)==ACTION::TAPIS || getLastAction(posAdversaire)==ACTION::SUIVRE){
        return false;
    }

    if(jetons>0 && getJoueur(posJoueur)->getCave()>=jetons && jetons >= miseCourante * 2){
        return true;
    }

    return false;
}

bool Jeu::peutSuivre(int posJoueur){

    int posAdversaire = getPositionJoueurAdverse(posJoueur);

    //On peut suivre quand le joueur précédent a misé, relancé, grosse blind ou fait tapis
    if(getLastAction(posAdversaire)==ACTION::CHECKER || getLastAction(posAdversaire)==ACTION::PETITE_BLIND
            || getLastAction(posAdversaire)==ACTION::SUIVRE || getLastAction(posAdversaire)==ACTION::PAS_ENCORE_D_ACTION){
        return false;
    }

    return true;
}

bool Jeu::estCouche(int posJoueur) const {
    return (getLastAction(posJoueur) == ACTION::SE_COUCHER);
}

void Jeu::executerAction(int posJoueur, Action a){
    int relance;

   // std::cout<<"action Joueur"<<std::to_string(posJoueur)<<" : "<<a.getAction()<<"Jetons joués : "<<a.getMontant()<<" étape: "<<getEtape()<<std::endl;
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

void Jeu::lancerPartie()
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
    return getLastAction(0)== ACTION::TAPIS || getLastAction(1) == ACTION::TAPIS;
}

