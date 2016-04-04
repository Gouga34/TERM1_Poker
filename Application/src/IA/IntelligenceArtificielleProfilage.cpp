/*========================================================================
Nom: IntelligenceArtificielleProfilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 07/04/2015            Creation: 07/04/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe IntelligenceArtificielleProfilage.
=========================================================================*/


#include "../../include/IA/IntelligenceArtificielleProfilage.h"
#include "../../include/Interface/Logger.h"
#include <QStringList>
#include <QTextStream>


IntelligenceArtificielleProfilage::IntelligenceArtificielleProfilage(bool estDealer, int jetons, int position)
    : ArtificialIntelligence(estDealer, jetons, position), phaseJeu(PHASE_PROFILAGE), profilage(0), scenario(0)
{

}

IntelligenceArtificielleProfilage::IntelligenceArtificielleProfilage(const ArtificialIntelligence& joueur)
    : ArtificialIntelligence(joueur), profilage(0), scenario(0)
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

profiling::TestScenarios IntelligenceArtificielleProfilage::getScenario() const{
    return *scenario;
}

profiling::Profiling* IntelligenceArtificielleProfilage::getProfilage() const {
    return profilage;
}

void IntelligenceArtificielleProfilage::setPseudoJoueurProfile(std::string pseudo) {
    profilJoueur.setPseudo(pseudo);

    if (profilage) {
        delete profilage;
    }
    profilage = new profiling::Profiling(m_resolver->getCalibrage(), &profilJoueur);
    profilage->m_currentProfilingPhase.newPhase(jeu->getOptions().nombrePartiesProfilage);

    profiling::Profile calibrageRecherche;
    if (!jeu->getJoueur(0)->isHumain()) {
        ArtificialIntelligence *iaProfilee = static_cast<ArtificialIntelligence*>(jeu->getJoueur(0));
        calibrageRecherche = *(iaProfilee->getCalibration());
    }

    if (scenario) {
        delete scenario;
    }
    scenario = new profiling::TestScenarios(&profilJoueur, m_resolver->getCalibrage(), calibrageRecherche);
}

void IntelligenceArtificielleProfilage::remplissageDonneesProfilage() {

    int nbTotalActions = 0;

    for(int i = 0; i<3; i++){
        nbTotalActions += jeu->getJoueur(0)->getCompteurActions()[i];
    }

    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].adversaryWinningProbabilities = jeu->getJoueur(0)->getChancesGain();

    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate += jeu->getJoueur(0)->getCompteurActions()[0];
    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate += jeu->getJoueur(0)->getCompteurActions()[1];
    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate += jeu->getJoueur(0)->getCompteurActions()[2];

    if (jeu->getJoueur(0)->getMisePlusHaute() > profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet) {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet = jeu->getJoueur(0)->getMisePlusHaute();
    }

    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet = jeu->getJoueur(0)->getMiseTotale();

    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].isFolded = jeu->estCouche(0);

    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].numberOfActions += nbTotalActions;
}


void IntelligenceArtificielleProfilage::calculProfilGlobalJoueur() {

    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].adversaryWinningProbabilities = jeu->getJoueur(0)->getChancesGain();

    double resultat;

    resultat = profiling::CalculateProfilingData::taux(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate,profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].numberOfActions);
    if (resultat == -1) {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate = 0;
    }
    else {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate = resultat;
    }

    resultat = profiling::CalculateProfilingData::taux(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate,profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].numberOfActions);
    if (resultat == -1) {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate = 0;
    }
    else {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate = resultat;
    }

    resultat = profiling::CalculateProfilingData::taux(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate,profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].numberOfActions);
    if (resultat == -1) {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate = 0;
    }
    else {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate = resultat;
    }

    resultat = profiling::CalculateProfilingData::taux(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet,getCaveDeDepart());
    if (resultat == -1) {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet = 0;
    }
    else {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet = resultat;
    }

    resultat = profiling::CalculateProfilingData::taux(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet,getCaveDeDepart());
    if (resultat == -1) {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet = 0;
    }
    else {
        profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet = resultat;
    }

    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].aggressivenessRate = profiling::CalculateProfilingData::aggressiveness(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].highestBet,profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].betRate,profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet);
    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].rationalityRate = profiling::CalculateProfilingData::rationality(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].adversaryWinningProbabilities,profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].totalBet);
    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].passivityRate = profiling::CalculateProfilingData::passivity( profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].callRate, profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].checkRate);
    profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].bluffRate = profiling::CalculateProfilingData::bluff(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].rationalityRate);


    profilage->m_playerProfile->setAggressiveness(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].aggressivenessRate);
    profilage->m_playerProfile->setRationality(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].rationalityRate);
    profilage->m_playerProfile->setPassivity(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].passivityRate);
    profilage->m_playerProfile->setBluff(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].bluffRate);

    RESULTAT_PARTIE resultatPartie = getJeu()->getResultatPartie();
    if(resultatPartie==GAGNE){
        profilage->m_numberTokensWonProfilingAI=getCave()-getCaveDeDepart();
        resultatPartie=PERDU;
    }
    else if(resultatPartie==PERDU){
        resultatPartie=GAGNE;
        profilage->m_numberTokensWonProfilingAI=getCave()-getCaveDeDepart();
    }
    else{
        profilage->m_numberTokensWonProfilingAI=0;
    }

    profilage->m_roughPlay = m_resolver->estAgressif();
    profilage->m_testScenario = (phaseJeu == PHASE_PROFILAGE);
    profilage->m_gameResultProfilingAIviewpoint=resultatPartie;

    if (phaseJeu == PHASE_PROFILAGE) {
        profilage->m_currentProfilingPhase.finishedPart(profilage->m_numberTokensWonProfilingAI);
    }
    else {
        profilage->m_currentGamePhase.finishedPart(profilage->m_numberTokensWonProfilingAI);
    }

    profilage->save();
}

