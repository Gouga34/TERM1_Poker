/*========================================================================
Nom: Profilage.cpp       Auteur: Manuel CHATAIGNER
Maj: 04/03/2015            Creation: 04/03/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Profilage.
=========================================================================*/

#include "../../include/Profiling/Profiling.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <iostream>
#include <QStringList>
#include <string>

namespace profiling {

    Profiling::Profiling(Profile *aiCalibration, Profile *profile)
        : m_aiProfile(aiCalibration), m_playerProfile(profile), m_partsNumber(0),
          m_numberProfilingGamesWon(0), m_numberProfilingGames(0), m_numberGameGamesWon(0),
          m_numberGamesGame(0), m_profilingEarnings(0), m_gameEarnings(0)
    {
        reset();

        m_currentProfilingPhase.newPhase(0);
        m_currentGamePhase.newPhase(0);
    }

    Profiling::~Profiling()
    {

    }

    void Profiling::reset()
    {
        int step = ETAPE_JEU::NB_ETAPES;

        m_stepParts[step].isFolded = false;
        m_stepParts[step].adversaryWinningProbabilities = 0;

        m_stepParts[step].aggressivenessRate = 0;
        m_stepParts[step].rationalityRate = 0;
        m_stepParts[step].bluffRate = 0;
        m_stepParts[step].passivityRate = 0;

        m_stepParts[step].callRate = 0;
        m_stepParts[step].checkRate = 0;
        m_stepParts[step].betRate = 0;

        m_stepParts[step].highestBet = 0;
        m_stepParts[step].totalBet = 0;

        m_stepParts[step].numberOfActions = 0;
    }

