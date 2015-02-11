#ifndef CARTEGRAPHIQUE_H
#define CARTEGRAPHIQUE_H

#include <QLabel>

class CarteGraphique : public QLabel
{
    private:

        static const unsigned int LARGEUR_CARTE;
        static const unsigned int HAUTEUR_CARTE;


        /*
        enum eSuit  { Spades = 0, Hearts = 1, Clubs = 2, Diamonds = 3 };
        enum eValue { Ace = 1, Two = 2, Three = 3, Four = 4, Five = 5, Six = 6, Seven = 7,
                      Eight = 8, Nine = 9, Ten = 10, Jack = 11, Queen = 12, King = 13 };
        */
        inline int MAKE_CARD(int Value, int Suit)
        {
            if(Value == 0)   return 0;
            if(Value >  13) Value = 13;

            if(Suit < 0)    Suit = 0;
            if(Suit > 3)    Suit = 3;

            return ( (Suit * 13) + (Value - 1) );
        }

    public:

        CarteGraphique(QPixmap p, int rang, int couleur);
        //CarteGraphique(Carte c);
        ~CarteGraphique();
};

#endif // CARTEGRAPHIQUE_H
