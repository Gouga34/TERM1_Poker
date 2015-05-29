/*========================================================================
Nom: ContenuFenetreIA.cpp       Auteur: Morgane Vidal
Maj: 22/05/2015            Creation: 22/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe
                                                        ContenuFenetreIA.
=========================================================================*/

#include "../../include/Interface/ContenuFenetreIA.h"
#include "../../include/IA/IntelligenceArtificielleProfilage.h"
#include <QStringList>
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QApplication>
#include <QScrollBar>
#include <QHeaderView>


ContenuFenetreIA::ContenuFenetreIA(Jeu *j):ContenuFenetre(j){

    //fenetre
    QVBoxLayout *boite = new QVBoxLayout;

    //Etat parties (en cours/ terminé):
    etatParties.setText("Parties en cours");

    //Tableau récapitulatif des parties
    initialisationRecapParties();

    //Tableau contenant les résultats globaux de toutes les parties
    initialisationResultatsGlobaux();

    //Graphique
    initialisationResultats();

    boite->addWidget(&etatParties);
    boite->addWidget(&calibrageIAProfilee);
    boite->addWidget(&recapParties);
    boite->addWidget(&resultatsGlobaux);
    boite->addWidget(&graphiqueResultats);

    setLayout(boite);
}

ContenuFenetreIA::~ContenuFenetreIA(){

}

void ContenuFenetreIA::initialisationRecapParties(){
    recapParties.setColumnCount(8);

    //Ajout première ligne:
    QStringList header;
    header<<"Scenarios en cours"<<"Agressivité IA qui profile"<<"Rationalité IA qui profile"
         <<"Agressivité déduite"<<"Rationalité déduite"<<"Taux de similarité"/*<<"Jeu agressif"*/<<"Gains"<<"Gain IA qui profile";
    recapParties.setHorizontalHeaderLabels(header);

    recapParties.resizeColumnsToContents();

    recapParties.setMaximumHeight(300);
}


void ContenuFenetreIA::initialisationResultatsGlobaux(){
    resultatsGlobaux.setColumnCount(5);

    QStringList header;
    header<<"Agressivité déduite"<<"Rationalité déduite"<<"Taux de similarité"<</*"Nombre de jeux agressifs"
            <<*/"Total gains"<<"Nombre de parties gagnées";

    QStringList verticalHeader;
    verticalHeader<<"Récapitulatif des résultats : ";
    resultatsGlobaux.setVerticalHeaderLabels(verticalHeader);
    resultatsGlobaux.setHorizontalHeaderLabels(header);
    QHeaderView* headers = resultatsGlobaux.horizontalHeader();
    headers->setSectionResizeMode(QHeaderView::Stretch);


    resultatsGlobaux.resizeColumnsToContents();

    int nouvelleLigne = resultatsGlobaux.rowCount();
    resultatsGlobaux.insertRow(nouvelleLigne);


    //init à 0:
    resultatsGlobaux.setItem(nouvelleLigne, AG_DEDUITE, new QTableWidgetItem(QString::number(0)));
    resultatsGlobaux.setItem(nouvelleLigne, RA_DEDUITE, new QTableWidgetItem(QString::number(0)));
    resultatsGlobaux.setItem(nouvelleLigne, SIMILARITE, new QTableWidgetItem(QString::number(0)));
    //resultatsGlobaux.setItem(nouvelleLigne, NB_JEUX_AGRESSIFS, new QTableWidgetItem(QString::number(0)));
    resultatsGlobaux.setItem(nouvelleLigne,TOTAL_GAINS, new QTableWidgetItem(QString::number(0)));
    resultatsGlobaux.setItem(nouvelleLigne, NB_PARTIES_GAGNEES, new QTableWidgetItem(QString::number(0)));

    resultatsGlobaux.setFixedHeight(50);
}