void IntelligenceArtificielleProfilage::ecritureScenariosDeTests() {
    scenario->setWinningChances(profilage->m_stepParts[ETAPE_JEU::NB_ETAPES].adversaryWinningProbabilities);
    scenario->saveGame();
}

void IntelligenceArtificielleProfilage::ecritureResultatsPartie() {
    calculProfilGlobalJoueur();

    // Si on profile le joueur
    if (phaseJeu == PHASE_PROFILAGE) {
        ecritureScenariosDeTests();
    }
}

double IntelligenceArtificielleProfilage::calculValeurProportionnelle(double minVal1, double val1, double maxVal1, double minVal2, double maxVal2) const {
    return ((val1 - minVal1) * ((maxVal2 - minVal2) / (maxVal1 - minVal1)) + minVal2);
}

PHASE_JEU_IA IntelligenceArtificielleProfilage::prochainTypeDeJeu() {

    if (profilage->m_currentProfilingPhase.getNbPartsDone() < profilage->m_currentProfilingPhase.getNbMinParts()) {
        return PHASE_PROFILAGE;
    }
    else if (profilage->m_currentGamePhase.getNbPartsDone() < profilage->m_currentGamePhase.getNbMinParts()) {
        return PHASE_GAINS;
    }
    else {
        if (phaseJeu == PHASE_PROFILAGE) {
            profilage->m_currentGamePhase.newPhase(jeu->getOptions().nombrePartiesGains);
            return PHASE_GAINS;
        }
        else {
            double gainParPartieJeu = static_cast<double>(profilage->m_currentGamePhase.getGains()) / profilage->m_currentGamePhase.getNbPartsDone();
            double gainParPartieProfilage = static_cast<double>(profilage->m_currentProfilingPhase.getGains()) / profilage->m_currentProfilingPhase.getNbPartsDone();

            if (profilage->m_currentGamePhase.getGains() < 0 || gainParPartieJeu < gainParPartieProfilage) {
                profilage->m_currentProfilingPhase.newPhase(jeu->getOptions().nombrePartiesReprofilage);
                return PHASE_PROFILAGE;
            }
            else {
                return PHASE_GAINS;
            }
        }
    }
}

void IntelligenceArtificielleProfilage::determinerTypeDeJeu() {

    // Si on recherche calibrage optimal
    if (!jeu->getOptions().profilage) {
        phaseJeu = PHASE_GAINS;
    }
    else {
        phaseJeu = prochainTypeDeJeu();
        m_resolver->setJeuAgressif(false);

        if (phaseJeu == PHASE_PROFILAGE) {
            setCalibragePourProfiler();
        }
        else {
            if (UTILISATION_DELTA_AGRESSIVITE && chancesGain >= 50 && chancesGain <= 70) {
                // Aléatoirement, on augmente l'agressivité
                m_resolver->setJeuAgressif((rand() % DELTA_AGRESSIVITE) == 0);
            }

            setCalibragePourJouer();
        }

        Logger::getInstance()->ajoutLogs("Calibrage IA profilage: agressivité: "+QString::number(m_resolver->getCalibrage()->getAggressiveness())
                                         +" rationalité: "+QString::number(m_resolver->getCalibrage()->getRationality()));
    }

    getProfilage()->reset();
}

