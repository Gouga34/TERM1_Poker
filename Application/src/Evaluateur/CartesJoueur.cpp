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
    initialiserOccurrences();
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

void CartesJoueur::initialiserOccurrences(){
    for(int i=0;i<15;i++){
        for(int j=0;j<5;j++){
            occurrences[i][j]=0;
        }
    }
}

void CartesJoueur::remplirTableau(vector<Carte> table, vector<Carte> mainJoueur){
    enregistrerOccurrencesCartes(mainJoueur);
    enregistrerOccurrencesCartes(table);
}

void CartesJoueur::enregistrerOccurrencesCartes(vector<Carte> listeCartes){
    for (vector<Carte>::iterator it = listeCartes.begin(); it != listeCartes.end(); ++it){
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

void CartesJoueur::quinteFlushRoyale(){
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
}

void CartesJoueur::quinteFlush(){
        int i=0;
        while(i<4 && getCombinaison()==0){
            if (occurrences[14][i]>=5){
                if(contientSuite(i)){
                    setCombinaison(QUINTE_FLUSH);
                }
            }
            i++;
        }
}

int CartesJoueur::carre(){
    int carte = -1;
//On regarde si on a un carré
    if((carte=cartesIdentiques(4)) > 0 ){
        setCombinaison(CARRE);
        setPoids(carte);
        carte =0;
    }
    return carte;
}

int CartesJoueur::full(){
    int carte = -1;
    if((carte=cartesIdentiques(3))>0 && cartesIdentiques(2)>0 ){
        setCombinaison(FULL);
        setPoids(carte);
        carte = 0;
    }
    return carte;
}

int CartesJoueur::brelan(){
    int carte=-1;
    if((carte = cartesIdentiques(3))>0){
        setCombinaison(BRELAN);
        setPoids(carte);
        carte = 0;
    }
    return carte;
}

int CartesJoueur::doublePaire(){
    int carte=-1;
    if((carte=cartesIdentiques(2,2))>0){
        setCombinaison(DOUBLE_PAIRE);
        setPoids(carte);
        carte=0;
    }
    return carte;
}

void CartesJoueur::paire(){
    int carte=-1;
    if((carte=cartesIdentiques(2,1))>0){
        setCombinaison(PAIRE);
        setPoids(carte);
    }
}

void CartesJoueur::calculCombinaison(){

	//On commence par regarder si on a une quinte flush royale
    quinteFlushRoyale();

    if(getCombinaison()==0){
        //QuinteFlush ?
        quinteFlush();

        if(getCombinaison()==0){
            //Carre ?
            int carte = carre();
            //Full ?
            if(carte==-1){
                carte = full();
            }
            if(carte==-1){
                //Flush?
                if(flush()){
                    setCombinaison(COULEUR);
                }
                //Suite?
                else if(contientSuite(4)){
                    setCombinaison(SUITE);
                }
                else {
                    //Brelan?
                    carte = brelan();
                    if(carte==-1){
                        //DoublePaire ?
                        carte = doublePaire();
                        if(carte==-1){
                            //Paire ?
                            paire();

                            //Carte haute
                            if(getCombinaison()==0){
                                setCombinaison(CARTE_HAUTE);
                            }
                        }
                    }
                }
            }
        }
    }
}


void CartesJoueur::setPoidsCarteLaPlusHaute(){
    int numeroLigne=-1;
    if(getCombinaison()==COULEUR){
        numeroLigne=getPoids();
    }
    else if(getCombinaison()==CARTE_HAUTE){
        numeroLigne=4;
    }

    int i=13;
    int p=0;
    while(p==0 && i>0){
        if(occurrences[i][numeroLigne]>0){
            setPoids(i);
            p=i;
        }
        i--;
    }
}

void CartesJoueur::calculerPoidsBasique(){
       setPoidsCarteLaPlusHaute();
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
