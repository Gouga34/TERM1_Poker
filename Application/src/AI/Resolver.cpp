/*========================================================================
Nom: Resolveur.cpp       Auteur: Morgane Vidal
Maj: 31/03/2015          Creation: 31/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les corps des méthodes
                                        la classe Resolveur.
=========================================================================*/


#include "../../include/AI/Resolver.h"
#include "../../include/Constantes.h"
#include "../../include/Profiling/CalculateProfilingData.h"

#include <vector>

namespace ai {

    Resolver::Resolver(ArtificialIntelligence *ai)
        : m_roughtPlay(false), m_ai(ai) {
        m_calibration = new profiling::Profile;
    }


    Resolver::~Resolver() {
        delete m_calibration;
    }

    bool Resolver::isAggressive() const {
        return m_roughtPlay;
    }

    void Resolver::setRoughtGame(bool roughtPlay) {
        m_roughtPlay = roughtPlay;
    }

    void Resolver::setCalibration(profiling::Profile profile) {
        m_calibration->setAggressiveness(profile.getAggressiveness());
        m_calibration->setRationality(profile.getRationality());
    }

    profiling::Profile* Resolver::getCalibration() {
        return m_calibration;
    }


    Action Resolver::calculateRoughtAction() {
        calculateAggressivenessTheoreticalTotalBet();

        ACTION action = PAS_ENCORE_D_ACTION;
        int tokensToBet = -1; //mise à effectuer s'il y en a une. Correspond au nombre de jetons

        double roundTheoreticalBet = m_ai->getCumulMisesEtRelances() * HAUSSE_MISES_AGRESSIVITE - m_ai->getCumulMisesEtRelances();
        //Si le total des mises de l'IA est supérieur à mth, on checke ou on se couche.
        //Sinon, on mise/relance/suit
        if (roundTheoreticalBet > m_aggressivenessTheoreticalTotalBet - m_ai->getCumulMisesEtRelances()
                || m_ai->getJeu()->getEtape() != RIVER) {
            roundTheoreticalBet = m_aggressivenessTheoreticalTotalBet - m_ai->getCumulMisesEtRelances();
        }
        //Si on peut miser
        if (m_ai->getJeu()->peutMiser(m_ai->getPosition(),1)) {
            // Si on a l'argent
            if (m_ai->getJeu()->peutMiser(m_ai->getPosition(),roundTheoreticalBet)) {
                action = MISER;
                tokensToBet = roundTheoreticalBet;
            }
            else {
                action = TAPIS;
            }
        }
        //Si le joueur adverse a misé ou relancé
        else if (m_ai->getJeu()->getLastAction(m_ai->getJeu()->getPositionJoueurAdverse(m_ai->getPosition())) == MISER
                    || m_ai->getJeu()->getLastAction(m_ai->getJeu()->getPositionJoueurAdverse(m_ai->getPosition())) == RELANCER
                    || m_ai->getJeu()->getLastAction(m_ai->getJeu()->getPositionJoueurAdverse(m_ai->getPosition())) == GROSSE_BLIND
                    || m_ai->getJeu()->getLastAction(m_ai->getJeu()->getPositionJoueurAdverse(m_ai->getPosition())) == TAPIS) {

            double variationAutoriseeMiseAdversaire = (VARIATION_AUTORISEE / 100) * m_ai->getJeu()->getMiseCourante();

            //Si la mise courante est inférieure à ce qu'il reste à miser +10% de la mise
            if (m_ai->getJeu()->getMiseCourante() <= m_aggressivenessTheoreticalTotalBet - m_ai->getCumulMisesEtRelances() + variationAutoriseeMiseAdversaire) {
                //On continue à jouer
                //Si la mise théorique est inférieure à la relance minimum (-10%), on suit
                if (roundTheoreticalBet < 2 * m_ai->getJeu()->getMiseCourante() - variationAutoriseeMiseAdversaire) {
                    action = SUIVRE;
                }
                else {   // On relance
                    //Si on peut relancer
                    if (m_ai->getJeu()->peutRelancer(m_ai->getPosition(), 2 * m_ai->getJeu()->getMiseCourante())) {
                        if (roundTheoreticalBet < 2 * m_ai->getJeu()->getMiseCourante()) {
                            tokensToBet = 2 * m_ai->getJeu()->getMiseCourante();
                        }
                        else {
                            if (m_ai->getJeu()->peutRelancer(m_ai->getPosition(), roundTheoreticalBet)) {
                                tokensToBet = roundTheoreticalBet;
                            }
                            else {
                                tokensToBet = m_ai->getCave();
                            }
                        }
                        action = RELANCER;
                    }
                    else {
                        if (m_ai->getJeu()->peutSuivre(m_ai->getPosition())) {
                            action = SUIVRE;
                        }
                    }
                }
            }
            else {
                if (m_ai->getJeu()->peutChecker(m_ai->getPosition())) {
                    action = CHECKER;
                }
                else {
                    action = SE_COUCHER;
                }
            }
        }
        else if (m_ai->getJeu()->peutChecker(m_ai->getPosition())) {
            action = CHECKER;
        }

        if (action == PAS_ENCORE_D_ACTION) {
            std::cout << "pas encore d'action" << std::endl;
        }

        return Action(action,tokensToBet);
    }

