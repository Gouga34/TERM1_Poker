/*========================================================================
Nom: IntelligenceArtificielleProfilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015            Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe IntelligenceArtificielleProfilage.
=========================================================================*/


#include "../../include/IA/IntelligenceArtificielleProfilage.h"


IntelligenceArtificielleProfilage::IntelligenceArtificielleProfilage(bool estDealer, int jetons, int position)
    : IntelligenceArtificielle(estDealer, jetons, position), continuerProfilage(true), profilage(0), scenario(0)
{

}

IntelligenceArtificielleProfilage::IntelligenceArtificielleProfilage(const IntelligenceArtificielle& joueur)
    : IntelligenceArtificielle(joueur), profilage(0), scenario(0)
{

}

IntelligenceArtificielleProfilage::~IntelligenceArtificielleProfilage()
{
    if (profilage != 0) {
        delete profilage;
    }

    if (scenario != 0) {
        delete scenario;
    }
}

Profilage* IntelligenceArtificielleProfilage::getProfilage() const {
    return profilage;
}

void IntelligenceArtificielleProfilage::setPseudoJoueurProfile(std::string pseudo) {
    profilJoueur.setPseudo(pseudo);

    if (profilage) {
        delete profilage;
    }
    profilage = new Profilage(&profilJoueur);

    Profil calibrageRecherche;
    if (!jeu->getJoueur(0)->estHumain()) {
        IntelligenceArtificielle *iaProfilee = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0));
        calibrageRecherche = iaProfilee->getCalibrage();
    }

    scenario = new ScenariosDeTests(&profilJoueur, resolveur->getCalibrage(), calibrageRecherche);
}

void IntelligenceArtificielleProfilage::remplissageDonneesProfilage() {

    int nbTotalActions = 0;

    for(int i = 0; i<3; i++){
        nbTotalActions += jeu->getJoueur(0)->getCompteurActions()[i];
    }

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].probaGainAdversaire = jeu->getJoueur(0)->getChancesGain();

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxMises += jeu->getJoueur(0)->getCompteurActions()[0];
    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxSuivis += jeu->getJoueur(0)->getCompteurActions()[1];
    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxChecks += jeu->getJoueur(0)->getCompteurActions()[2];

    if (jeu->getJoueur(0)->getMisePlusHaute() > profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute) {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute = jeu->getJoueur(0)->getMisePlusHaute();
    }

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].miseTotaleJoueur = jeu->getJoueur(0)->getMiseTotale();

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].couche = jeu->estCouche(0);

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].nbTotalActions += nbTotalActions;
}


void IntelligenceArtificielleProfilage::calculProfilGlobalJoueur() {

    double resultat;

    resultat = CalculDonneesProfilage::taux(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxMises,profilage->etatPartie[ETAPE_JEU::NB_ETAPES].nbTotalActions);
    if (resultat == -1) {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxMises = 0;
    }
    else {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxMises = resultat;
    }

    resultat = CalculDonneesProfilage::taux(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxSuivis,profilage->etatPartie[ETAPE_JEU::NB_ETAPES].nbTotalActions);
    if (resultat == -1) {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxSuivis = 0;
    }
    else {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxSuivis = resultat;
    }

    resultat = CalculDonneesProfilage::taux(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxChecks,profilage->etatPartie[ETAPE_JEU::NB_ETAPES].nbTotalActions);
    if (resultat == -1) {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxChecks = 0;
    }
    else {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxChecks = resultat;
    }

    resultat = CalculDonneesProfilage::taux(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute,CAVE_JOUEURS);
    if (resultat == -1) {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute = 0;
    }
    else {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute = resultat;
    }

    resultat = CalculDonneesProfilage::taux(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].miseTotaleJoueur,CAVE_JOUEURS);
    if (resultat == -1) {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].miseTotaleJoueur = 0;
    }
    else {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].miseTotaleJoueur = resultat;
    }

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxAgressivite = CalculDonneesProfilage::agressivite(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute,profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxMises,profilage->etatPartie[ETAPE_JEU::NB_ETAPES].miseTotaleJoueur);
    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxRationnalite = CalculDonneesProfilage::rationalite(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].probaGainAdversaire,profilage->etatPartie[ETAPE_JEU::NB_ETAPES].miseTotaleJoueur);
    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxPassivite = CalculDonneesProfilage::passivite( profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxSuivis, profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxChecks);
    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxBluff = CalculDonneesProfilage::bluff(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxRationnalite);


    profilage->profilJoueur->setAgressivite(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxAgressivite);
    profilage->profilJoueur->setRationalite(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxRationnalite);
    profilage->profilJoueur->setPassivite(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxPassivite);
    profilage->profilJoueur->setBluff(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxBluff);

    RESULTAT_PARTIE resultatPartie = getJeu()->getResultatPartie();
    if(resultatPartie==GAGNE){
        resultatPartie=PERDU;
    }
    else if(resultatPartie==PERDU){
        resultatPartie=GAGNE;
    }

    profilage->partieGagneeIAQuiProfile=resultatPartie;

    profilage->sauvegarder();
}

void IntelligenceArtificielleProfilage::ecritureScenariosDeTests() {
    // Si on profile le joueur
    if (continuerProfilage) {
        scenario->setCalibrageActuelIA(resolveur->getCalibrage());
        scenario->setChancesDeGain(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].probaGainAdversaire);
        continuerProfilage = scenario->sauvegarderPartie();
    }
}
