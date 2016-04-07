/*========================================================================
Nom: Fenetre.cpp       Auteur: Manuel CHATAIGNER
Maj: 11/02/2015            Creation: 11/02/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Fenetre.
=========================================================================*/

#include "../../include/Interface/Window.h"
#include "../../include/Interface/GraphicCard.h"
#include "../../include/Interface/HumanWindowContent.h"
#include "../../include/Game/HumanPlayer.h"
#include "../../include/AI/ArtificialIntelligenceProfiling.h"
#include "../../include/AI/IdealCalibration.h"
#include "../../include/Interface/AiWindowContent.h"

#include <QVBoxLayout>
#include <iostream>


Window::Window() : QWidget(), m_game(0), m_content(0)
{
    move(0,0);
    setWindowTitle(tr("Poker"));
    resize(1280,400);

    /** Fenêtre **/

    // Layout principal
    m_layout = new QVBoxLayout;
    m_startButton.setFixedSize(100, 30);

    m_layout->setAlignment(Qt::AlignCenter);
    m_layout->addWidget(&m_startButton);

    initialize();

    setLayout(m_layout);
}

Window::~Window()
{
    if (m_game) {
        delete m_game;
    }

    if (m_content) {
        delete m_content;
    }
}

void Window::initialize()
{
    if (m_game) {
        delete m_game;
        m_game = 0;
    }

    m_layout->removeWidget(m_content);

    if (m_content) {
        delete m_content;
        m_content = 0;
    }


    hide();

    //Récupération des options du jeu
    OptionsDialog optionsWindow;

    // Création du jeu
    m_game = new game::Game(2, 20, optionsWindow.getOptions());

    /** Joueur 1 **/

    game::Player *p1;
    QString playerPseudo = "inconnu";

    if (m_game->getOptions().aiPlayer) {
        p1 = new ai::ArtificialIntelligence(true, CAVE_JOUEURS, 0);
        ai::ArtificialIntelligence *ai = static_cast<ai::ArtificialIntelligence*>(p1);

        // Si on lance une recherche de calibrage optimal ou si on a choisi le calibrage
        if (!m_game->getOptions().profiling || m_game->getOptions().fixedProfiledAiCalibration) {
            ai->setCalibration(m_game->getOptions().profiledAi);
        }

        profiling::Profile calibration = *(ai->getCalibration());
        playerPseudo = QString::number(calibration.getAggressiveness()) + "_" + QString::number(calibration.getRationality());

        m_content = new AiWindowContent(m_game, this);
    }
    else {
        if (!m_game->getOptions().pseudo.isEmpty()) {
            playerPseudo = m_game->getOptions().pseudo;
        }

        m_content = new HumanWindowContent(m_game, this);
        p1 = new game::HumanPlayer(true, CAVE_JOUEURS, 0, static_cast<HumanWindowContent*>(m_content));
    }

    m_game->addPlayer(p1);


    /** IA qui profile **/

    ai::ArtificialIntelligenceProfiling *ai = new ai::ArtificialIntelligenceProfiling(false, CAVE_JOUEURS, 1);

    if (m_game->getOptions().fixedProfilingAiCalibration) {
        ai->setCalibration(m_game->getOptions().profilingAi);
    }

    m_game->addPlayer(ai);

    // Envoi du pseudo du joueur
    ai->setProfiledPlayerNickname(playerPseudo.toStdString());


    /** Bouton démarrer **/

    disconnect(&m_startButton, SIGNAL(clicked()), this, SLOT(initialize()));
    if (m_game->getOptions().profiling) {
        connect(&m_startButton, SIGNAL(clicked()), this, SLOT(startGame()));
    }
    else {
        connect(&m_startButton, SIGNAL(clicked()), this, SLOT(startOptimalCalibration()));
    }

    m_startButton.setText("Démarrer");
    m_layout->addWidget(m_content);
    show();
}

void Window::startGame()
{
    m_startButton.hide();

    int nbCalibrations = m_game->getOptions().nbCalibrations;
    int nbParts = m_game->getOptions().nbParts;

    if (m_game->getPlayer(0)->isHumain()) {
        nbCalibrations = 1;
        nbParts = 1;
    }

    ai::ArtificialIntelligenceProfiling *profilingAi = static_cast<ai::ArtificialIntelligenceProfiling*>(m_game->getPlayer(1));

    for (int i = 0; i < nbCalibrations; i++) {

        if (!m_game->getPlayer(0)->isHumain()) {
            ai::ArtificialIntelligence *profiledAi = static_cast<ai::ArtificialIntelligence*>(m_game->getPlayer(0));

            if (!m_game->getOptions().fixedProfiledAiCalibration) {
                profiledAi->changeRandomlyCalibration();
            }

            std::cout << "Calibrage IA profilée : agressivité: " << profiledAi->getCalibration()->getAggressiveness()
                    << " rationalité: " << profiledAi->getCalibration()->getRationality() << std::endl;

            QString pseudo = QString::number(profiledAi->getCalibration()->getAggressiveness()) + "_" + QString::number(profiledAi->getCalibration()->getRationality());

            profilingAi->setProfiledPlayerNickname(pseudo.toStdString());
        }

        for(int j = 0; j < nbParts; j++) {
            std::cout << "Partie " << j+1 << std::endl;

            m_game->newGame();

            if (m_game->getPlayer(0)->isHumain()) {
                HumanWindowContent *content = static_cast<HumanWindowContent*>(m_content);
                content->startPart();
            }

            m_game->launchPart();

            m_content->refresh();

            m_game->newHand();
        }

        profilingAi->writeEarningsAnalysis();
    }

    disconnect(&m_startButton, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(&m_startButton, SIGNAL(clicked()), this, SLOT(initialize()));
    m_startButton.setText("Réinitialiser");
    m_startButton.show();
}

void Window::startOptimalCalibration()
{
    ai::ArtificialIntelligence *profilingAi = static_cast<ai::ArtificialIntelligence*>(m_game->getPlayer(1));
    ai::ArtificialIntelligence *profiledAi = static_cast<ai::ArtificialIntelligence*>(m_game->getPlayer(0));

    ai::IdealCalibration calibration(m_game, profilingAi->getCalibration(), profiledAi->getCalibration(), m_game->getOptions().nbParts);
    calibration.launchParts();
}