    void Resolver::calculateAggressivenessTheoreticalTotalBet() {
        double rateTotalBet = 0.0;

        double levelSuperior, levelInferior;
        double theoreticalBetInferior, theoreticalBetSuperior;

        if (m_calibration->getAggressiveness() < AGRESSIVITE::PALIER1::FIN_AG_THEORIQUE) {
            levelInferior = AGRESSIVITE::PALIER1::DEBUT_AG_THEORIQUE;
            levelSuperior = AGRESSIVITE::PALIER1::FIN_AG_THEORIQUE;
            theoreticalBetInferior = AGRESSIVITE::PALIER1::DEBUT_MISE_TOTALE;
            theoreticalBetSuperior = AGRESSIVITE::PALIER1::FIN_MISE_TOTALE;
        }
        else if (m_calibration->getAggressiveness() < AGRESSIVITE::PALIER2::FIN_AG_THEORIQUE) {
            levelInferior = AGRESSIVITE::PALIER2::DEBUT_AG_THEORIQUE;
            levelSuperior = AGRESSIVITE::PALIER2::FIN_AG_THEORIQUE;
            theoreticalBetInferior = AGRESSIVITE::PALIER2::DEBUT_MISE_TOTALE;
            theoreticalBetSuperior = AGRESSIVITE::PALIER2::FIN_MISE_TOTALE;
        }
        else if (m_calibration->getAggressiveness() < AGRESSIVITE::PALIER3::FIN_AG_THEORIQUE) {
            levelInferior = AGRESSIVITE::PALIER3::DEBUT_AG_THEORIQUE;
            levelSuperior = AGRESSIVITE::PALIER3::FIN_AG_THEORIQUE;
            theoreticalBetInferior = AGRESSIVITE::PALIER3::DEBUT_MISE_TOTALE;
            theoreticalBetSuperior = AGRESSIVITE::PALIER3::FIN_MISE_TOTALE;
        }
        else {
            rateTotalBet = AGRESSIVITE::PALIER4::MISE_TOTALE;
        }

        if (rateTotalBet == 0.0) {
            rateTotalBet = ((m_calibration->getAggressiveness() - levelInferior) * ((theoreticalBetSuperior - theoreticalBetInferior) / (levelSuperior - levelInferior))) + theoreticalBetInferior;
        }
        m_aggressivenessTheoreticalTotalBet = (rateTotalBet / 100) * m_ai->getCaveDeDepart();
    }


