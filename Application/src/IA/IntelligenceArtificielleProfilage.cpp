/*========================================================================
Nom: IntelligenceArtificielleProfilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015            Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe IntelligenceArtificielleProfilage.
=========================================================================*/


#include "../../include/IA/IntelligenceArtificielleProfilage.h"
#include <QStringList>
#include <QTextStream>


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
    profilage = new Profilage(resolveur->getCalibrage(), &profilJoueur);

    Profil calibrageRecherche;
    if (!jeu->getJoueur(0)->estHumain()) {
        IntelligenceArtificielle *iaProfilee = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0));
        calibrageRecherche = *(iaProfilee->getCalibrage());
    }

    continuerProfilage = (NOMBRE_PARTIES_PROFILAGE > 0);
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
        profilage->nbJetonsGagnesIAQuiProfile=getCave()-CAVE_JOUEURS;
        resultatPartie=PERDU;
    }
    else if(resultatPartie==PERDU){
        resultatPartie=GAGNE;
        profilage->nbJetonsGagnesIAQuiProfile=getCave()-CAVE_JOUEURS;
    }
    else{
        profilage->nbJetonsGagnesIAQuiProfile=0;
    }

    profilage->scenarioDeTest=continuerProfilage;

    profilage->partieGagneeIAQuiProfile=resultatPartie;

    profilage->sauvegarder();
}

void IntelligenceArtificielleProfilage::ecritureScenariosDeTests() {
    // Si on profile le joueur
    if (continuerProfilage && !(CALCUL_CALIBRAGE_IDEAL)) {
        scenario->setCalibrageActuelIA(resolveur->getCalibrage());
        scenario->setChancesDeGain(profilage->etatPartie[ETAPE_JEU::NB_ETAPES].probaGainAdversaire);
        continuerProfilage = scenario->sauvegarderPartie();
    }
}

double IntelligenceArtificielleProfilage::calculValeurProportionnelle(double minVal1, double val1, double maxVal1, double minVal2, double maxVal2) const {
    return ((val1 - minVal1) * ((maxVal2 - minVal2) / (maxVal1 - minVal1)) + minVal2);
}

void IntelligenceArtificielleProfilage::setCalibragePourJouer() {

    if (!continuerProfilage && !(CALCUL_CALIBRAGE_IDEAL)) {

        /////// Lecture du calibrage pour gagner //////////

        QFile fichier(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+"jeu_IA.csv");
        if(!fichier.open(QIODevice::ReadOnly)){
            std::cerr<<"Erreur lors de l'ouverture du fichier jeu_IA.csv"<<std::endl;
            return;
        }

        QString ligne = fichier.readLine();
        ligne = fichier.readLine();

        while (!ligne.isEmpty()) {
            QStringList liste = ligne.split(",");

            double agressiviteMin = liste.at(0).split("-").at(0).toDouble();
            double agressiviteMax = liste.at(0).split("-").at(1).toDouble();

            if (scenario->getProfilDeduitGlobal().getAgressivite() >= agressiviteMin && scenario->getProfilDeduitGlobal().getAgressivite() <= agressiviteMax) {
                double rationaliteMin = liste.at(1).split("-").at(0).toDouble();
                double rationaliteMax = liste.at(1).split("-").at(1).toDouble();

                if (scenario->getProfilDeduitGlobal().getRationalite() >= rationaliteMin && scenario->getProfilDeduitGlobal().getRationalite() <= rationaliteMax) {

                    Profil nouveauProfil;

                    if (liste.at(2).split("-").size() == 1) {
                        nouveauProfil.setAgressivite(liste.at(2).toDouble());
                    }
                    else {
                        nouveauProfil.setAgressivite(calculValeurProportionnelle(agressiviteMin, scenario->getProfilDeduitGlobal().getAgressivite(), agressiviteMax,
                                                                                 liste.at(2).split("-").at(0).toDouble(), liste.at(2).split("-").at(1).toDouble()));
                    }

                    if (liste.at(3).split("-").size() == 1) {
                        nouveauProfil.setRationalite(liste.at(3).toDouble());
                    }
                    else {
                        nouveauProfil.setRationalite(calculValeurProportionnelle(rationaliteMin, scenario->getProfilDeduitGlobal().getRationalite(), rationaliteMax,
                                                                                 liste.at(3).split("-").at(0).toDouble(), liste.at(3).split("-").at(1).toDouble()));
                    }

                    setCalibrage(nouveauProfil);

                    fichier.close();
                    return;
                }
            }

            ligne = fichier.readLine();
        }

        fichier.close();
    }
}

