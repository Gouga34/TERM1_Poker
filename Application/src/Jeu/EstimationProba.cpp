#include "../../include/Jeu/EstimationProba.h"
#include "../../include/Jeu/Carte.h"

#include <string>
#include <unordered_map>
#include <vector>

int nombreDeTest = 10000;

EstimationProba::EstimationProba(Jeu* jeu, Joueur* joueur){
	srand((unsigned)time(0));
	this->jeuCourant = jeu;
	this->joueurCourant = joueur;
}

EstimationProba::~EstimationProba(){
}

double EstimationProba::estimation(){

	int nombreDeCoupGagner = 0;
	int position;
	std::string ensembleCourant;
	std::vector<Carte> table;
	std::vector<Carte> mainAdverse;
	std::vector<Carte> deck;
	std::unordered_map<std::string,int> listeEnsembleCarte;
	std::unordered_map<std::string,int>::const_iterator iterateurEnsembleCarte;
	
	for(int t=0; t <nombreDeTest; t++){
	
	
		do{
			deck = this->nouveauDeck();
			this->melange(deck);
			table.clear();
			mainAdverse.clear();
			ensembleCourant = "";
		
			table = jeuCourant->getTable();
            for(int i=0; i< 5 - (int) table.size(); i++){
				position = rand() % deck.size();
				ensembleCourant += std::to_string(deck.at(position).getRang()) + std::to_string(deck.at(position).getCouleur()) ;
				table.push_back(deck.at(position));
				deck.erase(deck.begin() + position);
			}
		
			for(int i=0; i< 2; i++){
				position = rand() % deck.size();
				ensembleCourant += std::to_string(deck.at(position).getRang()) + std::to_string(deck.at(position).getCouleur()) ;
				mainAdverse.push_back(deck.at(position));
				deck.erase(deck.begin() + position);
			}
			
			iterateurEnsembleCarte  = listeEnsembleCarte.find (ensembleCourant);
		}while( iterateurEnsembleCarte != listeEnsembleCarte.end());
		
		
		if(true){
			nombreDeCoupGagner++;
		}
	
	
	}
	
	
	return nombreDeCoupGagner / nombreDeTest;

}


std::vector<Carte> EstimationProba::nouveauDeck(){
	std::vector<Carte> deck;
	
	for(int i = 0; i<4; i++){
		for(int j= 1; j<14; j++){
			if( (joueurCourant->getMain().at(0).getRang() != j && joueurCourant->getMain().at(0).getCouleur() != i)
			&& (joueurCourant->getMain().at(1).getRang() != j && joueurCourant->getMain().at(1).getCouleur() != i) ){
			
				Carte carte(j,i);
				deck.push_back(carte);
			}
		}
	}
	return deck;
}

void EstimationProba::melange(std::vector<Carte>& deck){
	std::random_shuffle(deck.begin(),deck.end());
}



void EstimationProba::rangeTable(std::vector<Carte>& table){

	std::vector<Carte> tableRange;
	int ind;

	while(table.size() != 0){
		Carte c = table[0];
		ind = 0;
		if(c.getRang() != 1){
            for(int j=0; j< (int) table.size();j++){
				if(table[j].getRang() == 1){
					ind = j;
					c = table[j];
				}else if ( c.getRang() != 1 && c.getRang() < table[j].getRang()) {
					ind = j;
					c = table[j];
				}
			
			}
			table.erase(table.begin() + ind);
			tableRange.push_back(c);
		}else{
			tableRange.insert(tableRange.begin(),c);
		}
	}
	
	table = tableRange;	
}


