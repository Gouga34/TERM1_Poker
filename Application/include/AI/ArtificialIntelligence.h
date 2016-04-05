#ifndef ARTIFICIALINTELLIGENCE_H
#define ARTIFICIALINTELLIGENCE_H

#include "../Jeu/Player.h"
#include "../Jeu/Jeu.h"
#include "Resolver.h"
#include "WinningChancesEstimator.h"
#include "../Profiling/Profile.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

namespace ai {

    class Resolver;
    class WinningChancesEstimator;

    class ArtificialIntelligence : public Player {

        protected:
            Resolver *m_resolver; //Cerveau de l'intelligence artificielle

        private:
            std::vector<WinningChancesEstimator*> m_winningChancesEstimators;

        public :
            /**
            *@action : Constructeur de la classe Intelligence Artificielle
            *@param  : Un boolean indiquant si le jouer est le dealer, sa cave de depart et sa position
            **/
            ArtificialIntelligence(bool isDealer, int tokens, int m_position);

            /**
            *@action : Constructeur de la classe Intelligence Artificielle par copie de la classe Joueur
            *@param  : L'instance de la classe Joueur utilisee pour la copie
            **/
            ArtificialIntelligence(const ArtificialIntelligence& player);

            /**
            *@action : Destructeur de la classe Intelligence Artificielle
            **/
            virtual ~ArtificialIntelligence();

            virtual bool isHumain() const;

            /**
             * @return Calibrage de l'IA
             */
            virtual profiling::Profile* getCalibration();

            /**
             * @action Modifie les valeurs de calibrage de l'IA
             * @param profil Profil de l'IA
             */
            virtual void setCalibration(profiling::Profile profile);

            /**
             * @brief Attribue un nouveau calibrage tiré aléatoirement pour l'IA
             */
            virtual void changeRandomlyCalibration();

            /**
             * @action Lance une estimation des chances de gain dans plusieurs threads simultanément
             * @param nbTests Nombre de tests à lancer au total
             * @param nbThreads Nombre de threads à lancer
             */
            virtual void launchWinningChancesEstimator(int testsNumber, int threadsNumber = 1);

            /**
             * @action Attend que l'ensemble des threads soit terminé
             */
            virtual void waitWinningChancesEstimatorResult();

            /**
            *@action : Pemet a l'IA de jouer
            **/
            virtual Action play();

    };
}

#endif //ARTIFICIALINTELLIGENCE_H
