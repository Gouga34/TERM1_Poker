#include "../include/IntelligenceArtificielle.h"
#include <sstream> 

IntelligenceArtificielle::IntelligenceArtificielle(bool estDealer, int jetons): Joueur(estDealer, jetons){

}

IntelligenceArtificielle::IntelligenceArtificielle(Joueur joueur): Joueur(joueur){

}

IntelligenceArtificielle::~IntelligenceArtificielle(){

}


double IntelligenceArtificielle::calculProba(){

	std::ifstream fichier("../Probas/probas_preflops", std::ios::in);
	double probabilite = 0;
 
        if(fichier){
        	int rangCarte;       
        	    	
        	std::string main = "";
        	
        	for(int i=0; i<2; i++){
        	
			rangCarte = this->main.at(i).getRang();
			
			switch(rangCarte){
			
				case 1 : {
					main += "A";
					break;
				}
				
				case 2 : {
					main += "2";
					break;
				}
				
				case 3 : {
					main += "3";
					break;
				}
				
				case 4 : {
					main += "4";
					break;
				}
				
				case 5 : {
					main += "5";
					break;
				}
				
				case 6 : {
					main += "6";
					break;
				}
				
				case 7 : {
					main += "7";
					break;
				}
				
				case 8 : {
					main += "8";
					break;
				}
				
				case 9 : {
					main += "9";
					break;
				}
				
				case 10 : {
					main += "T";
					break;
				}
				
				case 11 : {
					main += "J";
					break;
				}
				
				case 12 : {
					main += "Q";
					break;
				}
				
				case 13 : {
					main += "K";
					break;
				}
			
			}
			   	
        	}
        	
        	if(this->main.at(0).getCouleur() == this->main.at(1).getCouleur()){
        		std::cout << this->main.at(0).getCouleur() << "," << this->main.at(1).getCouleur() << std::endl;
			main += "*";
		}
		
		std::string ligne;
		std::size_t found;
		std::istringstream isstream;
		std::string mot;
    
    		//TODO : Attention il faudra gérer le cas KQ et QK, il s'agit de la meme main mais seulement une est reconnue
                while(getline(fichier, ligne) ){
                	
        		isstream.str(ligne);
        		getline(isstream, mot, ' ');
        		
        		if(mot == main){
        			break;
        		}
                	
                }
                
                std::cout << main << std::endl;
                probabilite = atof(ligne.substr(4,4).c_str());

        	
                fichier.close();
        }else{
        	 std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
        }
               
	return probabilite;
}

void IntelligenceArtificielle::setTable(std::vector<Carte> tab){
	this->table = tab;
}

void IntelligenceArtificielle::jouer(){
	std::cout << this->main.at(0).getRang() << "," << this->main.at(0).getCouleur() << std::endl;
	std::cout << this->main.at(1).getRang() << "," << this->main.at(1).getCouleur() << std::endl;
	std::cout << this->calculProba() << std::endl;
}