void IntelligenceArtificielleProfilage::setCalibragePourProfiler() {

    if (jeu->getOptions().calibrageIaQuiProfileFixe) {
        return;
    }

    //On tire aléatoirement un nouveau taux d'agressivite:
    int agressivite=rand()%100+1;

    m_resolver->getCalibrage()->setAggressiveness(agressivite);
    m_resolver->getCalibrage()->setRationality(RATIONALITE_IA_PROFILAGE);
}

void IntelligenceArtificielleProfilage::setCalibragePourJouer() {

    if (jeu->getOptions().calibrageIaQuiProfileFixe) {
        return;
    }

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

        if (scenario->getGlobalDeductedProfile().getAggressiveness() >= agressiviteMin && scenario->getGlobalDeductedProfile().getAggressiveness() <= agressiviteMax) {
            double rationaliteMin = liste.at(1).split("-").at(0).toDouble();
            double rationaliteMax = liste.at(1).split("-").at(1).toDouble();

            if (scenario->getGlobalDeductedProfile().getRationality() >= rationaliteMin && scenario->getGlobalDeductedProfile().getRationality() <= rationaliteMax) {

                profiling::Profile nouveauProfil;

                if (liste.at(2).split("-").size() == 1) {
                    nouveauProfil.setAggressiveness(liste.at(2).toDouble());
                }
                else {
                    nouveauProfil.setAggressiveness(calculValeurProportionnelle(agressiviteMin, scenario->getGlobalDeductedProfile().getAggressiveness(), agressiviteMax,
                                                                             liste.at(2).split("-").at(0).toDouble(), liste.at(2).split("-").at(1).toDouble()));
                }

                if (liste.at(3).split("-").size() == 1) {
                    nouveauProfil.setRationality(liste.at(3).toDouble());
                }
                else {
                    nouveauProfil.setRationality(calculValeurProportionnelle(rationaliteMin, scenario->getGlobalDeductedProfile().getRationality(), rationaliteMax,
                                                                             liste.at(3).split("-").at(0).toDouble(), liste.at(3).split("-").at(1).toDouble()));
                }

                setCalibration(nouveauProfil);

                fichier.close();
                return;
            }
        }

        ligne = fichier.readLine();
    }

    fichier.close();
}

void IntelligenceArtificielleProfilage::ecritureAnalyseDesGains() {

    if (jeu->getOptions().analyseGainsParties) {

        profiling::Profile calibrageRecherche;

        if (!jeu->getJoueur(0)->isHumain()) {
            ArtificialIntelligence *iaProfilee = static_cast<ArtificialIntelligence*>(jeu->getJoueur(0));
            calibrageRecherche = *(iaProfilee->getCalibration());
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

            if (calibrageRecherche.getAggressiveness() >= agressiviteMin && calibrageRecherche.getAggressiveness() <= agressiviteMax) {
                rationaliteMin = liste.at(1).split("-").at(0).toDouble();
                rationaliteMax = liste.at(1).split("-").at(1).toDouble();

                if (calibrageRecherche.getRationality() >= rationaliteMin && calibrageRecherche.getRationality() <= rationaliteMax) {
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



        double tauxPartiesGagneesProfilage = profiling::CalculateProfilingData::taux(profilage->m_numberProfilingGamesWon, profilage->m_numberProfilingGames);
        double tauxPartiesGagneesJeu = profiling::CalculateProfilingData::taux(profilage->m_numberGameGamesWon, profilage->m_numberGamesGame);

        double tauxGainsProfilage = 0;
        double tauxGainsjeu=0;

        if (profilage->m_numberProfilingGames != 0) {
            tauxGainsProfilage = profilage->m_profilingEarnings
 / profilage->m_numberProfilingGames;
        }

        if (profilage->m_numberGamesGame != 0) {
            tauxGainsjeu=profilage->m_gameEarnings /  profilage->m_numberGamesGame;
        }

        out << calibrageRecherche.getAggressiveness() << "-" << calibrageRecherche.getRationality() << "," << scenario->getGlobalDeductedProfile().getAggressiveness()
            << "," << scenario->getGlobalDeductedProfile().getRationality() << "," << scenario->getSimilarityDegree() << ","
            << tauxPartiesGagneesProfilage << "," << tauxGainsProfilage << "," << tauxPartiesGagneesJeu << ","
            << tauxGainsjeu << endl;

        fichier.close();
    }
}
