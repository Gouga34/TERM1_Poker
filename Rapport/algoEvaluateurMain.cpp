/**** Ce document contient une ébauche d'algorithme que l'on pourrait implémenter pour l'évaluateur

On peut faire une classe à part, comme ça on pourra s'en servir sans l'application si besoin ******/

class Evaluateur{

	tableau nbOccurences;
	/**Le tableau nbOccurences comprend 5 lignes et 13 colonnes. 
		Les quatre premières lignes correspondent aux quatre couleurs,
		La dernière ligne au nombre d'occurences de chaque carte, toutes couleurs confondues,
		Les 12 premières colonnes correspondent aux cartes (de 1 à AS, en comptant l'AS deux fois)
		La dernière colonne correspond au nombre de cartes d'une même couleurs.
	*/
	int combinaison;
	int poids;


	/**
	*@action rempli le tableau des nbOccurences
	*/
	void remplirTableau(){
		pour chaque carte{
			nbOccurences[couleur,val]++;
			nbOccurences[couleur, 13]++;
			nbOccurences[4,val]++;
		}
	}

	/**
	*@return true si à la ligne ligne, on a une suite à partir du départ départ
	*/
	bool estSuite(depart,ligne){
		int i=depart;
		while(i<13 && nbOccurences[ligne, i]>1 && (depart+5)>i){
			i++;
		}
		return((depart+5)==i)
	}

	/**
	* @return true si la ligne contient une suite
	*/
	bool contientSuite(ligne){
		int i=0;
		int cpt = 0;
		while(i<13 && cpt<5){
			if(nbOccurences[couleur,i]>=1){
				cpt++;
			}
			else{
				cpt=0;
			}
		}
		return (cpt==5);
	}

	/**
	* @return true si on a nb cartes identiques nbfois fois
	*/
	bool cartesIdentiques(nb, nbfois (1 par défaut)){
		Pour toutes les cartes{
			if (tab[derniereLigne,carte]==nb){
				cpt++;
				if(cpt==nbfois){
					return true;
				}
			}
		}
		return false;
	}

	/**
	* @return true si on a un flush
	*/
	bool flush(){
		Pour toutes les couleurs{
			if(tab[couleur,13]>=5){
				return true;
			}
		}
		return false;
	}

	int getCombinaison(){
		if (this.combinaison==null){
			combinaison=calculCombinaison();
		}

		return combinaison;
	}

	/**
	*@return la combinaison de la main
	*/
	int calculCombinaison(){
		//On commence par regarder si on a une quinte flush royale
		Pour chaque couleur{
			if(nbOccurences[couleur,13]>=5){
				if(estSuite(10,couleur)){
					return combi;
				}
			}
		}

		//On regarde si on a une quinte flush
		Pour chaque couleur{
			if (nbOccurences[couleur, 13]>=5){
				if(contientSuite(couleur)){
					return combi;
				}
			}
		}

		//On regarde si on a un carré
		if(cartesIdentiques(4)){
			return combi;
		}

		//On regarde si on a un full
		else if(cartesIdentiques(3) && cartesIdentiques(2)){
			return combi;
		}

		//On regarde si on a une couleur
		else if(flush()){
			return combi;
		}

		//On regarde si on a une suite
		else if(contientSuite(derniereLigne)){
			return combi;
		}

		//On regarde si on a un brelan
		else if(cartescombiIdentiques(3)){
			return combi;
		}

		//On regarde si on a une double paire
		else if(cartesIdentiques(2,2)){
			return combi;
		}

		//On regarde si on a une paire
		else if(cartesIdentiques(2)){
			return combi;
		}

		//Sinon, carte haute
		else{
			return combi;
		}

	}


	/**
	* @return true si main1 a gagné, false sinon
	*/
	public static bool comparerMains(main1,main2){
		if(main1.getCombinaison()>main2.getCombinaison()){
			return true;
		}
		else if(main1.getCombinaison()<main2.getCombinaison()){
			return false;
		}
		else{
			main1.calculerPoids();
			main2.calculerPoids();

			if(main1.getPoids()>main2.getPoids){
				return true;
			}
			else{
				return false;
			}
		}
	}



}