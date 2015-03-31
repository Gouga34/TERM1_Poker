#include "../../include/IA/IntelligenceArtificielle.h"
#include "../../include/IA/EstimationProba.h"
#include "../../include/Interface/Logger.h"
#include <sstream> 
#include <string>

using namespace std;

IntelligenceArtificielle::IntelligenceArtificielle(bool estDealer, int jetons, int position)
    :Joueur(estDealer, jetons, position){
    resolveur = new Resolveur(this);
    agressivite = this->getJeu()->getAgressiviteIA();
    rationalite = this->getJeu()->getRationaliteIA();

}

IntelligenceArtificielle::IntelligenceArtificielle(Joueur joueur): Joueur(joueur){
    resolveur = new Resolveur(this);
    agressivite = this->getJeu()->getAgressiviteIA();
    rationalite = this->getJeu()->getRationaliteIA();
    string ration="rationalite : "+to_string(rationalite);
    string agress="agressivite : "+to_string(agressivite);
    Logger::getInstance()->ajoutLogs(ration);
    Logger::getInstance()->ajoutLogs(agress);
    //cout<<ration<<endl<<agress<<endl;
}

IntelligenceArtificielle::~IntelligenceArtificielle(){

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

void IntelligenceArtificielle::setTable(vector<Carte> tab){
	this->table = tab;
}

void IntelligenceArtificielle::jouer(){
	
    double estimation = 100 * EstimationProba::estimation(this->getJeu(), &this->getJeu()->getJoueur(this->getPosition()));
    setChancesGain(estimation);

    string chances="Chances Gain IA : "+to_string(estimation);
    Logger::getInstance()->ajoutLogs(chances);

    resolveur->setAgressivite(agressivite);
    resolveur->setRationalite(rationalite);

    pair<ACTION,int> action=resolveur->calculerAction();

    jeu->executerAction(getPosition(),action.first,action.second);

}
