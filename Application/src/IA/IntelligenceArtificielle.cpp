#include "../../include/IA/IntelligenceArtificielle.h"
#include "../../include/IA/EstimationProba.h"
#include "../../include/Interface/Logger.h"
#include <sstream> 
#include <string>

using namespace std;

IntelligenceArtificielle::IntelligenceArtificielle(bool estDealer, int jetons, int position)
    :Joueur(estDealer, jetons, position) {
    resolveur = new Resolveur(this);
    int agressivite=rand()%100+1;
    int rationalite=rand()%100+1;
    resolveur->getCalibrage().setAgressivite(agressivite);
    resolveur->getCalibrage().setRationalite(rationalite);
    if(position==0){
        std::cout<<"Calibrage IA profilée : agressivité: "<<agressivite<<" rationalité: "<<rationalite<<std::endl;
    }

}

IntelligenceArtificielle::IntelligenceArtificielle(Joueur joueur): Joueur(joueur){
    resolveur = new Resolveur(this);
}

IntelligenceArtificielle::~IntelligenceArtificielle(){
    if (resolveur != 0) {
        delete resolveur;
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

void IntelligenceArtificielle::setCalibrage(Profil profil) {
    string ration="Rationalite : "+to_string(profil.getRationalite());
    string agress="Agressivite : "+to_string(profil.getAgressivite());
    Logger::getInstance()->ajoutLogs(ration);
    Logger::getInstance()->ajoutLogs(agress);
    resolveur->setCalibrage(profil);
}

void IntelligenceArtificielle::estimationChancesDeGain()
{
    double estimation = 100*EstimationProba::estimation(this->getJeu(), this->getJeu()->getJoueur(this->getPosition()));
    setChancesGain(estimation);
    string chances;
    if(getPosition()==1){
       chances ="Chances Gain IA qui profile: "+to_string(estimation);
    }
    else{
        chances ="Chances Gain IA profilée: "+to_string(estimation);
    }
    Logger::getInstance()->ajoutLogs(chances);
}


Action IntelligenceArtificielle::jouer(){

    QString actions[] = {"mise", "relance", "suit", "check", "se couche", "rien", "fait tapis"};

    Action a = resolveur->calculerAction();

    QString log = "IA " + QString::number(position) + " " + actions[a.getAction()];
    if (a.getAction() == MISER || a.getAction() == RELANCER) {
        log += " " + QString::number(a.getMontant());
    }

    Logger::getInstance()->ajoutLogs(log);

    return a;
}
