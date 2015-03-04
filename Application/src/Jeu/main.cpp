#include "../../include/Jeu/Jeu.h"
#include <stdio.h>      
#include <stdlib.h>
#include <iostream>   

int main(){
	
	Jeu* jeu = new Jeu(1,20,1000,0);
	
	IntelligenceArtificielle IA(true, 1000,1);
	
	IA.setJeu(jeu);
	jeu->setJoueur(IA);
	
	jeu->distributionBlind();
	
	std::cout << jeu->getPot() << std::endl;
	
	jeu->distributionMain();
	
	((IntelligenceArtificielle) jeu->getJoueur(1)).jouer();
	
	std::cout << "--------------------------------------------------" << std::endl;
	
	((IntelligenceArtificielle) jeu->getJoueur(1)).executeCommande();
	
	std::cout << jeu->getPot() << std::endl;
	
	jeu->getJoueur(0).executeCommande();
	
	std::cout << jeu->getPot() << std::endl;
	
	free(jeu);
	return 1;
}  