    Action Resolver::calculateRationalAction() {
        ACTION action;
        int tokensToBet =-1; //-1 si pas une action avec une mise

        //On prend un nombre aléatoire
        int randomNumber = rand() % 101;
        if (randomNumber < (m_calibration->getRationality()-1)) { //Si random E [0 - (rationalite-1)]

            int theoreticalBetTokens = calculateRationalityBet(ACTION::MISER);

            //Si on peut pas checker et peut de chances de gain :
            if (m_ai->getChancesGain() < 30.0
                    && !(m_ai->getJeu()->peutChecker(m_ai->getPosition()))) {
                action = ACTION::SE_COUCHER;
            }
            else if (m_ai->getCumulMisesEtRelances() < theoreticalBetTokens) {
                int tokensToBetRationality = theoreticalBetTokens - m_ai->getCumulMisesEtRelances();

                if (m_ai->getJeu()->peutMiser(m_ai->getPosition(),tokensToBetRationality)) { //Si on peut miser
                    action = ACTION::MISER;
                    tokensToBet = tokensToBetRationality;
                }

                //Si on peut Relancer, on relance
                else if (m_ai->getJeu()->peutRelancer(m_ai->getPosition(),calculateRationalityBet(ACTION::RELANCER))) {
                    action = ACTION::RELANCER;
                    tokensToBet = calculateRationalityBet(action);
                }
                //Si on peut suivre, on suit
                else if (m_ai->getJeu()->peutSuivre(m_ai->getPosition())) {
                    action = ACTION::SUIVRE;
                }
                //Si on peut checker on checke
                else if (m_ai->getJeu()->peutChecker(m_ai->getPosition())) {
                    action = ACTION::CHECKER;
                }
            }
            else if (m_ai->getJeu()->peutChecker(m_ai->getPosition())) {
                action = ACTION::CHECKER;
            }
            else if (m_ai->getJeu()->peutSuivre(m_ai->getPosition())) {
                action = ACTION::SUIVRE;
            }
        }
        else{ //random E [rationalite- 100]
            std::vector<ACTION> actionsList;

         //Construction de la liste des actions possibles :
            if (m_ai->getJeu()->peutMiser(m_ai->getPosition(), 1)) {
                //Miser plus que la mise théroque:
                actionsList.push_back(GROSSE_BLIND); //En fait c'est miserplus que théorique mais

                //Miser moins que la mise théorique:
                actionsList.push_back(PETITE_BLIND);
            }
            //se coucher
            if (m_ai->getChancesGain() > 70) {
                actionsList.push_back(ACTION::SE_COUCHER);
            }
            else if (m_ai->getChancesGain() <= 70) { //Relancer et/ou Suivre
                if (m_ai->getJeu()->peutRelancer(m_ai->getPosition(),2 * m_ai->getJeu()->getMiseCourante())) {
                    actionsList.push_back(ACTION::RELANCER);
                }
                else if (m_ai->getJeu()->peutSuivre(m_ai->getPosition())) {
                    actionsList.push_back(ACTION::SUIVRE);
                }
                else if (m_ai->getJeu()->peutChecker(m_ai->getPosition())) {
                    actionsList.push_back(ACTION::CHECKER);
                }
            }
            else { //Checker
                if (m_ai->getJeu()->peutChecker(m_ai->getPosition())) {
                    actionsList.push_back(ACTION::CHECKER);
                }
            }

            //Choix aléatoire d'une des actions de la liste :
            int randomNumber=rand() % actionsList.size();

            double theoreticalBet = profiling::CalculateProfilingData::theoreticalBet(m_ai->getChancesGain());
            int theoreticalTokensToBet = (theoreticalBet * m_ai->getCave()) / 100;

            if (actionsList.at(randomNumber) == GROSSE_BLIND) {
                action = ACTION::MISER;

                //borneInférieure=10% de la cave + la mise théorique
                int lowerBound = theoreticalTokensToBet + ((m_ai->getCave() * 10) / 100);

                if (lowerBound < m_ai->getCave()) {
                    //On prend un nombre aléatoire entre borneInferieure et ia->getCave();
                    tokensToBet = (rand() % (m_ai->getCave() - lowerBound + 1)) + lowerBound;
                }
                else{
                    tokensToBet = m_ai->getCave();
                }
            }
            else if (actionsList.at(randomNumber) == PETITE_BLIND) {
                action = ACTION::MISER;

                //borneSuperieure=mise théorique -10% de la cave
                int upperBound = theoreticalTokensToBet - ((m_ai->getCave() * 10) / 100);

                if (upperBound > 0) {
                    tokensToBet = (rand() % upperBound) + 1;
                }
                else {
                    tokensToBet = m_ai->getCave();
                }
            }
            else {
                action = actionsList.at(randomNumber);

                if (action == ACTION::RELANCER) {
                    tokensToBet = 2 * m_ai->getJeu()->getMiseCourante();
                }
                else if (action == ACTION::SUIVRE) {
                    tokensToBet = m_ai->getJeu()->getMiseCourante();
                }
            }
        }

        return Action(action,tokensToBet);
    }


    int Resolver::calculateRationalityBet(ACTION action) {
        //On récupère la mise théorique si le joueur est 100% agressif
        double theoreticalBet = profiling::CalculateProfilingData::theoreticalBet(m_ai->getChancesGain());

        int tokensToBet = (theoreticalBet * m_ai->getCave()) / 100;

        if (action == ACTION::CHECKER) {
            tokensToBet = -1;
        }
        else if (action == ACTION::SUIVRE) {
            tokensToBet = m_ai->getJeu()->getMiseCourante();
        }
        else if (action == ACTION::RELANCER) {
            //Si on peut relancer
            if (tokensToBet >= 2 * m_ai->getJeu()->getMiseCourante()) {
                tokensToBet = 2 * m_ai->getJeu()->getMiseCourante();
            }
            else {
                tokensToBet = 0;
            }
        }

        return tokensToBet;
    }


