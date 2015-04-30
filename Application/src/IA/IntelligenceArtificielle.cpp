#include "../../include/IA/IntelligenceArtificielle.h"
#include "../../include/IA/EstimationProba.h"
#include "../../include/Interface/Logger.h"
#include <sstream> 
#include <string>

using namespace std;

IntelligenceArtificielle::IntelligenceArtificielle(bool estDealer, int jetons, int position)
    :Joueur(estDealer, jetons, position) {
    resolveur = new Resolveur(this);

    double agressivite=rand()%100+1;
    double rationalite=rand()%100+1;

    Profil p;
    p.setAgressivite(agressivite);
    p.setRationalite(rationalite);

    setCalibrage(p);

    if(position==0){
        std::cout<<"Calibrage IA profilée : agressivité: "<<agressivite<<" rationalité: "<<rationalite<<std::endl;
    }
}

IntelligenceArtificielle::IntelligenceArtificielle(const IntelligenceArtificielle& joueur): Joueur(joueur){
    resolveur = new Resolveur(this);
}

IntelligenceArtificielle::~IntelligenceArtificielle(){
    if (resolveur != 0) {
        delete resolveur;
    }
}

bool IntelligenceArtificielle::estHumain() const {
    return false;
}

double IntelligenceArtificielle::calculProba(){

    ifstream fichier(FICHIER_PROBAS_PREFLOP, ios::in);
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

Profil IntelligenceArtificielle::getCalibrage() const {
    return *(resolveur->getCalibrage());
}

void IntelligenceArtificielle::setCalibrage(Profil profil) {
    string ration="Rationalite : "+to_string(profil.getRationalite());
    string agress="Agressivite : "+to_string(profil.getAgressivite());
    Logger::getInstance()->ajoutLogs(ration);
    Logger::getInstance()->ajoutLogs(agress);
    resolveur->setCalibrage(profil);
}

void IntelligenceArtificielle::lancerEstimationChancesDeGain(int nbTests, int nbThreads)
{
    estimateurs.clear();
    double nbTestsParThread = static_cast<double>(nbTests) / nbThreads;

    for (int i = 0; i < nbThreads; i++) {
        EstimationProba *estimateur = new EstimationProba(getJeu(), getJeu()->getJoueur(getPosition()), nbTestsParThread);
        estimateurs.push_back(estimateur);
        estimateur->start();
    }
}

void IntelligenceArtificielle::attendreResultatEstimation()
{
    double sommeEstimations = 0;

    for (int i = 0; i < estimateurs.size(); i++) {
        estimateurs[i]->wait();
        sommeEstimations += estimateurs[i]->getResultat();

        delete estimateurs[i];
    }

    setChancesGain(100 * (sommeEstimations / estimateurs.size()));
    estimateurs.clear();
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
