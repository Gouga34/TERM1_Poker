/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 04/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Evaluateur.
=========================================================================*/

#include "../../include/Evaluateur/CartesJoueur.h"

using namespace std;

CartesJoueur::CartesJoueur(vector<Carte> table, vector<Carte> mainJoueur){
		poids=0;
		combinaison=0;
		for(int i=0;i<15;i++){
			for(int j=0;j<5;j++){
				occurrences[i][j]=0;
			}
		}

	remplirTableau(table,mainJoueur);
	calculCombinaison();

}

CartesJoueur::~CartesJoueur(){

}


int CartesJoueur::getCombinaison() const{
	return combinaison;
}

int CartesJoueur::getPoids() {
	if(poids==0 && combinaison==1){
		calculerPoids();
	}
	return poids;
}


void CartesJoueur::setCombinaison(int combi){
	combinaison=combi;
}

void CartesJoueur::setPoids(int p){
	poids=p;
}


void CartesJoueur::remplirTableau(vector<Carte> table, vector<Carte> mainJoueur){

	//Enregistrement des occurrences des cartes du joueur
	for (vector<Carte>::iterator it = mainJoueur.begin(); it != mainJoueur.end(); ++it){
		
		int carte = it->getRang();
		int couleur = it->getCouleur();
		
		
		if(carte==1){
			occurrences[0][couleur]++;
			occurrences[13][couleur]++;
			occurrences[0][4]++;
			occurrences[13][4]++;
		}
		else{
			
			occurrences[carte-1][couleur]++;
			occurrences[carte-1][4]++;

		}

		occurrences[14][couleur]++;
		

	}


	//Enregistrement des occurrences des cartes de la table
	for (vector<Carte>::iterator it = table.begin(); it != table.end(); ++it){
		int carte = it->getRang();
		int couleur = it->getCouleur();
		
		if(carte==1){
			occurrences[0][couleur]++;
			occurrences[13][couleur]++;
			occurrences[0][4]++;
			occurrences[13][4]++;
		}
		else{
			
			occurrences[carte-1][couleur]++;
			occurrences[carte-1][4]++;

		}

		occurrences[14][couleur]++;
	}

		// for(int i=0;i<15;i++){
		// 	for(int j=0;j<5;j++){
		// 		cout<<occurrences[i][j];
		// 	}
		// 	cout<<endl;
		// }

}


bool CartesJoueur::estSuite(int depart, int ligne) const{
	int i=depart;
	while(i<14 && occurrences[i][ligne]>=1 && (depart+5)>i){

		i++;

	}

	int fin=depart +5;

	return(fin==i);
}

bool CartesJoueur::contientSuite(int ligne) const{
    //cout<<"contientSuite"<<endl;
	int i=0;
	int cpt = 0;
	while(i<14 && cpt<5){
		if(occurrences[i][ligne]>=1){
			cpt++;
		}
		else{
			cpt=0;
		}


		i++;
	}
    //cout<<cpt<<" "<<i<<endl;
	return (cpt==5);	
}

bool CartesJoueur::cartesIdentiques(int nb, int nbfois) const{
    //cout<<"cartesIdentiques("<<nb<<","<<nbfois<<")"<<endl;
	int cpt=0;
	for(int i=0; i<12;i++){
		if (occurrences[i][4]==nb){
			cpt++;

			if(cpt==nbfois){
                //cout<<"return true"<<endl;
				return true;
			}
		}
	}
	return false;
}

bool CartesJoueur::flush() const{

	for(int i=0; i<4;i++){
		if(occurrences[14][i]>=5){
			return true;
		}
	}
	return false;
}

void CartesJoueur::calculCombinaison(){

	//On commence par regarder si on a une quinte flush royale
		for(int i=0;i<4;i++){
	
			if(occurrences[14][i]>=5){
				
				if(estSuite(9,i)){
					setCombinaison(10);
				}

				
			}
			
		}

		if(getCombinaison()==0){
			//On regarde si on a une quinte flush
				for(int i=0;i<4;i++){

					if (occurrences[14][i]>=5){
							
						if(contientSuite(i)){
							setCombinaison(9);
						}
					}
				}

				if(getCombinaison()==0){
				//On regarde si on a un carré
					if(cartesIdentiques(4)){
						setCombinaison(8);
					}
				
				//On regarde si on a un full
					else if(cartesIdentiques(3) && cartesIdentiques(2) ){
								setCombinaison(7);
							
					}

				//On regarde si on a une couleur
					else if(flush()){

						setCombinaison(6);
					}

				//On regarde si on a une suite
					else if(contientSuite(4)){
						setCombinaison(5);
					}

				//On regarde si on a un brelan
					else if(cartesIdentiques(3)){
						setCombinaison(4);
					}

				//On regarde si on a une double paire
					else if(cartesIdentiques(2,2)){
						setCombinaison(3);
					}

				//On regarde si on a une paire
					else if(cartesIdentiques(2)){
						setCombinaison(2);
					}

				//Sinon, carte haute
					else{
						setCombinaison(1);
					}
				}
		}
        //cout<<"combinaison calculée : "<<getCombinaison()<<endl;
}


void CartesJoueur::calculerPoids(){
	int i=13;
	int p=0; //la carte la plus haute
	while(p=0 && i>0){
		if(occurrences[i][4]>0){
			p=(i + 1);
			setPoids(p);
		}
		i++;
	}
}