void IntelligenceArtificielleProfilage::ecritureAnalyseDesGains() {

    if (ANALYSE_GAINS_PARTIES) {

        Profil calibrageRecherche;

        if (!jeu->getJoueur(0)->estHumain()) {
            IntelligenceArtificielle *iaProfilee = static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0));
            calibrageRecherche = *(iaProfilee->getCalibrage());
        }

        /////////// On cherche le palier ///////////

        QFile fichierPaliers(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE)+"jeu_IA.csv");
        if (!fichierPaliers.open(QIODevice::ReadOnly)) {
            std::cerr << "Erreur lors de l'ouverture du fichier jeu_IA.csv" << std::endl;
            return;
        }

        double agressiviteMin, agressiviteMax, rationaliteMin, rationaliteMax;

        QString ligne = fichierPaliers.readLine();
        ligne = fichierPaliers.readLine();

        bool palierTrouve = false;

        while (!ligne.isEmpty() && !palierTrouve) {
            QStringList liste = ligne.split(",");

            agressiviteMin = liste.at(0).split("-").at(0).toDouble();
            agressiviteMax = liste.at(0).split("-").at(1).toDouble();

            if (calibrageRecherche.getAgressivite() >= agressiviteMin && calibrageRecherche.getAgressivite() <= agressiviteMax) {
                rationaliteMin = liste.at(1).split("-").at(0).toDouble();
                rationaliteMax = liste.at(1).split("-").at(1).toDouble();

                if (calibrageRecherche.getRationalite() >= rationaliteMin && calibrageRecherche.getRationalite() <= rationaliteMax) {
                    palierTrouve = true;
                }
            }

            ligne = fichierPaliers.readLine();
        }

        fichierPaliers.close();


        if (!palierTrouve) {
            std::cerr << "Problème : le palier n'a pas été trouvé dans le fichier !" << std::endl;
            return;
        }


        // On ouvre le fichier du palier correspondant
        QString nomFichier = QString::number(agressiviteMin) + "-" + QString::number(agressiviteMax)
                               + "_" + QString::number(rationaliteMin) + "-" + QString::number(rationaliteMax) + ".csv";
        QFile fichier(QString::fromStdString(DOSSIER_ANALYSE_GAINS) + nomFichier);
        if (!fichier.open(QIODevice::Append | QIODevice::Text)) {
            std::cerr << "Erreur lors de l'ouverture du fichier d'analyse de gains" << std::endl;
            return;
        }

        QTextStream out(&fichier);

        if (fichier.size() == 0) {
            out << "Calibrage IA profilee,AG deduite globale,Ra deduite Globale,Taux similarite,NbParties gagnees scenarios,"
                << "Argent gagne scenarios,Nb parties gagnees apres scenarios,Argent gagne apres scenarios" << endl;
        }



        double tauxPartiesGagneesProfilage = profilage->nbPartiesGagneesProfilage * 100.0 / profilage->nbPartiesProfilage;
        double tauxPartiesGagneesJeu = profilage->nbPartiesGagneesJeu * 100.0 / profilage->nbPartiesJeu;

        out << calibrageRecherche.getAgressivite() << "-" << calibrageRecherche.getRationalite() << "," << scenario->getProfilDeduitGlobal().getAgressivite()
            << "," << scenario->getProfilDeduitGlobal().getRationalite() << "," << scenario->getTauxSimilarite() << ","
            << tauxPartiesGagneesProfilage << "," << profilage->gainsProfilage << "," << tauxPartiesGagneesJeu << ","
            << profilage->gainsJeu << endl;

        fichier.close();
    }
}
