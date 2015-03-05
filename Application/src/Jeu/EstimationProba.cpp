#include "../../include/Jeu/EstimationProba.h"
#include "../../include/Jeu/Carte.h"

#include <string>
#include <unordered_map>
#include <vector>

int nombreDeTest = 20000;

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
    int tailleTable;
	std::string ensembleCourant;
	std::vector<Carte> table;
	std::vector<Carte> mainAdverse;
	std::vector<Carte> deck;
	std::unordered_map<std::string,int> listeEnsembleCarte;
	std::unordered_map<std::string,int>::const_iterator iterateurEnsembleCarte;
	
	for(int t=0; t <nombreDeTest; t++){
		do{
            deck.clear();
            deck= this->nouveauDeck();
			this->melange(deck);
			table.clear();
			mainAdverse.clear();
            ensembleCourant.clear();;
			table = jeuCourant->getTable();
            tailleTable =  5 - (int) table.size();

            for(int i=0; i< tailleTable; i++){
				position = rand() % deck.size();
                ensembleCourant += std::to_string(deck.at(position).getRang());
                ensembleCourant += std::to_string(deck.at(position).getCouleur()) ;
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
		
		
        if(Evaluateur::comparerMains(table,this->joueurCourant->getMain(), mainAdverse)){

            std::cout << std::endl << "Table : " << std::endl;
            for(int i=0; i<5; i++){
                std::cout << table.at(i).getRang() << "," << table.at(i).getCouleur() << "; ";
            }
            std::cout << std::endl;


            std::cout << "Main adverse : " << std::endl;
            for(int i=0; i<2; i++){
                std::cout << mainAdverse.at(i).getRang()  << "," << mainAdverse.at(i).getCouleur();
            }

            std::cout << std::endl;
            std::cout << "Main du joueur : " << std::endl;
            for(int i=0; i<2; i++){
                std::cout << this->joueurCourant->getMain().at(i).getRang()  << "," << this->joueurCourant->getMain().at(i).getCouleur() << "; ";
            }

			nombreDeCoupGagner++;
            std::cout << std::endl;
		}
	
	
	}
	
    return (double) nombreDeCoupGagner / nombreDeTest;

}


std::vector<Carte> EstimationProba::nouveauDeck(){
	std::vector<Carte> deck;
	
    for(int i =COULEUR_CARTE::PIQUE; i<=COULEUR_CARTE::CARREAU; i++){
        for(int j=RANG_CARTE::AS; j<=RANG_CARTE::K; j++){			
                Carte carte(j,i);
				deck.push_back(carte);
		}
	}

    deck.erase(deck.begin() + this->joueurCourant->getMain().at(0).getId());
    deck.erase(deck.begin() + this->joueurCourant->getMain().at(1).getId());

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


