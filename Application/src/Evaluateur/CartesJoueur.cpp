/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 09/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Evaluateur.
=========================================================================*/

#include "../../include/Evaluateur/CartesJoueur.h"
#include "../../include/Constantes.h"

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

        if(carte==AS){
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

       if(carte==AS){
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
   // for(int i=0;i<=14;i++){
   //     cout<<i<<" ";
   //     for(int j=0;j<=4;j++){
   //         cout<<occurrences[i][j]<<" ";
   //     }
   //     cout<<endl;
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

bool CartesJoueur::contientSuite(int ligne) {

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
	if(cpt==5){
		setPoids(i);
	}

	return (cpt==5);	
}

int CartesJoueur::cartesIdentiques(int nb, int nbfois) const{

    int poids=0;
	int cpt=0;
	for(int i=13; i>0;i--){
		if (occurrences[i][4]==nb){
			cpt++;
            if(poids==0){
                poids=i;
            }

			if(cpt==nbfois){
                return poids;
			}
		}
	}
	return -1;
}

bool CartesJoueur::flush() {

	for(int i=0; i<4;i++){
		if(occurrences[14][i]>=5){
			//On dit que le poids est égal à la ligne de la couleur 
			//afin de pouvoir trouver plus tard si égalité la carte la plus haute de la couleur 
			//sans avoir à chercher à nouveau quelle couleur c'est
            setPoids(i);
            return true;
		}
	}
	return false;
}

void CartesJoueur::calculCombinaison(){

	//On commence par regarder si on a une quinte flush royale
        int i=0;
        while(i<4 && getCombinaison()==0){
			if(occurrences[14][i]>=5){
				
				if(estSuite(9,i)){
					setCombinaison(QUINTE_FLUSH_ROYALE);
					setPoids(14);
				}
			}
            i++;
		}

		if(getCombinaison()==0){
			//On regarde si on a une quinte flush
                i=0;
                while(i<4 && getCombinaison()==0){
                    if (occurrences[14][i]>=5){
						if(contientSuite(i)){
							setCombinaison(QUINTE_FLUSH);
						}
					}
                    i++;
				}

				if(getCombinaison()==0){
					int carte;
				//On regarde si on a un carré
					if((carte=cartesIdentiques(4)) > 0 ){
						setCombinaison(CARRE);
						setPoids(carte);
						carte =0;
					}
				
				//On regarde si on a un full
					else if((carte=cartesIdentiques(3))>0 && cartesIdentiques(2)>0 ){
                        setCombinaison(FULL);
                        setPoids(carte);
                        carte = 0;
					}

				//On regarde si on a une couleur
					else if(flush()){
						setCombinaison(COULEUR);
					}

				//On regarde si on a une suite
					else if(contientSuite(4)){
						setCombinaison(SUITE);
					}

				//On regarde si on a un brelan
					else if((carte = cartesIdentiques(3))>0){
						setCombinaison(BRELAN);
						setPoids(carte);
						carte = 0;
					}

				//On regarde si on a une double paire
					else if((carte=cartesIdentiques(2,2))>0){
						setCombinaison(DOUBLE_PAIRE);
						setPoids(carte);
                        carte=0;
                    }

				//On regarde si on a une paire
					else if((carte=cartesIdentiques(2,1))>0){
						setCombinaison(PAIRE);
						setPoids(carte);
					}

				//Sinon, carte haute
					else{
						setCombinaison(CARTE_HAUTE);
					}
				}
		}
}


void CartesJoueur::calculerPoidsBasique(){
	if(getCombinaison()==COULEUR){ //on met le poids à la carte la plus haute (-1)
		int i=13;
		int p=0;
		while(p==0 && i>0){
			if(occurrences[i][getPoids()]>0){
				setPoids(i);
				p=i;
			}
			i--;
		}
	}
	else if(getCombinaison()==CARTE_HAUTE){//on met le poids à la carte la plus haute (-1)
		int i=13;
		int p=0; //la carte la plus haute
		while(p==0 && i>0){
			if(occurrences[i][4]>0){
				p=i;
				setPoids(p);
			}
			i--;
		}
	}
}

 RESULTAT_PARTIE CartesJoueur::comparerCombinaisonsEgales(CartesJoueur main2){
 	
 	if(getPoids()==0){
 		calculerPoidsBasique();
 	}
 	if(main2.getPoids()==0){
 		main2.calculerPoidsBasique();
 	}


 	if(getPoids()==main2.getPoids()){
 		if(getCombinaison()==CARRE){
            int p = 0, p2 = 0;
 			int i = 13;
 			//On calcule la carte la plus haute autre que celles du carré pour this
            while(p==0 && p2==0 && i>0){
 				if(occurrences[i][4]<4 && occurrences[i][4]>0){
 					p=i;
                }

                if(main2.occurrences[i][4]<4 && main2.occurrences[i][4]>0){
                    p2=i;
                }

 				i--;
 			}


            if(p>p2){
 				return GAGNE;
 			}
            else if(p<p2){
 				return PERDU;
 			}
 			else{
 				return EGALITE;
 			}
 		}
 		else if(getCombinaison()==FULL){

 			//On va donc comparer les cartes de la paire
 			setPoids(cartesIdentiques(2));
            main2.setPoids(main2.cartesIdentiques(2));
 		}
 		else if(getCombinaison()==COULEUR){

 			int p=0; int i=13;
 			flush();
 			int couleurThis=getPoids();
 			main2.flush();
 			int couleurMain2=getPoids();

 			while(i>0 && p==0){
                main2.setPoids(0);
                setPoids(0);
 				if(occurrences[i][couleurThis]>0){

 					setPoids(i);
 				}
 				if(main2.occurrences[i][couleurMain2]>0){
 					main2.setPoids(i);

 				}
 				if(occurrences[i][couleurThis]!=occurrences[i][couleurMain2]){
 					p=1;
 				}
 				i--;
 			}


 		}
 		else if(getCombinaison()==BRELAN){
 			int p=0; int i=13; int cpt=3;
 			while(i>0 && p==0){
                main2.setPoids(0);
                setPoids(0);
 				if(occurrences[i][4]==1 || main2.occurrences[i][4]==1){
 					cpt++;
 				}
 				if(occurrences[i][4]>0 && occurrences[i][4]!=3){
 					setPoids(i);
 				}
 				if(main2.occurrences[i][4]>0 && main2.occurrences[i][4]!=3){
 					main2.setPoids(i);
 				} 	
 				if(getPoids()!=main2.getPoids()){
 					p=1;
 				}
 				i--;
 			}
 		}
 		else if(getCombinaison()==DOUBLE_PAIRE){


 			int p=0;
 			int i=13;
 			int poidsCarte = getPoids();
 			main2.setPoids(0);
            setPoids(0);
 			//On commence par comparer la seconde paire
			while(i>0 && p==0){
                main2.setPoids(0);
                setPoids(0);
				if (occurrences[i][4]==2 && i!=poidsCarte){
					setPoids(i);
				}
				if (main2.occurrences[i][4]==2 && i!=poidsCarte){
					main2.setPoids(i);
				}
				if(getPoids()!=main2.getPoids()){
					p=1;
				}

				i--;
			}

			//Si la seconde paire est la même, on compare la carte la plus haute, mais si on a une autre paire, faut quand même que ça soit la carte haute!
			if(getPoids()==main2.getPoids()){
				int p=0;
 				int i=13;
 				int poidsCartePaireMain1=getPoids();
 				int poidsCartePaireMain2=main2.getPoids();

 				//On remet les poids à 0 pour compter juste une seule carte.
 				setPoids(0);
 				main2.setPoids(0);
 				while(i>0 && p==0){
					if ((occurrences[i][4]==1 || (i<poidsCartePaireMain1 && occurrences[i][4]>=1)) && getPoids()==0){
						setPoids(i);
					}
					if ((main2.occurrences[i][4]==1 || (i<poidsCartePaireMain2 && occurrences[i][4]>=1)) && main2.getPoids()==0){
						main2.setPoids(i);
					}
					if(getPoids()>0 || main2.getPoids()>0){//si au moins un nouveau poids, on arrête
						p=1;
					}
					i--;
				}

			}
 		}
 		else if(getCombinaison()==PAIRE){
 			int cpt=2;
 			int p=0;
 			int i=13;
 			while(cpt<5 && i>0 && p==0){
                main2.setPoids(0);
                setPoids(0);

 				if(occurrences[i][4]==1 || main2.occurrences[i][4]==1){
 					cpt++;
 				}
                if(occurrences[i][4]==1){
                    setPoids(i);

 				}
                if(main2.occurrences[i][4]==1){
                    main2.setPoids(i);
 				}

 				if(getPoids()!=main2.getPoids()){
                    p=1;
                }
 				i--;
 			}
 		}
 		else if(getCombinaison()==CARTE_HAUTE){
 			int cpt=1;
 			int p=0;
 			int i=getPoids();
 			while(cpt<5 && i>0 && p==0){
                main2.setPoids(0);
                setPoids(0);

 				if(occurrences[i][4]==1){
 					
 					setPoids(i);
 				}
 				if(main2.occurrences[i][4]==1){
 					main2.setPoids(i);
 				}

 				if(getPoids()!=main2.getPoids()){
 					p=1;
 				}
 				i--;
 			}
		}



 		if(getPoids()>main2.getPoids()){

			return GAGNE;
		}
		else if(getPoids()<main2.getPoids()){
			return PERDU;
		}
		else{

			return EGALITE;
		}

 	}
 	else if(getPoids()>main2.getPoids()){
 		return GAGNE;
 	}
 	else {
 		return PERDU;
 	}


 }