void ContenuFenetreIA::initialisationResultats(){

    graphiqueResultats.setMinimumSize(600, 300);

    // add two new graphs and set their look:
    graphiqueResultats.addGraph(graphiqueResultats.xAxis, graphiqueResultats.yAxis);
    graphiqueResultats.graph(0)->setPen(QPen(Qt::blue)); // Taux de similarité
    graphiqueResultats.graph(0)->setName("Taux de similarité");

    graphiqueResultats.addGraph(graphiqueResultats.xAxis, graphiqueResultats.yAxis2);
    graphiqueResultats.graph(1)->setPen(QPen(Qt::red)); // Cumul des gains
    graphiqueResultats.graph(1)->setName("Cumul des jetons gagnés");

    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    graphiqueResultats.yAxis2->setVisible(true);
    graphiqueResultats.yAxis2->setTickLabels(true);

    graphiqueResultats.xAxis->setRange(0, jeu->getOptions().nombreParties);
    graphiqueResultats.yAxis2->setRange(0, 100);
    graphiqueResultats.yAxis->setRange(-30000, 30000);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(graphiqueResultats.xAxis, SIGNAL(rangeChanged(QCPRange)), graphiqueResultats.xAxis2, SLOT(setRange(QCPRange)));

    // Note: we could have also just called resultats.rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    graphiqueResultats.setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
    graphiqueResultats.legend->setVisible(true);
    graphiqueResultats.axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom | Qt::AlignRight);

    pointsNombreDeParties << 0;
    pointsTauxSimilarite << 0.0;
    pointsCumulGains << 0;

    // pass data points to graphs:
    graphiqueResultats.graph(0)->setData(pointsNombreDeParties, pointsTauxSimilarite);
    graphiqueResultats.graph(1)->setData(pointsNombreDeParties, pointsCumulGains);

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    graphiqueResultats.graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    graphiqueResultats.graph(1)->rescaleAxes();

    graphiqueResultats.xAxis->setLabel("Nombre de parties");
    graphiqueResultats.yAxis->setLabel("Taux de similarité");
    graphiqueResultats.yAxis2->setLabel("Cumul de jetons gagnés");
}

void ContenuFenetreIA::majResultatsGlobaux(){
    IntelligenceArtificielleProfilage *IA =static_cast<IntelligenceArtificielleProfilage*>(jeu->getJoueur(1));
    int ligne =resultatsGlobaux.rowCount()-1;
    //Ag déduite:
    resultatsGlobaux.setItem(ligne, AG_DEDUITE, new QTableWidgetItem(QString::number(IA->getScenario().getProfilDeduitGlobal().getAgressivite())));
    //Ra déduite:
    resultatsGlobaux.setItem(ligne, RA_DEDUITE, new QTableWidgetItem(QString::number(IA->getScenario().getProfilDeduitGlobal().getRationalite())));
    //Taux similarite:
    resultatsGlobaux.setItem(ligne,SIMILARITE, new QTableWidgetItem(QString::number(IA->getScenario().getTauxSimilarite())));
    //Nb jeux agressifs:
    //QTableWidgetItem* nbPrec=resultatsGlobaux.takeItem(ligne,NB_JEUX_AGRESSIFS);
    //int nbPrecs = nbPrec->text().toInt();
    //resultatsGlobaux.setItem(ligne,NB_JEUX_AGRESSIFS,new QTableWidgetItem(QString::number(nbPrecs+IA->getProfilage()->jeuAgressif)) );
    //Total gains:
    QTableWidgetItem* gainsPrec = resultatsGlobaux.takeItem(ligne,TOTAL_GAINS);
    int gainsPrecs = gainsPrec->text().toInt();
    int gainsTot = gainsPrecs +IA->getProfilage()->nbJetonsGagnesIAQuiProfile;
    resultatsGlobaux.setItem(ligne,TOTAL_GAINS,new QTableWidgetItem(QString::number(gainsTot)) );
    //Nb parties gagnées
    if(IA->getProfilage()->partieGagneeIAQuiProfile==1){
        int nbPartiesGagneesPrec=resultatsGlobaux.takeItem(ligne,NB_PARTIES_GAGNEES)->text().toInt();
        int nbPartiesTot=nbPartiesGagneesPrec+IA->getProfilage()->partieGagneeIAQuiProfile;
        resultatsGlobaux.setItem(ligne,NB_PARTIES_GAGNEES, new QTableWidgetItem(QString::number(nbPartiesTot)));
    }
}

