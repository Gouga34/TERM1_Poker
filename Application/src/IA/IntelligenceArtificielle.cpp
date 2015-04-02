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
}

void IntelligenceArtificielle::remplissageDonneesProfilage() {

    int nbTotalActions = 0;

    for(int i = 0; i<3; i++){
        nbTotalActions += jeu->getJoueur(0)->getCompteurActions()[i];
    }

    profilage->etatPartie[jeu->getEtape()].probaGainAdversaire = 100 * EstimationProba::estimation(jeu, this);
    profilage->etatPartie[jeu->getEtape()].tauxMises = CalculDonneesProfilage::taux(this->getCompteurActions()[0],nbTotalActions);
    profilage->etatPartie[jeu->getEtape()].tauxSuivis = CalculDonneesProfilage::taux(this->getCompteurActions()[1],nbTotalActions);
    profilage->etatPartie[jeu->getEtape()].tauxChecks = CalculDonneesProfilage::taux(this->getCompteurActions()[2],nbTotalActions);

    profilage->etatPartie[jeu->getEtape()].misePlusHaute = CalculDonneesProfilage::taux(this->getMisePlusHaute(),this->getCave());
    profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur = CalculDonneesProfilage::taux(this->getMiseTotale(),this->getCave());

    profilage->etatPartie[jeu->getEtape()].tauxAgressivite = CalculDonneesProfilage::agressivite(profilage->etatPartie[jeu->getEtape()].misePlusHaute,profilage->etatPartie[jeu->getEtape()].tauxMises,profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur);
    profilage->etatPartie[jeu->getEtape()].tauxRationnalite = CalculDonneesProfilage::rationalite(profilage->etatPartie[jeu->getEtape()].probaGainAdversaire,profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur);
    profilage->etatPartie[jeu->getEtape()].tauxPassivite = CalculDonneesProfilage::passivite( profilage->etatPartie[jeu->getEtape()].tauxSuivis, profilage->etatPartie[jeu->getEtape()].tauxChecks);
    profilage->etatPartie[jeu->getEtape()].tauxBluff = CalculDonneesProfilage::bluff(profilage->etatPartie[jeu->getEtape()].tauxRationnalite);

    profilage->etatPartie[jeu->getEtape()].pot = jeu->getPot();

    profilage->etatPartie[jeu->getEtape()].couche = jeu->estCouche(0);

    profilage->correction(jeu->getEtape());
}

void IntelligenceArtificielle::jouer(){
	
    double estimation = 100*EstimationProba::estimation(this->getJeu(), this->getJeu()->getJoueur(this->getPosition()));
    setChancesGain(estimation);

    string chances="Chances Gain IA : "+to_string(estimation);
    Logger::getInstance()->ajoutLogs(chances);

    pair<ACTION,int> action=resolveur->calculerAction();

    jeu->executerAction(getPosition(),action.first,action.second);

}
