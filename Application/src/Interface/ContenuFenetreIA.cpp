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

    boite->addWidget(&etatParties);
    boite->addWidget(&calibrageIAProfilee);
    boite->addWidget(&recapParties);
    boite->addWidget(&resultatsGlobaux);

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


void ContenuFenetreIA::initialisationResultatsGlobaux(){
    resultatsGlobaux.setColumnCount(6);

    QStringList header;
    header<<"Agressivité déduite"<<"Rationalité déduite"<<"Taux de similarité"<<"Nombre de jeux agressifs"
            <<"Total gains"<<"Nombre de parties gagnées";

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
    resultatsGlobaux.setItem(nouvelleLigne, NB_JEUX_AGRESSIFS, new QTableWidgetItem(QString::number(0)));
    resultatsGlobaux.setItem(nouvelleLigne,TOTAL_GAINS, new QTableWidgetItem(QString::number(0)));
    resultatsGlobaux.setItem(nouvelleLigne, NB_PARTIES_GAGNEES, new QTableWidgetItem(QString::number(0)));

        resultatsGlobaux.setFixedHeight(50);
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
    QTableWidgetItem* nbPrec=resultatsGlobaux.takeItem(ligne,NB_JEUX_AGRESSIFS);
    int nbPrecs = nbPrec->text().toInt();
    resultatsGlobaux.setItem(ligne,NB_JEUX_AGRESSIFS,new QTableWidgetItem(QString::number(nbPrecs+IA->getProfilage()->jeuAgressif)) );
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

    //Résultats globaux
    majResultatsGlobaux();

    scrollAutomatiqueTableau();

    qApp->processEvents();
}
