/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 04/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Evaluateur.
=========================================================================*/
#include "../../include/Evaluateur/Evaluateur.h"
#include "../../include/Constantes.h"

using namespace std;

int Evaluateur::comparerMains(vector<Carte> table, vector<Carte> mainJoueur1, vector<Carte> mainJoueur2){

	CartesJoueur main1(table, mainJoueur1);

	CartesJoueur main2(table, mainJoueur2);

	if(main1.getCombinaison()>main2.getCombinaison()){
			return 1;

		
		}
		else if(main1.getCombinaison()<main2.getCombinaison()){
			return 0;
		
		}
		else{
				return(main1.comparerCombinaisonsEgales(main2));	
			
		}
}
