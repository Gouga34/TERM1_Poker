/*========================================================================
Nom: ContenuFenetreHumain.cpp       Auteur: Manuel Chataigner
Maj: 20/05/2015            Creation: 20/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ContenuFenetreHumain.
=========================================================================*/

#include "../../include/Gui/HumanWindowContent.h"
#include "../../include/Gui/CardsDialog.h"
#include "../../include/Gui/Logger.h"

#include <QString>
#include <QHBoxLayout>
#include <QEventLoop>


namespace gui
{
    QPixmap *HumanWindowContent::m_cardsTexture = 0;

    HumanWindowContent::HumanWindowContent(game::Game *game, Window *window) : WindowContent(game)
    {
        // Couleur de fond
        QPalette pal(palette());
        pal.setColor(QPalette::Background, QColor(20, 127, 20));
        window->setPalette(pal);

        QHBoxLayout *layout = new QHBoxLayout;

        // ////////////////////////////////////////////////////
        // Logs
        // ////////////////////////////////////////////////////

        QVBoxLayout *logsLayout = new QVBoxLayout;

        QLabel *dashboardTitle = new QLabel("Tableau de bord :");
        dashboardTitle->setFixedSize(300, 50);
        dashboardTitle->setStyleSheet("QLabel {color : #89DF57; text-align:center; font-weight:bold;}");

        m_logs->setReadOnly(true);
        m_logs->setMaximumSize(300, 300);

        m_logsButton = new QPushButton("Afficher/Cacher");
        m_logsButton->setFixedWidth(300);

        m_partResult = new QLabel;

        logsLayout->setAlignment(Qt::AlignTop);
        logsLayout->setAlignment(Qt::AlignHCenter);
        logsLayout->setSpacing(20);

        logsLayout->addWidget(dashboardTitle);
        logsLayout->addWidget(m_logs);
        logsLayout->addWidget(m_logsButton);
        logsLayout->addWidget(m_partResult);

        connect(m_logsButton, SIGNAL(clicked()), this, SLOT(logsDisplay()));


        // ////////////////////////////////////////////////////
        // Cartes
        // ////////////////////////////////////////////////////

        QVBoxLayout *gameLayout = new QVBoxLayout;

        // Chargement de l'image
        if (!m_cardsTexture){
            m_cardsTexture = new QPixmap(QString::fromStdString(CARDS_TEXTURE));
        }

        QHBoxLayout *opponentLayout =new QHBoxLayout;

        //Sous-layout IA
        m_aiActionDone = new QLineEdit;
        m_aiActionDone->setReadOnly(true);
        m_aiActionDone->setFixedWidth(80);
        m_aiActionDone->setPlaceholderText("Action");

        m_opponentHandLayout = new CardsList;

        opponentLayout->addLayout(m_opponentHandLayout);
        opponentLayout->addWidget(m_aiActionDone);

        m_handLayout = new CardsList;
        m_commonCardsLayout = new CardsList;

        gameLayout->setSpacing(150);
        gameLayout->setAlignment(Qt::AlignTop);

        gameLayout->addLayout(opponentLayout);
        gameLayout->addLayout(m_commonCardsLayout);
        gameLayout->addLayout(m_handLayout);


        // ////////////////////////////////////////////////////
        // Boutons d'action
        // ////////////////////////////////////////////////////

        QHBoxLayout *startLayout = new QHBoxLayout;

        m_startButton = new QPushButton("Démarrage partie");
        m_startButton->setMaximumWidth(150);
        m_startButton->hide();
        connect(m_startButton, SIGNAL(clicked()), window, SLOT(startGame()));

        m_cardsChoiceButton = new QCheckBox("Choix des cartes");
        m_cardsChoiceButton->setChecked(false);
        m_cardsChoiceButton->setObjectName(QString("Checkbox"));
        m_cardsChoiceButton->setStyleSheet("QWidget#Checkbox { background-color: rgb(255, 255, 255); border-style: solid; border-color: black; border-width: 1px; padding : 3px; }");

        startLayout->setAlignment(Qt::AlignLeft);
        startLayout->setSpacing(10);
        startLayout->addWidget(m_startButton);
        startLayout->addWidget(m_cardsChoiceButton);


        QVBoxLayout *buttonsLayout = new QVBoxLayout;

        buttonsLayout->setSpacing(10);
        buttonsLayout->setAlignment(Qt::AlignTop);

        m_buttons[CHECK] = new QPushButton("Checker");
        m_buttons[BET] = new QPushButton("Miser");
        m_buttons[CALL] = new QPushButton("Suivre");
        m_buttons[RAISE] = new QPushButton("Relancer");
        m_buttons[DROP] = new QPushButton("Se coucher");
        m_buttons[ALL_IN] = new QPushButton("Tapis");

        buttonsLayout->addWidget(m_buttons[CHECK]);
        buttonsLayout->addWidget(m_buttons[BET]);
        buttonsLayout->addWidget(m_buttons[CALL]);
        buttonsLayout->addWidget(m_buttons[RAISE]);
        buttonsLayout->addWidget(m_buttons[DROP]);
        buttonsLayout->addWidget(m_buttons[ALL_IN]);

        connect(m_buttons[CHECK], SIGNAL(clicked()), this, SLOT(check()));
        connect(m_buttons[BET], SIGNAL(clicked()), this, SLOT(bet()));
        connect(m_buttons[CALL], SIGNAL(clicked()), this, SLOT(call()));
        connect(m_buttons[RAISE], SIGNAL(clicked()), this, SLOT(raise()));
        connect(m_buttons[DROP], SIGNAL(clicked()), this, SLOT(drop()));
        connect(m_buttons[ALL_IN], SIGNAL(clicked()), this, SLOT(allIn()));


        // ////////////////////////////////////////////////////
        // Fenetre
        // ////////////////////////////////////////////////////

        m_pot = new TokensCounter;
        m_playerCave = new TokensCounter;
        m_aiCave = new TokensCounter;

        QHBoxLayout *playerLayout = new QHBoxLayout;

        m_betValue = new QSpinBox;
        m_betValue->setRange(1, 10000);

        playerLayout->setAlignment(Qt::AlignRight);
        playerLayout->setSpacing(50);

        playerLayout->addWidget(m_playerCave);
        playerLayout->addWidget(m_betValue);
        playerLayout->addLayout(buttonsLayout);


        QVBoxLayout *optionsLayout = new QVBoxLayout;

        optionsLayout->setSpacing(100);
        optionsLayout->setAlignment(Qt::AlignHCenter);

        optionsLayout->addLayout(startLayout);
        optionsLayout->addWidget(m_aiCave);
        optionsLayout->addWidget(m_pot);
        optionsLayout->addLayout(playerLayout);

        // Layout principal
        layout->setSpacing(100);
        layout->addLayout(logsLayout);
        layout->addLayout(gameLayout);
        layout->addLayout(optionsLayout);

        setLayout(layout);
    }

