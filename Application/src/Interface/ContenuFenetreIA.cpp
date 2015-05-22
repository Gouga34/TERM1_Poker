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

ContenuFenetreIA::ContenuFenetreIA(Jeu *j):ContenuFenetre(j){
    //fenetre
    QVBoxLayout *boite = new QVBoxLayout;

    //Etat parties (en cours/ terminé):
    etatParties.setText("Parties en cours");

    //Tableau récapitulatif des parties
    initialisationRecapParties();

    boite->addWidget(&etatParties);
    boite->addWidget(&calibrageIAProfilee);
    boite->addWidget(&recapParties);

    setLayout(boite);
}

ContenuFenetreIA::~ContenuFenetreIA(){

}

void ContenuFenetreIA::initialisationRecapParties(){
    recapParties.setColumnCount(9);

    //Ajout première ligne:
    QStringList header;
    header<<"Scenarios en cours"<<"Agressivité IA qui profile"<<"Rationalité IA qui profile"
         <<"Agressivité déduite"<<"Rationalité déduite"<<"Taux de similarité"<<"Jeu agressif"<<"Gains"<<"Gain IA qui profile";
    recapParties.setHorizontalHeaderLabels(header);

    recapParties.resizeColumnsToContents();
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

    recapParties.setItem(nouvelleLigne, JEU_AGRESSIF, new QTableWidgetItem(QString::number(IA->getProfilage()->jeuAgressif)));

    recapParties.setItem(nouvelleLigne, GAINS, new QTableWidgetItem(QString::number(IA->getProfilage()->nbJetonsGagnesIAQuiProfile)));

    recapParties.setItem(nouvelleLigne, GAIN_IA_QUI_PROFILE, new QTableWidgetItem(QString::number(IA->getProfilage()->partieGagneeIAQuiProfile)));
}

void ContenuFenetreIA::scrollAutomatiqueTableau(){
    QScrollBar *sb = recapParties.verticalScrollBar();
    sb->setValue(sb->maximum());
}

void ContenuFenetreIA::actualiser(){
    //Calibrage IA profilée
    majCalibrageIAProfilee();

    //Ajout ligne résultats
    ajouterLigne();

    scrollAutomatiqueTableau();

    qApp->processEvents();
}