    Action Resolver::calculateAction() {
        Action roughtAction = calculateRoughtAction();
        Action rationalAction = calculateRationalAction();

        bool randomSelection = false;
        ACTION action = PAS_ENCORE_D_ACTION;
        int tokensToBet = -1;

        if (m_roughtPlay) {
            action = roughtAction.getAction();
            tokensToBet = roughtAction.getMontant();
        }
        // Fusion des deux résultats :Si les actions ne sont pas les mêmes, on choisit une des deux actions:
        else if (roughtAction.getAction() != rationalAction.getAction()) {
            //CHECKER et SE_COUCHER
            if ((roughtAction.getAction() == CHECKER
                    && rationalAction.getAction() == SE_COUCHER)
                 ||(roughtAction.getAction() == SE_COUCHER
                    && rationalAction.getAction()==CHECKER)) {
                action = CHECKER;
            }
            //CHECKER et SUIVRE
            if ((roughtAction.getAction() == CHECKER
                    && rationalAction.getAction() == SUIVRE)
                 ||(roughtAction.getAction() == SUIVRE
                    && rationalAction.getAction() == CHECKER)) {
                randomSelection = true;
            }
            //CHECKER et MISER
            if ((roughtAction.getAction() == CHECKER
                    && rationalAction.getAction() == MISER)
                 ||(roughtAction.getAction() == MISER
                    && rationalAction.getAction() == CHECKER)) {

                if (roughtAction.getAction() == CHECKER) {
                    roughtAction.setAction(MISER);
                    roughtAction.setMontant(m_ai->getJeu()->getBlind());
                }
                else {
                    rationalAction.setAction(MISER);
                    rationalAction.setMontant(m_ai->getJeu()->getBlind());
                }
            }
            //SUIVRE et RELANCER
            if ((roughtAction.getAction() == SUIVRE
                    && rationalAction.getAction() == RELANCER)
                  || (roughtAction.getAction() == RELANCER
                     && rationalAction.getAction() == SUIVRE)) {

                if (roughtAction.getAction() == RELANCER) {
                    if (roughtAction.getMontant() > m_ai->getJeu()->getMiseCourante() * 2) {
                        tokensToBet = m_ai->getJeu()->getMiseCourante() * 2;
                        action = RELANCER;
                    }
                    else {
                        action = SUIVRE;
                    }
                }
                else {
                    if (rationalAction.getMontant() > m_ai->getJeu()->getMiseCourante() * 2) {
                        tokensToBet = m_ai->getJeu()->getMiseCourante() * 2;
                        action = RELANCER;
                    }
                    else {
                        action = SUIVRE;
                    }
                }
            }
            //SUIVRE et SE_COUCHER
            if ((roughtAction.getAction() == SUIVRE
                    && rationalAction.getAction() == SE_COUCHER)
                 || (roughtAction.getAction() == SE_COUCHER
                    && rationalAction.getAction() == SUIVRE)) {
                randomSelection = true;
            }
            //RELANCER et SE_COUCHER
            if ((roughtAction.getAction() == SE_COUCHER
                    && rationalAction.getAction() == RELANCER)
                 || (roughtAction.getAction() == RELANCER
                    && rationalAction.getAction() == SE_COUCHER)) {
                action = SUIVRE;
            }
            //MISER et SE_COUCHER
            if ((roughtAction.getAction() == MISER
                    && rationalAction.getAction() == SE_COUCHER)
                 || (roughtAction.getAction() == SE_COUCHER
                    && rationalAction.getAction() == MISER)) {

                if (roughtAction.getAction() == SE_COUCHER) {
                    roughtAction.setAction(MISER);
                    rationalAction.setMontant(rationalAction.getMontant() / 2);
                }
                else {
                    rationalAction.setAction(MISER);
                    roughtAction.setMontant(roughtAction.getMontant() / 2);
                }
            }
            //TAPIS et MISER
            if ((roughtAction.getAction() == TAPIS
                    && rationalAction.getAction() == MISER)
                 || (roughtAction.getAction() == MISER
                     && rationalAction.getAction() == TAPIS)) {

                if (roughtAction.getAction() == TAPIS) {
                    roughtAction.setAction(MISER);
                    roughtAction.setMontant(m_ai->getCave());
                } else {
                    rationalAction.setAction(MISER);
                    rationalAction.setMontant(m_ai->getCave());
                }
            }
            //TAPIS et RELANCER
            if ((roughtAction.getAction() == TAPIS
                    && rationalAction.getAction() == RELANCER)
                 || (roughtAction.getAction() == RELANCER
                     && rationalAction.getAction() == TAPIS)) {

                if (roughtAction.getAction() == TAPIS) {
                    roughtAction.setAction(RELANCER);
                    roughtAction.setMontant(m_ai->getCave());
                }
                else {
                    rationalAction.setAction(RELANCER);
                    rationalAction.setMontant(m_ai->getCave());
                }
            }
            //TAPIS et SE_COUCHER
            if ((roughtAction.getAction() == TAPIS
                    && rationalAction.getAction() == SE_COUCHER)
                || (roughtAction.getAction() == SE_COUCHER
                    && rationalAction.getAction() == TAPIS)) {

                int tokens = (m_ai->getCave() == 1) ? 1 : m_ai->getCave() / 2;

                if (m_ai->getJeu()->peutSuivre(m_ai->getPosition())) {
                    action = SUIVRE;
                }
                else if (m_ai->getJeu()->peutMiser(m_ai->getPosition(),tokens)) {
                    action = MISER;
                    tokensToBet = tokens;
                }
            }
            //TAPIS et CHECKER
            if ((roughtAction.getAction() == TAPIS
                    && rationalAction.getAction() == CHECKER)
                 || (roughtAction.getAction() == CHECKER
                     && rationalAction.getAction() == TAPIS)) {
                if (m_ai->getJeu()->peutSuivre(m_ai->getPosition())) {
                    action = SUIVRE;
                }
                else {
                    int tokens = (m_ai->getCave() == 1) ? 1 : m_ai->getCave() / 2;

                    if (m_ai->getJeu()->peutMiser(m_ai->getPosition(),tokens)) {
                        action = MISER;
                        tokensToBet = tokens;
                    }
                }
            }

            if (randomSelection) {
                int total = m_calibration->getRationality() + m_calibration->getAggressiveness();

                int randomNumber = rand()%total + 1;

                //Si random E [0-agressivité], on prend l'action et les jetons de l'agressivité
                if (randomNumber < m_calibration->getAggressiveness()) {
                    action = roughtAction.getAction();
                    tokensToBet = roughtAction.getMontant();
                }
                //Sinon, random E[agressivite+1 - total], on prend l'action et les jetons de la rationalité
                else {
                    action = rationalAction.getAction();
                    tokensToBet = rationalAction.getMontant();
                }
            }
       }

       //Sinon, si l'action est relancer ou miser
       if (roughtAction.getAction() == rationalAction.getAction()
               && (roughtAction.getAction() == ACTION::RELANCER
                   || roughtAction.getAction() == ACTION::MISER)) {

            action = roughtAction.getAction();

            int tokensToBetMax, tokensToBetMin, rateMax;

            if (roughtAction.getMontant() > rationalAction.getMontant()) {
                tokensToBetMax = roughtAction.getMontant();
                tokensToBetMin = rationalAction.getMontant();
            }
            else {
                tokensToBetMax = rationalAction.getMontant();
                tokensToBetMin = roughtAction.getMontant();
            }
            if (m_calibration->getAggressiveness() > m_calibration->getRationality()) {
                rateMax = m_calibration->getAggressiveness();
            }
            else {
                rateMax = m_calibration->getRationality();
            }

            //On va prendre une mise comprise entre le min et le max.
            //Celle-ci aura plus de chances d'être plus proche du taux le plus fort.

            int total = m_calibration->getAggressiveness() + m_calibration->getRationality();
            double ratio = static_cast<double>(rateMax) / total;
            tokensToBet = tokensToBetMin + (ratio * abs(tokensToBetMax - tokensToBetMin));

        } else {
           if (action == PAS_ENCORE_D_ACTION) {
                action = roughtAction.getAction();
           }
        }

        if (tokensToBet > m_ai->getCave()) {
            tokensToBet = m_ai->getCave();
        }

        return Action(action,tokensToBet);
    }
}