    HumanWindowContent::~HumanWindowContent()
    {

    }

    void HumanWindowContent::startPart()
    {
        // Sélection des cartes par l'utilisateur
        if (m_cardsChoiceButton->isChecked()) {
            CardsDialog cardsWindow(this);
            std::vector<int> ids = cardsWindow.cardsChoice();

            if (!ids.empty()) {
                m_game->affectsCards(ids);
            }
        }

        m_startButton->hide();
        m_cardsChoiceButton->hide();
        m_partResult->clear();

        Logger::getInstance()->addLogs("Distribution des cartes");
        displayTable();


        // Affichage de la main adverse dans les logs
        std::vector<game::Card> opponentHand = m_game->getPlayer(1)->getHand();

        Logger::getInstance()->addLogs("Jeu adverse : ");
        for (unsigned int i = 0; i < opponentHand.size(); i++) {
            Logger::getInstance()->addLogs("-> " + QString::number(opponentHand.at(i).getRank())
                      + " " + GraphicCard::colors[opponentHand.at(i).getColor()]);
        }

        // Main du joueur
        m_handLayout->clear();
        m_handLayout->addCards(m_game->getPlayer(0)->getHand());

        // Cartes communes
        m_commonCardsLayout->clear();

        for (int i = 0; i < 5; i++) {
            GraphicCard *back = new GraphicCard(0, 0);
            m_commonCardsLayout->addWidget(back);
        }

        // Main adverse

        GraphicCard *back = new GraphicCard(0, 0);
        GraphicCard *back2 = new GraphicCard(0, 0);

        m_opponentHandLayout->clear();
        m_opponentHandLayout->addWidget(back);
        m_opponentHandLayout->addWidget(back2);

        enableButtons(false);
    }

    void HumanWindowContent::refresh()
    {
        m_pot->display(m_game->getPot());
        m_playerCave->display(m_game->getPlayer(0)->getCave());
        m_aiCave->display(m_game->getPlayer(1)->getCave());

        m_betValue->setMinimum(m_game->getBlind());

        endOfPart();
    }

