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

std::vector<int> IntelligenceArtificielle::getBornesRationalite(double estimation){

	std::vector<int> bornes;
	
	for(int i=0; i< RATIONALITE::NB_PALLIERS; i++){
		if( estimation >= RATIONALITE::PALIER1::DEBUT_GAIN &&  estimation <= RATIONALITE::PALIER1::DEBUT_GAIN ){
		    bornes.push_back(RATIONALITE::PALIER1::DEBUT_GAIN);
		    bornes.push_back(RATIONALITE::PALIER1::FIN_GAIN);
		    bornes.push_back(RATIONALITE::PALIER1::DEBUT_RA_THEORIQUE);
		    bornes.push_back(RATIONALITE::PALIER1::FIN_RA_THEORIQUE);

		    return bornes;
		}else if ( estimation >= RATIONALITE::PALIER2::DEBUT_GAIN &&  estimation <= RATIONALITE::PALIER2::DEBUT_GAIN ){
		    bornes.push_back(RATIONALITE::PALIER2::DEBUT_GAIN);
		    bornes.push_back(RATIONALITE::PALIER2::FIN_GAIN);
		    bornes.push_back(RATIONALITE::PALIER2::DEBUT_RA_THEORIQUE);
		    bornes.push_back(RATIONALITE::PALIER2::FIN_RA_THEORIQUE);

		    return bornes;
		}else if ( estimation >= RATIONALITE::PALIER3::DEBUT_GAIN &&  estimation <= RATIONALITE::PALIER3::DEBUT_GAIN ){
		    bornes.push_back(RATIONALITE::PALIER3::DEBUT_GAIN);
		    bornes.push_back(RATIONALITE::PALIER3::FIN_GAIN);
		    bornes.push_back(RATIONALITE::PALIER3::DEBUT_RA_THEORIQUE);
		    bornes.push_back(RATIONALITE::PALIER3::FIN_RA_THEORIQUE);

		    return bornes;
		}else if ( estimation >= RATIONALITE::PALIER4::DEBUT_GAIN &&  estimation <= RATIONALITE::PALIER4::DEBUT_GAIN ){
		    bornes.push_back(RATIONALITE::PALIER4::DEBUT_GAIN);
		    bornes.push_back(RATIONALITE::PALIER4::FIN_GAIN);
		    bornes.push_back(RATIONALITE::PALIER4::DEBUT_RA_THEORIQUE);
		    bornes.push_back(RATIONALITE::PALIER4::FIN_RA_THEORIQUE);

		    return bornes;
		}
	}
	return bornes;
}

int IntelligenceArtificielle::calculMiseTheorique(double estimation){
	
	std::vector<int> bornes = this->getBornesRationalite(estimation);
    
    	return 0;
	//return ( (estimation-bornes.at(0)*[(bornes.at(3)-bornes.at(2)/(bornes.at(1)-bornes.at(0))] ) + bornes.at(2) ;
	
}

void IntelligenceArtificielle::jouer(){
	
    //double proba = this->calculProba();
	
	EstimationProba estime( this->getJeu(), &this->getJeu()->getJoueur(this->getPosition()) );
	double estimation = 100 * estime.estimation();
/*
   	double agressivite = this->getJeu()->getAgressiviteIA();
   	double rationalite = this->getJeu()->getRationaliteIA();
   	
     std::vector<int> bornes;

     bornes = getBornesRationalite(10);

     for(int i = 0; i< bornes.size(); i++){
         std::cout << bornes.at(i) << std::endl;
     }*/
	
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
