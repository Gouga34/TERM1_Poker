#include "../../include/IA/EstimationProba.h"
#include "../../include/Jeu/Carte.h"

#include <string>
#include <unordered_map>
#include <vector>



double EstimationProba::estimation(Jeu* jeuCourant, Joueur* joueurCourant){

    double nombreDeCoupGagner = 0;
	int position;
    int tailleTable;
	std::string ensembleCourant;
	std::vector<Carte> table;
	std::vector<Carte> mainAdverse;
	std::vector<Carte> deck;
    std::unordered_map<std::string,int> listeEnsembleCarte;
    std::unordered_map<std::string,int>::const_iterator iterateurEnsembleCarte;
	
    for(int t=0; t <NOMBRE_DE_TESTS; t++){
        //do{
            deck.clear();
            deck= nouveauDeck(joueurCourant);
            EstimationProba::melange(deck);
			table.clear();
			mainAdverse.clear();
            //ensembleCourant.clear();;
			table = jeuCourant->getTable();
            tailleTable =  5 - (int) table.size();

            for(int i=0; i< tailleTable; i++){
				position = rand() % deck.size();
                //ensembleCourant += std::to_string(deck.at(position).getRang());
                //ensembleCourant += std::to_string(deck.at(position).getCouleur()) ;
				table.push_back(deck.at(position));
				deck.erase(deck.begin() + position);
			}

			for(int i=0; i< 2; i++){
				position = rand() % deck.size();
                //ensembleCourant += std::to_string(deck.at(position).getRang()) + std::to_string(deck.at(position).getCouleur()) ;
				mainAdverse.push_back(deck.at(position));
				deck.erase(deck.begin() + position);
            }
			
            //iterateurEnsembleCarte  = listeEnsembleCarte.find (ensembleCourant);

        //}while( iterateurEnsembleCarte != listeEnsembleCarte.end());
		
        bool comparaisonMains = Evaluateur::comparerMains(table,joueurCourant->getMain(), mainAdverse);

        if(comparaisonMains==GAGNE){
			nombreDeCoupGagner++;

        }else if(comparaisonMains==EGALITE){
            nombreDeCoupGagner = nombreDeCoupGagner;
        }
	}
	
    return (double) nombreDeCoupGagner / NOMBRE_DE_TESTS;

}


std::vector<Carte> EstimationProba::nouveauDeck(Joueur* joueurCourant){
	std::vector<Carte> deck;
	
    for(int i =COULEUR_CARTE::PIQUE; i<=COULEUR_CARTE::CARREAU; i++){
        for(int j=RANG_CARTE::AS; j<=RANG_CARTE::K; j++){			
                Carte carte(j,i);
				deck.push_back(carte);
		}
	}

    deck.erase(deck.begin() + joueurCourant->getMain().at(0).getId());
    deck.erase(deck.begin() + joueurCourant->getMain().at(1).getId());

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


