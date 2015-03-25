#include "../../include/Jeu/IntelligenceArtificielle.h"
#include "../../include/Jeu/EstimationProba.h"
#include <sstream> 

IntelligenceArtificielle::IntelligenceArtificielle(bool estDealer, int jetons, int position): Joueur(estDealer, jetons, position){

}

IntelligenceArtificielle::IntelligenceArtificielle(Joueur joueur): Joueur(joueur){

}

IntelligenceArtificielle::~IntelligenceArtificielle(){

}

double IntelligenceArtificielle::calculProba(){


    std::ifstream fichier("../Application/ressources/Probas/probas_preflops", std::ios::in);
	double probabilite = 0;
 
        if(fichier){
        	int rangCarte;       
        	    	
        	std::string main = "";
            std::string main2 = "";
        	
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
                    main += std::to_string(rangCarte);
                    main2 = std::to_string(rangCarte) + main2;
                    break;
			}
			   	
        }
        	
        if(this->main.at(0).getCouleur() == this->main.at(1).getCouleur()){
			main += "*";
            		main2 += "*";
	}
		
		std::string ligne;
		std::istringstream isstream;
		std::string mot;
    
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
        	 std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
        }
               
	return probabilite;
}

void IntelligenceArtificielle::setTable(std::vector<Carte> tab){
	this->table = tab;
}

std::vector<Integer> IntelligenceArtificielle::getBornesRationalite(double estimation){

    std::vector<Integer> bornes;
	
	for(int i=0; i< RATIONALITE::NB_PALLIERS; i++){
        if( estimation >= RATIONALITE::PALLIER1::DEBUT_GAIN &&  estimation <= RATIONALITE::PALLIER1::DEBUT_GAIN ){
            borne.push_back(RATIONALITE::PALLIER1::DEBUT_GAIN);
            borne.push_back(RATIONALITE::PALLIER1::FIN_GAIN);
            borne.push_back(RATIONALITE::PALLIER1::DEBUT_RA_THEORIQUE);
            borne.push_back(RATIONALITE::PALLIER1::FIN_RA_THEORIQUE);

            return bornes;
        }else if ( estimation >= RATIONALITE::PALLIER2::DEBUT_GAIN &&  estimation <= RATIONALITE::PALLIER2::DEBUT_GAIN ){
            borne.push_back(RATIONALITE::PALLIER2::DEBUT_GAIN);
            borne.push_back(RATIONALITE::PALLIER2::FIN_GAIN);
            borne.push_back(RATIONALITE::PALLIER2::DEBUT_RA_THEORIQUE);
            borne.push_back(RATIONALITE::PALLIER2::FIN_RA_THEORIQUE);

            return bornes;
        }else if ( estimation >= RATIONALITE::PALLIER3::DEBUT_GAIN &&  estimation <= RATIONALITE::PALLIER3::DEBUT_GAIN ){
            borne.push_back(RATIONALITE::PALLIER3::DEBUT_GAIN);
            borne.push_back(RATIONALITE::PALLIER3::FIN_GAIN);
            borne.push_back(RATIONALITE::PALLIER3::DEBUT_RA_THEORIQUE);
            borne.push_back(RATIONALITE::PALLIER3::FIN_RA_THEORIQUE);

            return bornes;
        }else if ( estimation >= RATIONALITE::PALLIER4::DEBUT_GAIN &&  estimation <= RATIONALITE::PALLIER4::DEBUT_GAIN ){
            borne.push_back(RATIONALITE::PALLIER4::DEBUT_GAIN);
            borne.push_back(RATIONALITE::PALLIER4::FIN_GAIN);
            borne.push_back(RATIONALITE::PALLIER4::DEBUT_RA_THEORIQUE);
            borne.push_back(RATIONALITE::PALLIER4::FIN_RA_THEORIQUE);

            return bornes;
        }
	}
}

/*
int IntelligenceArtificielle::calculMiseTheorique(double estimation){
	
    //return
	
    //( ((p-g1)*[(m2-m1)/(g2-g1)] ) + m1
	
	return 0;
}*/

void IntelligenceArtificielle::jouer(){
	
    //double proba = this->calculProba();
	
	EstimationProba estime( this->getJeu(), &this->getJeu()->getJoueur(this->getPosition()) );
	double estimation = 100 * estime.estimation();

   	double agressivite = this->getJeu()->getAgressiviteIA();
   	double rationalite = this->getJeu()->getRationaliteIA();
   	
     std::vector<Integer> bornes;

     bornes = getBornesRationalite(10);

     for(int i = 0; i< bornes.size(); i++){
         std::cout << bornes.at(i) << std::endl;
     }
	
	if(estimation > 75){
		if(this->getCave() < this->getJeu()->getMise() * 2){
			this->getJeu()->tapis(this->getPosition());
		}else if(this->getJeu()->peutRelancer(this->getPosition())){
			this->getJeu()->relancer(this->getPosition(), this->getJeu()->getMise() * 2);
		}else if(this->getCave() > this->getJeu()->getBlind()*2){
			this->getJeu()->miser(this->getPosition(), 2*this->getJeu()->getBlind());
		}else{
			this->getJeu()->tapis(this->getPosition());
		}	
	}else if (estimation > 50 ){
		if(this->getJeu()->getMise() == 0 && this->getCave() > this->getJeu()->getBlind()){
			this->getJeu()->miser(this->getPosition(), this->getJeu()->getBlind());
		}else if(this->getJeu()->getMise() == 0 && this->getCave() < this->getJeu()->getBlind()){
			this->getJeu()->tapis(this->getPosition());
		}else if (this->getCave() < this->getJeu()->getMise()) {
			this->getJeu()->tapis(this->getPosition());
		}else{
			this->getJeu()->suivre(this->getPosition());
		}	
	}else{
		if(this->getJeu()->getListeActions().at(this->getPosition()) == TYPES::ACTION_LIST::PETITE_BLIND){
			this->getJeu()->suivre(this->getPosition());
		}else if(this->getJeu()->peutChecker(this->getPosition())){
			this->getJeu()->checker(this->getPosition());
		}else{
			this->getJeu()->seCoucher(this->getPosition());
		}
	}
}
