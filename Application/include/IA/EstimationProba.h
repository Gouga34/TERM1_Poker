#ifndef EstimationProba_h
#define EstimationProba_h

#include "../Jeu/Jeu.h"
#include "../Jeu/Joueur.h"
#include "../Assessor/Assessor.h"
#include <QThread>

class EstimationProba : public QThread {

    private:

        // Nombre de tests à effectuer
        int nbTests;

        // Résultat de l'estimation lancée dans le thread
        double resultatRestimation;

        Jeu *jeu;
        Joueur *joueur;


        /**
        *@action : Creer un nouveau deck prive des carte du joueur courant
        *@return  : Le deck des cartes restantes
        **/
       static  std::vector<game::Card> nouveauDeck(Joueur* joueurCourant);

       /**
       *@action : Melange le jeu de carte
       *@param  : Le jeu a melanger
       **/
       static void melange(std::vector<game::Card>& deck);

    protected:

        /**
         * @brief Lance l'estimation à partir des attributs dans un thread séparé
         */
        virtual void run();

    public:

        EstimationProba(int nombreTests = NOMBRE_DE_TESTS);

        EstimationProba(Jeu *j, Joueur *jo, int nombreTests = NOMBRE_DE_TESTS);

        double getResultat() const;

        void setNbTests(int tests);

        void setJeu(Jeu *j);

        void setJoueur(Joueur *jo);

		/**
		*@action : Permet de connaitre la probabilitee de gagner a un instant 't' pour le joueur courant
		*@return  : La probabilite de gagner
		**/
        static double estimation(Jeu* jeuCourant, Joueur* joueurCourant, int nombreTests = NOMBRE_DE_TESTS);
};


#endif
