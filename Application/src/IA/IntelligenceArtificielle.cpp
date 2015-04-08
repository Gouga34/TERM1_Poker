#include "../../include/IA/IntelligenceArtificielle.h"
#include "../../include/IA/EstimationProba.h"
#include "../../include/Interface/Logger.h"
#include <sstream> 
#include <string>

using namespace std;

IntelligenceArtificielle::IntelligenceArtificielle(bool estDealer, int jetons, int position)
    :Joueur(estDealer, jetons, position), profilage(0){
    resolveur = new Resolveur(this);
}

IntelligenceArtificielle::IntelligenceArtificielle(Joueur joueur): Joueur(joueur){
    resolveur = new Resolveur(this);
}

IntelligenceArtificielle::~IntelligenceArtificielle(){
    if (resolveur != 0) {
        delete resolveur;
    }

    if (profilage != 0) {
        delete profilage;
    }

    if (scenario != 0) {
        delete scenario;
    }
}

double IntelligenceArtificielle::calculProba(){

    ifstream fichier("../Application/ressources/Probas/probas_preflops", ios::in);
	double probabilite = 0;
 
        if(fichier){
        	int rangCarte;       
        	    	
            string main = "";
            string main2 = "";
        	
        	for(int i=0; i<2; i++){
        	
			rangCarte = this->main.at(i).getRang();
			
			switch(rangCarte){
			
				case 1 : {
					main += "A";
                    main2 = "A" + main2;
					break;
				}
				
				case 10 : {
					main += "T";
                    main2 = "T" + main2;
					break;
				}
				
				case 11 : {
					main += "J";
                    main2 = "J" + main2;
					break;
				}
				
				case 12 : {
					main += "Q";
                    main2 = "Q" + main2;
					break;
				}
				
				case 13 : {
					main += "K";
                    main2 = "K" + main2;
					break;
				}

                default:
                    main += to_string(rangCarte);
                    main2 = to_string(rangCarte) + main2;
                    break;
			}
			   	
        }
        	
        if(this->main.at(0).getCouleur() == this->main.at(1).getCouleur()){
			main += "*";
            		main2 += "*";
	}
		
        string ligne;
        istringstream isstream;
        string mot;
    
                while(getline(fichier, ligne) ){
        		isstream.str(ligne);
        		getline(isstream, mot, ' ');
        		
                if(mot == main || mot == main2){
                    probabilite = atof(ligne.substr(4,4).c_str());
        			break;
        		}
                	
                }

                fichier.close();
        }else{
             cerr << "Impossible d'ouvrir le fichier !" << endl;
        }
               
	return probabilite;
}

Profilage* IntelligenceArtificielle::getProfilage() const {
    return profilage;
}

void IntelligenceArtificielle::setCalibrage(Profil profil) {
    string ration="Rationalite : "+to_string(profil.getRationalite());
    string agress="Agressivite : "+to_string(profil.getAgressivite());
    Logger::getInstance()->ajoutLogs(ration);
    Logger::getInstance()->ajoutLogs(agress);
    resolveur->setCalibrage(profil);
}

void IntelligenceArtificielle::setPseudoJoueur(string pseudo) {
    profilJoueur.setPseudo(pseudo);

    if (profilage) {
        delete profilage;
    }
    profilage = new Profilage(&profilJoueur);

    scenario = new ScenariosDeTests(&profilJoueur, &resolveur->getCalibrage());
}

