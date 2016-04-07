/*========================================================================
Nom: ContenuFenetreIA.cpp       Auteur: Morgane Vidal
Maj: 22/05/2015            Creation: 22/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe
                                                        ContenuFenetreIA.
=========================================================================*/

#include "../../include/Gui/AiWindowContent.h"
#include "../../include/AI/ArtificialIntelligenceProfiling.h"
#include <QStringList>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QApplication>
#include <QScrollBar>
#include <QHeaderView>


namespace gui
{
    AiWindowContent::AiWindowContent(game::Game *game, Window *window):WindowContent(game)
    {
        // Couleur de fond
        QPalette pal(palette());
        pal.setColor(QPalette::Background, QColor(240, 240, 240));
        window->setPalette(pal);

        //fenetre
        QVBoxLayout *box = new QVBoxLayout;

        //Etat parties (en cours/ terminé):
        m_partsState.setText("Parties en cours");

        //Tableau récapitulatif des parties
        initPartsSummary();

        //Tableau contenant les résultats globaux de toutes les parties
        initGlobalResults();

        //Graphique
        initResults();

        box->addWidget(&m_partsState);
        box->addWidget(&m_profiledAiCalibration);
        box->addWidget(&m_partsSummary);
        box->addWidget(&m_globalResults);
        box->addWidget(&m_resultsGraph);

        setLayout(box);
    }

    AiWindowContent::~AiWindowContent()
    {

    }

    void AiWindowContent::initPartsSummary()
    {
        m_partsSummary.setColumnCount(8);

        //Ajout première ligne:
        QStringList header;
        header << "Scenarios en cours" << "Agressivité IA qui profile" << "Rationalité IA qui profile"
             << "Agressivité déduite" << "Rationalité déduite" << "Taux de similarité"
            /*<< "Jeu agressif"*/ << "Gains" << "Gain IA qui profile";

        m_partsSummary.setHorizontalHeaderLabels(header);

        m_partsSummary.resizeColumnsToContents();

        m_partsSummary.setMaximumHeight(300);
    }

    void AiWindowContent::initGlobalResults()
    {
        m_globalResults.setColumnCount(5);

        QStringList header;
        header << "Agressivité déduite" << "Rationalité déduite" << "Taux de similarité" << /*"Nombre de jeux agressifs"
                <<*/ "Total gains" << "Nombre de parties gagnées";

        QStringList verticalHeader;
        verticalHeader << "Récapitulatif des résultats : ";

        m_globalResults.setVerticalHeaderLabels(verticalHeader);
        m_globalResults.setHorizontalHeaderLabels(header);

        QHeaderView* headers = m_globalResults.horizontalHeader();
        headers->setSectionResizeMode(QHeaderView::Stretch);

        m_globalResults.resizeColumnsToContents();

        int newLine = m_globalResults.rowCount();
        m_globalResults.insertRow(newLine);


        //init à 0:
        m_globalResults.setItem(newLine, DEDUCTED_AGGRESSIVENESS, new QTableWidgetItem(QString::number(0)));
        m_globalResults.setItem(newLine, DEDUCTED_RATIONALITY, new QTableWidgetItem(QString::number(0)));
        m_globalResults.setItem(newLine, SIMILARITY, new QTableWidgetItem(QString::number(0)));
        //resultatsGlobaux.setItem(newLine, NB_JEUX_AGRESSIFS, new QTableWidgetItem(QString::number(0)));
        m_globalResults.setItem(newLine,TOTAL_WINNINGS, new QTableWidgetItem(QString::number(0)));
        m_globalResults.setItem(newLine, NB_WON_GAMES, new QTableWidgetItem(QString::number(0)));

        m_globalResults.setFixedHeight(50);
    }

    void AiWindowContent::initResults()
    {
        m_resultsGraph.setMinimumSize(600, 300);

        // add two new graphs and set their look:
        m_resultsGraph.addGraph(m_resultsGraph.xAxis, m_resultsGraph.yAxis);
        m_resultsGraph.graph(0)->setPen(QPen(Qt::blue)); // Taux de similarité
        m_resultsGraph.graph(0)->setName("Taux de similarité");

        m_resultsGraph.addGraph(m_resultsGraph.xAxis, m_resultsGraph.yAxis2);
        m_resultsGraph.graph(1)->setPen(QPen(Qt::red)); // Cumul des gains
        m_resultsGraph.graph(1)->setName("Cumul des jetons gagnés");

        // configure right and top axis to show ticks but no labels:
        // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
        m_resultsGraph.yAxis2->setVisible(true);
        m_resultsGraph.yAxis2->setTickLabels(true);

        m_resultsGraph.xAxis->setRange(0, m_game->getOptions().nbParts);
        m_resultsGraph.yAxis2->setRange(0, 100);
        m_resultsGraph.yAxis->setRange(-30000, 30000);

        // make left and bottom axes always transfer their ranges to right and top axes:
        connect(m_resultsGraph.xAxis, SIGNAL(rangeChanged(QCPRange)), m_resultsGraph.xAxis2, SLOT(setRange(QCPRange)));

        // Note: we could have also just called resultats.rescaleAxes(); instead
        // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
        m_resultsGraph.setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
        m_resultsGraph.legend->setVisible(true);
        m_resultsGraph.axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom | Qt::AlignRight);

