#include "../include/IntelligenceArtificielle.h"
#include <sstream> 

IntelligenceArtificielle::IntelligenceArtificielle(bool estDealer, int jetons, int position): Joueur(estDealer, jetons, position){

}

IntelligenceArtificielle::IntelligenceArtificielle(Joueur joueur): Joueur(joueur){

}

IntelligenceArtificielle::~IntelligenceArtificielle(){

}

/**
*@action : Permet d'obtenir la probabilité pre-Flop
*@return : La probabilitee pre-Flop de la main courant
**/
double IntelligenceArtificielle::calculProba(){

    std::ifstream fichier("../../Probas/probas_preflops", std::ios::in);
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
        		std::cout << this->main.at(0).getCouleur() << "," << this->main.at(1).getCouleur() << std::endl;
			main += "*";
            main2 += "*";
		}
		
		std::string ligne;
		std::size_t found;
		std::istringstream isstream;
		std::string mot;
    
    		//TODO : Attention il faudra gérer le cas KQ et QK, il s'agit de la meme main mais seulement une est reconnue
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

/**
*@action : Permet d'obtenir les carte communes
*@param  : Un ensemble de carte representant les cartes communes
**/
void IntelligenceArtificielle::setTable(std::vector<Carte> tab){
	this->table = tab;
}


/**
*@action : Pemet a l'IA de jouer
**/
void IntelligenceArtificielle::jouer(){
	std::cout << this->main.at(0).getRang() << "," << this->main.at(0).getCouleur() << std::endl;
	std::cout << this->main.at(1).getRang() << "," << this->main.at(1).getCouleur() << std::endl;
	std::cout << this->calculProba() << std::endl;
}