void IntelligenceArtificielle::remplissageDonneesProfilage() {

    int nbTotalActions = 0;

    for(int i = 0; i<3; i++){
        nbTotalActions += jeu->getJoueur(0)->getCompteurActions()[i];
    }

    double resultat;

    profilage->etatPartie[jeu->getEtape()].probaGainAdversaire = 100 * EstimationProba::estimation(jeu, this);

    resultat = CalculDonneesProfilage::taux(this->getCompteurActions()[0],nbTotalActions);
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].tauxMises = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].tauxMises = 0;
    }

    resultat = CalculDonneesProfilage::taux(this->getCompteurActions()[1],nbTotalActions);
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].tauxSuivis = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].tauxSuivis = 0;
    }

    resultat = CalculDonneesProfilage::taux(this->getCompteurActions()[2],nbTotalActions);
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].tauxChecks = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].tauxChecks = 0;
    }

    resultat = CalculDonneesProfilage::taux(this->getMisePlusHaute(),this->getCave());
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].misePlusHaute = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].misePlusHaute = 0;
    }


    resultat = CalculDonneesProfilage::taux(this->getMiseTotale(),this->getCave());
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur = 0;
    }


    resultat = CalculDonneesProfilage::agressivite(profilage->etatPartie[jeu->getEtape()].misePlusHaute,profilage->etatPartie[jeu->getEtape()].tauxMises,profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur);
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].tauxAgressivite = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].tauxAgressivite = 0;
    }

    resultat = CalculDonneesProfilage::rationalite(profilage->etatPartie[jeu->getEtape()].probaGainAdversaire,profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur);
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].tauxRationnalite = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].tauxRationnalite = 0;
    }


    resultat = CalculDonneesProfilage::passivite( profilage->etatPartie[jeu->getEtape()].tauxSuivis, profilage->etatPartie[jeu->getEtape()].tauxChecks);
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].tauxPassivite = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].tauxPassivite = 0;
    }


    resultat = CalculDonneesProfilage::bluff(profilage->etatPartie[jeu->getEtape()].tauxRationnalite);
    if(resultat < 0){
        profilage->etatPartie[jeu->getEtape()].tauxBluff = resultat;
    }else{
        profilage->etatPartie[jeu->getEtape()].tauxBluff = 0;
    }

    profilage->etatPartie[jeu->getEtape()].pot = jeu->getPot();

    profilage->etatPartie[jeu->getEtape()].couche = jeu->estCouche(0);
}

void IntelligenceArtificielle::calculProfilGlobalJoueur() {
    //On récupère les 4 taux des 4 parties et on en fait la moyenne, pour chaque type de jeu

    int sommeAgressivite=0;
    int sommeRationalite=0;
    int sommePassivite=0;
    int sommeBluff=0;
    for(int i=0;i<ETAPE_JEU::NB_ETAPES;i++){
        sommeAgressivite+=profilage->etatPartie[i].tauxAgressivite;
        sommeRationalite+=profilage->etatPartie[i].tauxRationnalite;
        sommePassivite+=profilage->etatPartie[i].tauxPassivite;
        sommeBluff+=profilage->etatPartie[i].tauxBluff;
    }

    profilage->profilJoueur->setAgressivite(sommeAgressivite/ETAPE_JEU::NB_ETAPES);
    profilage->profilJoueur->setRationalite(sommeRationalite/ETAPE_JEU::NB_ETAPES);
    profilage->profilJoueur->setPassivite(sommePassivite/ETAPE_JEU::NB_ETAPES);
    profilage->profilJoueur->setBluff(sommeBluff/ETAPE_JEU::NB_ETAPES);

    profilage->sauvegarder();
}

void IntelligenceArtificielle::ecritureScenariosDeTests() {
    scenario->setCalibrageActuelIA(&resolveur->getCalibrage());
    scenario->setChancesDeGain(profilage->etatPartie[ETAPE_JEU::RIVER].probaGainAdversaire);
    scenario->sauvegarderPartie();
}

void IntelligenceArtificielle::estimationChancesDeGain()
{
    double estimation = 100*EstimationProba::estimation(this->getJeu(), this->getJeu()->getJoueur(this->getPosition()));
    setChancesGain(estimation);

    string chances="Chances Gain IA : "+to_string(estimation);
    Logger::getInstance()->ajoutLogs(chances);
}

void IntelligenceArtificielle::jouer(){

    pair<ACTION,int> action=resolveur->calculerAction();

    jeu->executerAction(getPosition(),action.first,action.second);

}
