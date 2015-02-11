#include "../include/Jeu.h"
#include <stdio.h>      
#include <stdlib.h>
#include <iostream>   

using namespace std;

int main(){
	
	Jeu jeu(2,20,1000,0);
	
	IntelligenceArtificielle IA(false, 1000);
	Carte c1(13, 2);
	Carte c2(12, 2);

	IA.ajouteCarte(c2);
	IA.ajouteCarte(c1);
	
	IA.jouer();

	return 1;
}  