void ContenuFenetreIA::majCalibrageIAProfilee(){

   int agressiviteIAProfilee=static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0))->getCalibrage()->getAgressivite();
   int rationaliteIAProfilee=static_cast<IntelligenceArtificielle*>(jeu->getJoueur(0))->getCalibrage()->getRationalite();
   calibrageIAProfilee.setText("Calibrage IA profilée : "+QString::number(agressiviteIAProfilee)+"% d'agressivité et "+QString::number(rationaliteIAProfilee)+"% de rationalité");
}

void ContenuFenetreIA::ajouterLigne(){

    int nouvelleLigne = recapParties.rowCount();
    IntelligenceArtificielleProfilage *IA =static_cast<IntelligenceArtificielleProfilage*>(jeu->getJoueur(1));
    //Ajout d'une ligne:
    recapParties.insertRow(nouvelleLigne);

    recapParties.setItem(nouvelleLigne,SCENARIOS_EN_COURS,new QTableWidgetItem(QString::number(IA->getProfilage()->scenarioDeTest)));

    recapParties.setItem(nouvelleLigne,AGRESSIVITE_IA_QUI_PROFILE, new QTableWidgetItem(QString::number(IA->getCalibrage()->getAgressivite())));

    recapParties.setItem(nouvelleLigne,RATIONALITE_IA_QUI_PROFILE, new QTableWidgetItem(QString::number(IA->getCalibrage()->getRationalite())));

    recapParties.setItem(nouvelleLigne,AGRESSIVITE_DEDUITE, new QTableWidgetItem(QString::number(IA->getScenario().getProfilDeduitGlobal().getAgressivite())));

    recapParties.setItem(nouvelleLigne,RATIONALITE_DEDUITE, new QTableWidgetItem(QString::number(IA->getScenario().getProfilDeduitGlobal().getRationalite())));

    recapParties.setItem(nouvelleLigne, TAUX_SIMILARITE, new QTableWidgetItem(QString::number(IA->getScenario().getTauxSimilarite())));

    //recapParties.setItem(nouvelleLigne, JEU_AGRESSIF, new QTableWidgetItem(QString::number(IA->getProfilage()->jeuAgressif)));

    recapParties.setItem(nouvelleLigne, GAINS, new QTableWidgetItem(QString::number(IA->getProfilage()->nbJetonsGagnesIAQuiProfile)));

    recapParties.setItem(nouvelleLigne, GAIN_IA_QUI_PROFILE, new QTableWidgetItem(QString::number(IA->getProfilage()->partieGagneeIAQuiProfile)));
}

void ContenuFenetreIA::scrollAutomatiqueTableau(){
    QScrollBar *sb = recapParties.verticalScrollBar();
    sb->setValue(sb->maximum());
}

void ContenuFenetreIA::majGraphiqueResultats(){

    pointsNombreDeParties << recapParties.rowCount();
    pointsTauxSimilarite << resultatsGlobaux.item(0, SIMILARITE)->text().toFloat();
    pointsCumulGains << resultatsGlobaux.item(0, TOTAL_GAINS)->text().toInt();

    // pass data points to graphs:
    graphiqueResultats.graph(0)->setData(pointsNombreDeParties, pointsTauxSimilarite);
    graphiqueResultats.graph(1)->setData(pointsNombreDeParties, pointsCumulGains);

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    graphiqueResultats.graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    graphiqueResultats.graph(1)->rescaleAxes();
}

void ContenuFenetreIA::actualiser(){
    //Calibrage IA profilée
    majCalibrageIAProfilee();

    //Ajout ligne résultats
    ajouterLigne();

    //Résultats globaux
    majResultatsGlobaux();

    scrollAutomatiqueTableau();

    majGraphiqueResultats();

    qApp->processEvents();
}