    game::Action HumanWindowContent::getAction()
    {
        // Mise à jour des informations
        displayTable();
        m_pot->display(m_game->getPot());
        m_playerCave->display(m_game->getPlayer(0)->getCave());
        m_aiCave->display(m_game->getPlayer(1)->getCave());

        enableButtons(true);

        // Attente d'une action de l'utilisateur
        QEventLoop loop;
        connect(this, SIGNAL(actionSelected()), &loop, SLOT(quit()));
        loop.exec();

        enableButtons(false);

        return game::Action(m_userAction, m_betValue->value());
    }

    void HumanWindowContent::logsDisplay()
    {
        bool visible = !m_logs->isHidden();

        m_logs->setHidden(visible);
    }

    void HumanWindowContent::enableButtons(bool value)
    {
        // On désactive tous les boutons en fin de tour
        if (!value) {
            for (int i = 0; i < BUTTONS_NB; i++) {
                m_buttons[i]->setEnabled(false);
            }
        }
        else {      // Sinon on vérifie les actions possibles pour les activer

            for (int i = 0; i < BUTTONS_NB; i++) {
                switch (i) {

                    case CHECK:
                        m_buttons[i]->setEnabled(m_game->canCheck(0));
                        break;

                    case BET:
                        m_buttons[i]->setEnabled(m_game->canBet(0, 1));
                        break;

                    case CALL:
                        m_buttons[i]->setEnabled(m_game->canCall(0));
                        break;

                    case RAISE:
                        m_buttons[i]->setEnabled(m_game->canRaise(0, 2 * m_game->getCurrentBet()));
                        break;

                    default:
                        m_buttons[i]->setEnabled(true);
                        break;
                }
            }

            m_betValue->setMinimum(2 * m_game->getCurrentBet());
            m_betValue->setMaximum(m_game->getPlayer(0)->getCave());
        }
    }

    void HumanWindowContent::displayTable()
    {
        std::vector<game::Card> table = m_game->getTable();

        m_commonCardsLayout->clear();
        m_commonCardsLayout->addCards(table);

        // On complète la table avec des dos de carte
        for (unsigned int i = 0; i < 5 - table.size(); i++) {
            GraphicCard *back = new GraphicCard(0, 0);
            m_commonCardsLayout->addWidget(back);
        }
    }

    void HumanWindowContent::check()
    {
        m_userAction = ACTION::CHECK;

        Logger::getInstance()->addLogs("Joueur 1 check");

        emit actionSelected();
    }

    void HumanWindowContent::bet()
    {
        m_userAction = ACTION::BET;

        Logger::getInstance()->addLogs("Joueur 1 mise " + QString::number(m_betValue->value()));

        emit actionSelected();
    }

    void HumanWindowContent::call()
    {
        m_userAction = ACTION::CALL;

        Logger::getInstance()->addLogs("Joueur 1 suit");

        emit actionSelected();
    }

    void HumanWindowContent::raise()
    {
        m_userAction = ACTION::RAISE;

        Logger::getInstance()->addLogs("Joueur 1 relance " + QString::number(m_betValue->value()));

        emit actionSelected();
    }

    void HumanWindowContent::drop()
    {
        m_userAction = ACTION::FOLD;

        Logger::getInstance()->addLogs("Joueur 1 se couche");

        emit actionSelected();
    }

    void HumanWindowContent::allIn()
    {
        m_userAction = ACTION::ALL_IN;

        Logger::getInstance()->addLogs("Joueur 1 fait tapis");

        emit actionSelected();
    }

    void HumanWindowContent::endOfPart()
    {
        Logger::getInstance()->addLogs("Partie terminée !");
        enableButtons(false);

        m_opponentHandLayout->clear();
        m_opponentHandLayout->addCards(m_game->getPlayer(1)->getHand());

        displayTable();

        int result = m_game->getGameResult();

        if (result == WON) {
            m_partResult->setStyleSheet("QLabel {color : #89DF57; font-size : 40px; text-align:center; padding-left:80px; font-weight:bold;}");
            m_partResult->setText("Gagné !");
        }
        else if (result == EQUALITY) {
            m_partResult->setStyleSheet("QLabel {color : #23BDFE; font-size : 40px; text-align:center; padding-left:80px; font-weight:bold;}");
            m_partResult->setText("Egalité");
        }
        else {
            m_partResult->setStyleSheet("QLabel {color : #FE0000; font-size : 40px; text-align:center; padding-left:70px; font-weight:bold;}");
            m_partResult->setText("Perdu !");
        }

        m_startButton->setText("Rejouer");
        m_startButton->show();
        m_cardsChoiceButton->show();
    }
}
