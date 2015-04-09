/*========================================================================
Nom: IntelligenceArtificielleProfilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015            Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe IntelligenceArtificielleProfilage.
=========================================================================*/


#include "../../include/IA/IntelligenceArtificielleProfilage.h"


IntelligenceArtificielleProfilage::IntelligenceArtificielleProfilage(bool estDealer, int jetons, int position)
    : IntelligenceArtificielle(estDealer, jetons, position), profilage(0), scenario(0)
{

}

IntelligenceArtificielleProfilage::IntelligenceArtificielleProfilage(Joueur joueur)
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

    scenario = new ScenariosDeTests(&profilJoueur, &resolveur->getCalibrage());
}

void IntelligenceArtificielleProfilage::remplissageDonneesProfilage() {

    int nbTotalActions = 0;

    for(int i = 0; i<3; i++){
        nbTotalActions += jeu->getJoueur(0)->getCompteurActions()[i];
    }

    //double resultat;

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].probaGainAdversaire = jeu->getJoueur(0)->getChancesGain();

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxMises += this->getCompteurActions()[0];
    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxSuivis += this->getCompteurActions()[1];
    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].tauxChecks += this->getCompteurActions()[2];

    if (this->getMisePlusHaute() > profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute) {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute = this->getMisePlusHaute();
    }

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].miseTotaleJoueur += this->getMiseTotale();

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].couche = jeu->estCouche(0);

    profilage->etatPartie[ETAPE_JEU::NB_ETAPES].nbTotalActions += nbTotalActions;


    /*resultat = CalculDonneesProfilage::taux(this->getCompteurActions()[0],nbTotalActions);
    if (resultat == -1) {
        profilage->etatPartie[jeu->getEtape()].tauxMises = 0;
    }
    else {
        profilage->etatPartie[jeu->getEtape()].tauxMises = resultat;
    }

    resultat = CalculDonneesProfilage::taux(this->getCompteurActions()[1],nbTotalActions);
    if (resultat == -1) {
        profilage->etatPartie[jeu->getEtape()].tauxSuivis = 0;
    }
    else {
        profilage->etatPartie[jeu->getEtape()].tauxSuivis = resultat;
    }

    resultat = CalculDonneesProfilage::taux(this->getCompteurActions()[2],nbTotalActions);
    if (resultat == -1) {
        profilage->etatPartie[jeu->getEtape()].tauxChecks = 0;
    }
    else {
        profilage->etatPartie[jeu->getEtape()].tauxChecks = resultat;
    }

    resultat = CalculDonneesProfilage::taux(this->getMisePlusHaute(),this->getCave());
    if (resultat == -1) {
        profilage->etatPartie[jeu->getEtape()].misePlusHaute = 0;
    }
    else {
        profilage->etatPartie[jeu->getEtape()].misePlusHaute = resultat;
    }

    resultat = CalculDonneesProfilage::taux(this->getMiseTotale(),this->getCave());
    if (resultat == -1) {
        profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur = 0;
    }
    else {
        profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur = resultat;
    }

    profilage->etatPartie[jeu->getEtape()].tauxAgressivite = CalculDonneesProfilage::agressivite(profilage->etatPartie[jeu->getEtape()].misePlusHaute,profilage->etatPartie[jeu->getEtape()].tauxMises,profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur);
    profilage->etatPartie[jeu->getEtape()].tauxRationnalite = CalculDonneesProfilage::rationalite(profilage->etatPartie[jeu->getEtape()].probaGainAdversaire,profilage->etatPartie[jeu->getEtape()].miseTotaleJoueur);
    profilage->etatPartie[jeu->getEtape()].tauxPassivite = CalculDonneesProfilage::passivite( profilage->etatPartie[jeu->getEtape()].tauxSuivis, profilage->etatPartie[jeu->getEtape()].tauxChecks);
    profilage->etatPartie[jeu->getEtape()].tauxBluff = CalculDonneesProfilage::bluff(profilage->etatPartie[jeu->getEtape()].tauxRationnalite);

    profilage->etatPartie[jeu->getEtape()].pot = jeu->getPot();

    profilage->etatPartie[jeu->getEtape()].couche = jeu->estCouche(0);
    */
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

    /** TODO Changer le 1000 par la cave en début de partie **/
    resultat = CalculDonneesProfilage::taux(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute,1000);
    if (resultat == -1) {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute = 0;
    }
    else {
        profilage->etatPartie[ETAPE_JEU::NB_ETAPES].misePlusHaute = resultat;
    }

    /** TODO Changer le 1000 par la cave en début de partie **/
    resultat = CalculDonneesProfilage::taux(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].miseTotaleJoueur,1000);
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


    //On récupère les 4 taux des 4 parties et on en fait la moyenne, pour chaque type de jeu

    /*double sommeAgressivite=0;
    double sommeRationalite=0;
    double sommePassivite=0;
    double sommeBluff=0;
    for(int i=0;i<ETAPE_JEU::NB_ETAPES;i++){
        sommeAgressivite+=profilage->etatPartie[i].tauxAgressivite;
        sommeRationalite+=profilage->etatPartie[i].tauxRationnalite;
        sommePassivite+=profilage->etatPartie[i].tauxPassivite;
        sommeBluff+=profilage->etatPartie[i].tauxBluff;
    }

    profilage->profilJoueur->setAgressivite(sommeAgressivite/ETAPE_JEU::NB_ETAPES);
    profilage->profilJoueur->setRationalite(sommeRationalite/ETAPE_JEU::NB_ETAPES);
    profilage->profilJoueur->setPassivite(sommePassivite/ETAPE_JEU::NB_ETAPES);
    profilage->profilJoueur->setBluff(sommeBluff/ETAPE_JEU::NB_ETAPES);
*/
    profilage->sauvegarder();
}

void IntelligenceArtificielleProfilage::ecritureScenariosDeTests() {
    scenario->setCalibrageActuelIA(&resolveur->getCalibrage());
    scenario->setChancesDeGain(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].probaGainAdversaire);
    scenario->sauvegarderPartie();
}
