#include "../../include/IA/EstimationProba.h"
#include "../../include/Jeu/Carte.h"

#include <string>
#include <unordered_map>
#include <vector>


EstimationProba::EstimationProba(int nombreTests)
    : nbTests(nombreTests), jeu(0), joueur(0), resultatRestimation(0) {

}

EstimationProba::EstimationProba(Jeu *j, Joueur *jo, int nombreTests)
    : nbTests(nombreTests), jeu(j), joueur(jo), resultatRestimation(0) {

}

double EstimationProba::getResultat() const {
    return resultatRestimation;
}

void EstimationProba::setNbTests(int tests) {
    nbTests = tests;
}

void EstimationProba::setJeu(Jeu *j) {
    jeu= j;
}

void EstimationProba::setJoueur(Joueur *jo) {
    joueur = jo;
}

void EstimationProba::run() {
    resultatRestimation = estimation(jeu, joueur, nbTests);
}

double EstimationProba::estimation(Jeu* jeuCourant, Joueur* joueurCourant, int nombreTests){

    double nombreDeCoupGagner = 0;
	int position;
    int tailleTable;
    //std::string ensembleCourant;
	std::vector<Carte> table;
	std::vector<Carte> mainAdverse;
	std::vector<Carte> deck;
//    std::unordered_map<std::string,int> listeEnsembleCarte;
//    std::unordered_map<std::string,int>::const_iterator iterateurEnsembleCarte;
	
    for(int t=0; t <nombreTests; t++){
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
	
    return (double) nombreDeCoupGagner / nombreTests;
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
