/*========================================================================
Nom: Resolveur.cpp       Auteur: Morgane Vidal
Maj: 31/03/2015          Creation: 31/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les corps des méthodes
                                        la classe Resolveur.
=========================================================================*/


#include "../../include/AI/Resolver.h"
#include "../../include/Constants.h"
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


    game::Action Resolver::calculateRoughtAction() {
        calculateAggressivenessTheoreticalTotalBet();

        ACTION action = NO_ACTION;
        int tokensToBet = -1; //mise à effectuer s'il y en a une. Correspond au nombre de jetons

        double roundTheoreticalBet = m_ai->getAccumulatedBetsAndRaises() * INCREASE_AGGRESSIVE_BETS - m_ai->getAccumulatedBetsAndRaises();
        //Si le total des mises de l'IA est supérieur à mth, on checke ou on se couche.
        //Sinon, on mise/relance/suit
        if (roundTheoreticalBet > m_aggressivenessTheoreticalTotalBet - m_ai->getAccumulatedBetsAndRaises()
                || m_ai->getGame()->getStep() != RIVER) {
            roundTheoreticalBet = m_aggressivenessTheoreticalTotalBet - m_ai->getAccumulatedBetsAndRaises();
        }
        //Si on peut miser
        if (m_ai->getGame()->canBet(m_ai->getPosition(),1)) {
            // Si on a l'argent
            if (m_ai->getGame()->canBet(m_ai->getPosition(),roundTheoreticalBet)) {
                action = BET;
                tokensToBet = roundTheoreticalBet;
            }
            else {
                action = ALL_IN;
            }
        }
        //Si le joueur adverse a misé ou relancé
        else if (m_ai->getGame()->getPlayersLastAction(m_ai->getGame()->getOpponentsPosition(m_ai->getPosition())) == BET
                    || m_ai->getGame()->getPlayersLastAction(m_ai->getGame()->getOpponentsPosition(m_ai->getPosition())) == RAISE
                    || m_ai->getGame()->getPlayersLastAction(m_ai->getGame()->getOpponentsPosition(m_ai->getPosition())) == BIG_BLIND
                    || m_ai->getGame()->getPlayersLastAction(m_ai->getGame()->getOpponentsPosition(m_ai->getPosition())) == ALL_IN) {

            double variationAutoriseeMiseAdversaire = (ALLOWED_VARIATION / 100) * m_ai->getGame()->getCurrentBet();

            //Si la mise courante est inférieure à ce qu'il reste à miser +10% de la mise
            if (m_ai->getGame()->getCurrentBet() <= m_aggressivenessTheoreticalTotalBet - m_ai->getAccumulatedBetsAndRaises() + variationAutoriseeMiseAdversaire) {
                //On continue à jouer
                //Si la mise théorique est inférieure à la relance minimum (-10%), on suit
                if (roundTheoreticalBet < 2 * m_ai->getGame()->getCurrentBet() - variationAutoriseeMiseAdversaire) {
                    action = CALL;
                }
                else {   // On relance
                    //Si on peut relancer
                    if (m_ai->getGame()->canRaise(m_ai->getPosition(), 2 * m_ai->getGame()->getCurrentBet())) {
                        if (roundTheoreticalBet < 2 * m_ai->getGame()->getCurrentBet()) {
                            tokensToBet = 2 * m_ai->getGame()->getCurrentBet();
                        }
                        else {
                            if (m_ai->getGame()->canRaise(m_ai->getPosition(), roundTheoreticalBet)) {
                                tokensToBet = roundTheoreticalBet;
                            }
                            else {
                                tokensToBet = m_ai->getCave();
                            }
                        }
                        action = RAISE;
                    }
                    else {
                        if (m_ai->getGame()->canCall(m_ai->getPosition())) {
                            action = CALL;
                        }
                    }
                }
            }
            else {
                if (m_ai->getGame()->canCheck(m_ai->getPosition())) {
                    action = CHECK;
                }
                else {
                    action = FOLD;
                }
            }
        }
        else if (m_ai->getGame()->canCheck(m_ai->getPosition())) {
            action = CHECK;
        }

        if (action == NO_ACTION) {
            std::cout << "pas encore d'action" << std::endl;
        }

        return game::Action(action,tokensToBet);
    }

    void Resolver::calculateAggressivenessTheoreticalTotalBet() {
        double rateTotalBet = 0.0;

        double levelSuperior, levelInferior;
        double theoreticalBetInferior, theoreticalBetSuperior;

        if (m_calibration->getAggressiveness() < AGGRESSIVENESS::PALIER1::FIN_AG_THEORIQUE) {
            levelInferior = AGGRESSIVENESS::PALIER1::DEBUT_AG_THEORIQUE;
            levelSuperior = AGGRESSIVENESS::PALIER1::FIN_AG_THEORIQUE;
            theoreticalBetInferior = AGGRESSIVENESS::PALIER1::DEBUT_MISE_TOTALE;
            theoreticalBetSuperior = AGGRESSIVENESS::PALIER1::FIN_MISE_TOTALE;
        }
        else if (m_calibration->getAggressiveness() < AGGRESSIVENESS::PALIER2::FIN_AG_THEORIQUE) {
            levelInferior = AGGRESSIVENESS::PALIER2::DEBUT_AG_THEORIQUE;
            levelSuperior = AGGRESSIVENESS::PALIER2::FIN_AG_THEORIQUE;
            theoreticalBetInferior = AGGRESSIVENESS::PALIER2::DEBUT_MISE_TOTALE;
            theoreticalBetSuperior = AGGRESSIVENESS::PALIER2::FIN_MISE_TOTALE;
        }
        else if (m_calibration->getAggressiveness() < AGGRESSIVENESS::PALIER3::FIN_AG_THEORIQUE) {
            levelInferior = AGGRESSIVENESS::PALIER3::DEBUT_AG_THEORIQUE;
            levelSuperior = AGGRESSIVENESS::PALIER3::FIN_AG_THEORIQUE;
            theoreticalBetInferior = AGGRESSIVENESS::PALIER3::DEBUT_MISE_TOTALE;
            theoreticalBetSuperior = AGGRESSIVENESS::PALIER3::FIN_MISE_TOTALE;
        }
        else {
            rateTotalBet = AGGRESSIVENESS::PALIER4::MISE_TOTALE;
        }

        if (rateTotalBet == 0.0) {
            rateTotalBet = ((m_calibration->getAggressiveness() - levelInferior) * ((theoreticalBetSuperior - theoreticalBetInferior) / (levelSuperior - levelInferior))) + theoreticalBetInferior;
        }
        m_aggressivenessTheoreticalTotalBet = (rateTotalBet / 100) * m_ai->getStartingCave();
    }


    game::Action Resolver::calculateRationalAction() {
        ACTION action;
        int tokensToBet =-1; //-1 si pas une action avec une mise

        //On prend un nombre aléatoire
        int randomNumber = rand() % 101;
        if (randomNumber < (m_calibration->getRationality()-1)) { //Si random E [0 - (rationalite-1)]

            int theoreticalBetTokens = calculateRationalityBet(ACTION::BET);

            //Si on peut pas checker et peut de chances de gain :
            if (m_ai->getWinningChances() < 30.0
                    && !(m_ai->getGame()->canCheck(m_ai->getPosition()))) {
                action = ACTION::FOLD;
            }
            else if (m_ai->getAccumulatedBetsAndRaises() < theoreticalBetTokens) {
                int tokensToBetRationality = theoreticalBetTokens - m_ai->getAccumulatedBetsAndRaises();

                if (m_ai->getGame()->canBet(m_ai->getPosition(),tokensToBetRationality)) { //Si on peut miser
                    action = ACTION::BET;
                    tokensToBet = tokensToBetRationality;
                }

                //Si on peut Relancer, on relance
                else if (m_ai->getGame()->canRaise(m_ai->getPosition(),calculateRationalityBet(ACTION::RAISE))) {
                    action = ACTION::RAISE;
                    tokensToBet = calculateRationalityBet(action);
                }
                //Si on peut suivre, on suit
                else if (m_ai->getGame()->canCall(m_ai->getPosition())) {
                    action = ACTION::CALL;
                }
                //Si on peut checker on checke
                else if (m_ai->getGame()->canCheck(m_ai->getPosition())) {
                    action = ACTION::CHECK;
                }
            }
            else if (m_ai->getGame()->canCheck(m_ai->getPosition())) {
                action = ACTION::CHECK;
            }
            else if (m_ai->getGame()->canCall(m_ai->getPosition())) {
                action = ACTION::CALL;
            }
        }
        else{ //random E [rationalite- 100]
            std::vector<ACTION> actionsList;

         //Construction de la liste des actions possibles :
            if (m_ai->getGame()->canBet(m_ai->getPosition(), 1)) {
                //Miser plus que la mise théroque:
                actionsList.push_back(BIG_BLIND); //En fait c'est miserplus que théorique mais

                //Miser moins que la mise théorique:
                actionsList.push_back(SMALL_BLIND);
            }
            //se coucher
            if (m_ai->getWinningChances() > 70) {
                actionsList.push_back(ACTION::FOLD);
            }
            else if (m_ai->getWinningChances() <= 70) { //Relancer et/ou Suivre
                if (m_ai->getGame()->canRaise(m_ai->getPosition(),2 * m_ai->getGame()->getCurrentBet())) {
                    actionsList.push_back(ACTION::RAISE);
                }
                else if (m_ai->getGame()->canCall(m_ai->getPosition())) {
                    actionsList.push_back(ACTION::CALL);
                }
                else if (m_ai->getGame()->canCheck(m_ai->getPosition())) {
                    actionsList.push_back(ACTION::CHECK);
                }
            }
            else { //Checker
                if (m_ai->getGame()->canCheck(m_ai->getPosition())) {
                    actionsList.push_back(ACTION::CHECK);
                }
            }

            //Choix aléatoire d'une des actions de la liste :
            int randomNumber=rand() % actionsList.size();

            double theoreticalBet = profiling::CalculateProfilingData::theoreticalBet(m_ai->getWinningChances());
            int theoreticalTokensToBet = (theoreticalBet * m_ai->getCave()) / 100;

            if (actionsList.at(randomNumber) == BIG_BLIND) {
                action = ACTION::BET;

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
            else if (actionsList.at(randomNumber) == SMALL_BLIND) {
                action = ACTION::BET;

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

                if (action == ACTION::RAISE) {
                    tokensToBet = 2 * m_ai->getGame()->getCurrentBet();
                }
                else if (action == ACTION::CALL) {
                    tokensToBet = m_ai->getGame()->getCurrentBet();
                }
            }
        }

        return game::Action(action,tokensToBet);
    }


    int Resolver::calculateRationalityBet(ACTION action) {
        //On récupère la mise théorique si le joueur est 100% agressif
        double theoreticalBet = profiling::CalculateProfilingData::theoreticalBet(m_ai->getWinningChances());

        int tokensToBet = (theoreticalBet * m_ai->getCave()) / 100;

        if (action == ACTION::CHECK) {
            tokensToBet = -1;
        }
        else if (action == ACTION::CALL) {
            tokensToBet = m_ai->getGame()->getCurrentBet();
        }
        else if (action == ACTION::RAISE) {
            //Si on peut relancer
            if (tokensToBet >= 2 * m_ai->getGame()->getCurrentBet()) {
                tokensToBet = 2 * m_ai->getGame()->getCurrentBet();
            }
            else {
                tokensToBet = 0;
            }
        }

        return tokensToBet;
    }


    game::Action Resolver::calculateAction() {
        game::Action roughtAction = calculateRoughtAction();
        game::Action rationalAction = calculateRationalAction();

        bool randomSelection = false;
        ACTION action = NO_ACTION;
        int tokensToBet = -1;

        if (m_roughtPlay) {
            action = roughtAction.getAction();
            tokensToBet = roughtAction.getTokens();
        }
        // Fusion des deux résultats :Si les actions ne sont pas les mêmes, on choisit une des deux actions:
        else if (roughtAction.getAction() != rationalAction.getAction()) {
            //CHECKER et SE_COUCHER
            if ((roughtAction.getAction() == CHECK
                    && rationalAction.getAction() == FOLD)
                 ||(roughtAction.getAction() == FOLD
                    && rationalAction.getAction()==CHECK)) {
                action = CHECK;
            }
            //CHECKER et SUIVRE
            if ((roughtAction.getAction() == CHECK
                    && rationalAction.getAction() == CALL)
                 ||(roughtAction.getAction() == CALL
                    && rationalAction.getAction() == CHECK)) {
                randomSelection = true;
            }
            //CHECKER et MISER
            if ((roughtAction.getAction() == CHECK
                    && rationalAction.getAction() == BET)
                 ||(roughtAction.getAction() == BET
                    && rationalAction.getAction() == CHECK)) {

                if (roughtAction.getAction() == CHECK) {
                    roughtAction.setAction(BET);
                    roughtAction.setTokens(m_ai->getGame()->getBlind());
                }
                else {
                    rationalAction.setAction(BET);
                    rationalAction.setTokens(m_ai->getGame()->getBlind());
                }
            }
            //SUIVRE et RELANCER
            if ((roughtAction.getAction() == CALL
                    && rationalAction.getAction() == RAISE)
                  || (roughtAction.getAction() == RAISE
                     && rationalAction.getAction() == CALL)) {

                if (roughtAction.getAction() == RAISE) {
                    if (roughtAction.getTokens() > m_ai->getGame()->getCurrentBet() * 2) {
                        tokensToBet = m_ai->getGame()->getCurrentBet() * 2;
                        action = RAISE;
                    }
                    else {
                        action = CALL;
                    }
                }
                else {
                    if (rationalAction.getTokens() > m_ai->getGame()->getCurrentBet() * 2) {
                        tokensToBet = m_ai->getGame()->getCurrentBet() * 2;
                        action = RAISE;
                    }
                    else {
                        action = CALL;
                    }
                }
            }
            //SUIVRE et SE_COUCHER
            if ((roughtAction.getAction() == CALL
                    && rationalAction.getAction() == FOLD)
                 || (roughtAction.getAction() == FOLD
                    && rationalAction.getAction() == CALL)) {
                randomSelection = true;
            }
            //RELANCER et SE_COUCHER
            if ((roughtAction.getAction() == FOLD
                    && rationalAction.getAction() == RAISE)
                 || (roughtAction.getAction() == RAISE
                    && rationalAction.getAction() == FOLD)) {
                action = CALL;
            }
            //MISER et SE_COUCHER
            if ((roughtAction.getAction() == BET
                    && rationalAction.getAction() == FOLD)
                 || (roughtAction.getAction() == FOLD
                    && rationalAction.getAction() == BET)) {

                if (roughtAction.getAction() == FOLD) {
                    roughtAction.setAction(BET);
                    rationalAction.setTokens(rationalAction.getTokens() / 2);
                }
                else {
                    rationalAction.setAction(BET);
                    roughtAction.setTokens(roughtAction.getTokens() / 2);
                }
            }
            //TAPIS et MISER
            if ((roughtAction.getAction() == ALL_IN
                    && rationalAction.getAction() == BET)
                 || (roughtAction.getAction() == BET
                     && rationalAction.getAction() == ALL_IN)) {

                if (roughtAction.getAction() == ALL_IN) {
                    roughtAction.setAction(BET);
                    roughtAction.setTokens(m_ai->getCave());
                } else {
                    rationalAction.setAction(BET);
                    rationalAction.setTokens(m_ai->getCave());
                }
            }
            //TAPIS et RELANCER
            if ((roughtAction.getAction() == ALL_IN
                    && rationalAction.getAction() == RAISE)
                 || (roughtAction.getAction() == RAISE
                     && rationalAction.getAction() == ALL_IN)) {

                if (roughtAction.getAction() == ALL_IN) {
                    roughtAction.setAction(RAISE);
                    roughtAction.setTokens(m_ai->getCave());
                }
                else {
                    rationalAction.setAction(RAISE);
                    rationalAction.setTokens(m_ai->getCave());
                }
            }
            //TAPIS et SE_COUCHER
            if ((roughtAction.getAction() == ALL_IN
                    && rationalAction.getAction() == FOLD)
                || (roughtAction.getAction() == FOLD
                    && rationalAction.getAction() == ALL_IN)) {

                int tokens = (m_ai->getCave() == 1) ? 1 : m_ai->getCave() / 2;

                if (m_ai->getGame()->canCall(m_ai->getPosition())) {
                    action = CALL;
                }
                else if (m_ai->getGame()->canBet(m_ai->getPosition(),tokens)) {
                    action = BET;
                    tokensToBet = tokens;
                }
            }
            //TAPIS et CHECKER
            if ((roughtAction.getAction() == ALL_IN
                    && rationalAction.getAction() == CHECK)
                 || (roughtAction.getAction() == CHECK
                     && rationalAction.getAction() == ALL_IN)) {
                if (m_ai->getGame()->canCall(m_ai->getPosition())) {
                    action = CALL;
                }
                else {
                    int tokens = (m_ai->getCave() == 1) ? 1 : m_ai->getCave() / 2;

                    if (m_ai->getGame()->canBet(m_ai->getPosition(),tokens)) {
                        action = BET;
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
                    tokensToBet = roughtAction.getTokens();
                }
                //Sinon, random E[agressivite+1 - total], on prend l'action et les jetons de la rationalité
                else {
                    action = rationalAction.getAction();
                    tokensToBet = rationalAction.getTokens();
                }
            }
       }

       //Sinon, si l'action est relancer ou miser
       if (roughtAction.getAction() == rationalAction.getAction()
               && (roughtAction.getAction() == ACTION::RAISE
                   || roughtAction.getAction() == ACTION::BET)) {

            action = roughtAction.getAction();

            int tokensToBetMax, tokensToBetMin, rateMax;

            if (roughtAction.getTokens() > rationalAction.getTokens()) {
                tokensToBetMax = roughtAction.getTokens();
                tokensToBetMin = rationalAction.getTokens();
            }
            else {
                tokensToBetMax = rationalAction.getTokens();
                tokensToBetMin = roughtAction.getTokens();
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
           if (action == NO_ACTION) {
                action = roughtAction.getAction();
           }
        }

        if (tokensToBet > m_ai->getCave()) {
            tokensToBet = m_ai->getCave();
        }

        return game::Action(action,tokensToBet);
    }
}