    void Profiling::save()
    {
       QString filename = QString::fromStdString(m_playerProfile->getPseudo()) + ".csv";
       QFile file(QString::fromStdString(DOSSIER_PROFILAGE_STATIQUE) + filename);

       if (m_playerProfile->getPseudo().compare("") != 0) {
           if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {

               bool alreadyPresent = false;

               //Vérif si le pseudo est déjà présent ds le fichier "pseudos.txt".
               QFile nicknamesFile(QString::fromStdString(FICHIER_PSEUDOS_PROFILAGE));
               if (!nicknamesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                   std::cerr << "Erreur lors de l'ouverture du fichier des pseudos !" << std::endl;
                   return;
               }

               QTextStream flux(&nicknamesFile);

               QString line;
               while (!flux.atEnd()) {
                   line = flux.readLine();
                   if (line.compare(QString::fromStdString(m_playerProfile->getPseudo())) == 0 || line.compare(QString::fromStdString("")) == 0) {
                       alreadyPresent = true;
                   }
                }

                nicknamesFile.close();

                //Si pas présent, ajout du pseudo dans le fichier
                if (!alreadyPresent) {
                   // Ouverture de la liste des pseudos
                   QFile nicknamesList(QString::fromStdString(FICHIER_PSEUDOS_PROFILAGE));
                   if (!nicknamesList.open(QIODevice::Append | QIODevice::Text)) {
                       std::cerr << "Erreur lors de l'ouverture du fichier des pseudos !" << std::endl;
                       return;
                   }

                   QTextStream out(&nicknamesList);
                   out << QString::fromStdString(m_playerProfile->getPseudo()) << endl;

                   nicknamesList.close();
                }
           }

           QTextStream out(&file);
           QString line;

           //ajout du nombre de parties et du nombre de parties gagnées par l'IA qui profile.
            if (file.size() == 0) {
                out << "Gain IA qui profile,Jetons gagnes IA qui profile,Scenarios de tests en cours,Jeu agressif,Agressivite IA qui profile, Rationalite IA qui profile,Chances de gain de l'IA profilee,"
                    << "Agressivite IA profilee,Rationalite IA profilee"/*,Bluff IA profilee,Passivite IA profilee*/ << ",Nombre de checks,"
                    << "Nombre de mises,Nombre de suivis,Total des mises de l'IA profilee,Mise la plus haute IA profilee,Joueur profile se couche" << endl;

                // Ajout du contenu de fin
                out << endl
                    << "Nombre de parties:,0" << endl
                    << "Nombre de parties gagnees par l'IA qui profile pendant profilage :,0,sur,0" << endl
                    << "Nombre de parties gagnees par l'IA qui profile pendant jeu :,0,sur,0" << endl
                    << "Gains de l'IA qui profile pendant profilage :,0" << endl
                    << "Gains de l'IA qui profile pendant jeu :,0" << endl;
            }

            // On va chercher les informations à la fin du fichier
            file.seek(0);
            line = file.readLine();

            // Tant qu'on est pas à la fin
            while (!line.isEmpty() && line != "\n") {
                line = file.readLine();
            }

            line = file.readLine();
            m_partsNumber = line.split(",").at(1).toInt();

            line = file.readLine();
            m_numberProfilingGamesWon = line.split(",").at(1).toInt();
            m_numberProfilingGames = line.split(",").at(3).toInt();

            line = file.readLine();
            m_numberGameGamesWon = line.split(",").at(1).toInt();
            m_numberGamesGame = line.split(",").at(3).toInt();

            line = file.readLine();
            m_profilingEarnings = line.split(",").at(1).toInt();

            line = file.readLine();
            m_gameEarnings = line.split(",").at(1).toInt();


            // On se replace au niveau du total
            file.seek(0);
            line = file.readLine();

            // Tant qu'on est pas à la fin
            while (!line.isEmpty() && line != "\n") {
                line = file.readLine();
            }

            // On efface la fin
            file.seek(file.pos() - 1);
            file.resize(file.pos());

            //Écriture de la ligne supplémentaire
            int i = ETAPE_JEU::NB_ETAPES;
            if (m_gameResultProfilingAIviewpoint == GAGNE) {
                out << 1 << ",";
            }
            else if (m_gameResultProfilingAIviewpoint == EGALITE) {
                out << 2 << ",";
            }
            else {
                out << 0 << ",";
            }

            out << m_numberTokensWonProfilingAI << ","
                << m_testScenario << ","
                << m_roughPlay << ","
                << m_aiProfile->getAggressiveness() << ","
                << m_aiProfile->getRationality() << ","
                << m_stepParts[i].adversaryWinningProbabilities << ","
                << m_stepParts[i].aggressivenessRate << ","
                << m_stepParts[i].rationalityRate << ","
                << m_stepParts[i].checkRate << ","
                << m_stepParts[i].betRate << ","
                << m_stepParts[i].callRate << ","
                << m_stepParts[i].totalBet << ","
                << m_stepParts[i].highestBet << ","
                << m_stepParts[i].isFolded << endl;

            // On écrit le total
            m_partsNumber++;
            if (m_testScenario) {
                m_numberProfilingGames++;

                if (m_gameResultProfilingAIviewpoint == GAGNE) {
                    m_numberProfilingGamesWon++;
                }

                m_profilingEarnings += m_numberTokensWonProfilingAI;
            }
            else {
                m_numberGamesGame++;

                if (m_gameResultProfilingAIviewpoint == GAGNE) {
                    m_numberGameGamesWon++;
                }

                m_gameEarnings += m_numberTokensWonProfilingAI;
            }

            out << endl
                << "Nombre de parties:," << m_partsNumber << endl
                << "Nombre de parties gagnees par l'IA qui profile pendant profilage :," << m_numberProfilingGamesWon << ",sur," << m_numberProfilingGames << endl
                << "Nombre de parties gagnees par l'IA qui profile pendant jeu :," << m_numberGameGamesWon << ",sur," << m_numberGamesGame << endl
                << "Gains de l'IA qui profile pendant profilage :," << m_profilingEarnings << endl
                << "Gains de l'IA qui profile pendant jeu :," << m_gameEarnings << endl;

            file.close();
        }
    }
}