        m_partsNumberPoints << 0;
        m_similarityRatePoints << 0.0;
        m_winningsPluralityPoints << 0;

        // pass data points to graphs:
        m_resultsGraph.graph(0)->setData(m_partsNumberPoints, m_similarityRatePoints);
        m_resultsGraph.graph(1)->setData(m_partsNumberPoints, m_winningsPluralityPoints);

        // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
        m_resultsGraph.graph(0)->rescaleAxes();
        // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
        m_resultsGraph.graph(1)->rescaleAxes();

        m_resultsGraph.xAxis->setLabel("Nombre de parties");
        m_resultsGraph.yAxis->setLabel("Taux de similarité");
        m_resultsGraph.yAxis2->setLabel("Cumul de jetons gagnés");

        m_resultsGraph.addLayer("sectionBackground", m_resultsGraph.layer("grid"), QCustomPlot::limBelow);
    }

    void AiWindowContent::updateGlobalResults()
    {
        ai::ArtificialIntelligenceProfiling *ai = static_cast<ai::ArtificialIntelligenceProfiling*>(m_game->getPlayer(1));
        int line = m_globalResults.rowCount() - 1;

        //Ag déduite:
        m_globalResults.setItem(line, DEDUCTED_AGGRESSIVENESS, new QTableWidgetItem(QString::number(ai->getScenario().getGlobalDeductedProfile().getAggressiveness())));
        //Ra déduite:
        m_globalResults.setItem(line, DEDUCTED_RATIONALITY, new QTableWidgetItem(QString::number(ai->getScenario().getGlobalDeductedProfile().getRationality())));
        //Taux similarite:
        m_globalResults.setItem(line,SIMILARITY, new QTableWidgetItem(QString::number(ai->getScenario().getSimilarityDegree())));
        //Nb jeux agressifs:
        //QTableWidgetItem* prevNb = resultatsGlobaux.takeItem(ligne,NB_JEUX_AGRESSIFS);
        //int nbPrecs = nbPrec->text().toInt();
        //resultatsGlobaux.setItem(ligne,NB_JEUX_AGRESSIFS,new QTableWidgetItem(QString::number(nbPrecs+IA->getProfilage()->jeuAgressif)) );
        //Total gains:
        QTableWidgetItem* previousWinningsItem = m_globalResults.takeItem(line, TOTAL_WINNINGS);
        int previousWinnings = previousWinningsItem->text().toInt();
        int totalWinnings = previousWinnings + ai->getProfiling()->m_numberTokensWonProfilingAI;
        m_globalResults.setItem(line, TOTAL_WINNINGS, new QTableWidgetItem(QString::number(totalWinnings)));

        // Nb parties gagnées
        if (ai->getProfiling()->m_gameResultProfilingAIviewpoint == 1) {
            int nbPartiesGagneesPrec = m_globalResults.takeItem(line, NB_WON_GAMES)->text().toInt();
            int nbPartiesTot = nbPartiesGagneesPrec + ai->getProfiling()->m_gameResultProfilingAIviewpoint;
            m_globalResults.setItem(line, NB_WON_GAMES, new QTableWidgetItem(QString::number(nbPartiesTot)));
        }
    }

    void AiWindowContent::updateProfiledAiCalibration()
    {
       int profiledAiAggressiveness = static_cast<ai::ArtificialIntelligence*>(m_game->getPlayer(0))->getCalibration()->getAggressiveness();
       int profiledAiRationality = static_cast<ai::ArtificialIntelligence*>(m_game->getPlayer(0))->getCalibration()->getRationality();
       m_profiledAiCalibration.setText("Calibrage IA profilée : " + QString::number(profiledAiAggressiveness) + "% d'agressivité et "
                                       + QString::number(profiledAiRationality) + "% de rationalité");
    }

    void AiWindowContent::addLine()
    {
        int newLine = m_partsSummary.rowCount();
        ai::ArtificialIntelligenceProfiling *ai = static_cast<ai::ArtificialIntelligenceProfiling*>(m_game->getPlayer(1));

        //Ajout d'une ligne:
        m_partsSummary.insertRow(newLine);

        m_partsSummary.setItem(newLine, CURRENT_SCENARIOS, new QTableWidgetItem(QString::number(ai->getProfiling()->m_testScenario)));

        m_partsSummary.setItem(newLine, PROFILING_AI_AGGRESSIVENESS, new QTableWidgetItem(QString::number(ai->getCalibration()->getAggressiveness())));

        m_partsSummary.setItem(newLine, PROFILING_AI_RATIONALITY, new QTableWidgetItem(QString::number(ai->getCalibration()->getRationality())));

        m_partsSummary.setItem(newLine, DEDUCTED_AGGRESSIVENESS, new QTableWidgetItem(QString::number(ai->getScenario().getGlobalDeductedProfile().getAggressiveness())));

        m_partsSummary.setItem(newLine, DEDUCTED_RATIONALITY, new QTableWidgetItem(QString::number(ai->getScenario().getGlobalDeductedProfile().getRationality())));

        m_partsSummary.setItem(newLine, SIMILARITY_RATE, new QTableWidgetItem(QString::number(ai->getScenario().getSimilarityDegree())));

        //recapParties.setItem(newLine, AGGRESSIVE_PLAY, new QTableWidgetItem(QString::number(IA->getProfilage()->jeuAgressif)));

        m_partsSummary.setItem(newLine, WINNINGS, new QTableWidgetItem(QString::number(ai->getProfiling()->m_numberTokensWonProfilingAI)));

        m_partsSummary.setItem(newLine, PROFILING_AI_WINNINGS, new QTableWidgetItem(QString::number(ai->getProfiling()->m_gameResultProfilingAIviewpoint)));
    }

    void AiWindowContent::autoTabScroll()
    {
        QScrollBar *scrollbar = m_partsSummary.verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
    }

    void AiWindowContent::changeBackgroundDuringProfiling()
    {
        QCPItemRect *section = new QCPItemRect(&m_resultsGraph);
        m_resultsGraph.addItem(section);

        section->setLayer("sectionBackground");

        section->topLeft->setTypeX(QCPItemPosition::ptPlotCoords);
        section->topLeft->setTypeY(QCPItemPosition::ptAxisRectRatio);
        section->topLeft->setAxes(m_resultsGraph.xAxis, m_resultsGraph.yAxis);
        section->topLeft->setAxisRect(m_resultsGraph.axisRect());

        section->bottomRight->setTypeX(QCPItemPosition::ptPlotCoords);
        section->bottomRight->setTypeY(QCPItemPosition::ptAxisRectRatio);
        section->bottomRight->setAxes(m_resultsGraph.xAxis, m_resultsGraph.yAxis);
        section->bottomRight->setAxisRect(m_resultsGraph.axisRect());

        section->setClipToAxisRect(true); // is by default true already, but this will change in QCP 2.0.0

        section->topLeft->setCoords(m_partsSummary.rowCount()-1, -0.1); // the y value is now in axis rect ratios, so -0.1 is "barely above" the top axis rect border
        section->bottomRight->setCoords(m_partsSummary.rowCount(), 1.1); // the y value is now in axis rect ratios, so 1.1 is "barely below" the bottom axis rect border

        section->setBrush(QBrush(QColor(200, 200, 200, 100)));
        section->setPen(Qt::NoPen);
    }

    void AiWindowContent::updateResultsGraph()
    {
        m_partsNumberPoints << m_partsSummary.rowCount();
        m_similarityRatePoints << m_globalResults.item(0, SIMILARITY)->text().toFloat();
        m_winningsPluralityPoints << m_globalResults.item(0, TOTAL_WINNINGS)->text().toInt();

        // pass data points to graphs:
        m_resultsGraph.graph(0)->setData(m_partsNumberPoints, m_similarityRatePoints);
        m_resultsGraph.graph(1)->setData(m_partsNumberPoints, m_winningsPluralityPoints);

        // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
        m_resultsGraph.graph(0)->rescaleAxes();
        // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
        m_resultsGraph.graph(1)->rescaleAxes();

        ai::ArtificialIntelligenceProfiling *ai = static_cast<ai::ArtificialIntelligenceProfiling*>(m_game->getPlayer(1));
        if (ai->getProfiling()->m_testScenario == 1) {
            changeBackgroundDuringProfiling();
        }

        m_resultsGraph.replot();
    }

    void AiWindowContent::refresh()
    {
        //Calibrage IA profilée
        updateProfiledAiCalibration();

        //Ajout ligne résultats
        addLine();

        //Résultats globaux
        updateGlobalResults();

        autoTabScroll();

        updateResultsGraph();

        qApp->processEvents();
    }
}
