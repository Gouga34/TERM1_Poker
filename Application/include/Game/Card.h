#ifndef CARD_H
#define CARD_H

/**

Couleur  { Pique = 0, Coeur = 1, Trefle = 2, Carreau = 3 };
Rang { As = 1, 2, 3, 4, 5,  6,  7, 8,  9, 10, Valet = 11, Dame = 12, Roi = 13 };

**/

#include "../Constants.h"

namespace game {

    class Card
    {
        private:

            int m_id;

        public:

            Card(int rank, int color);
            virtual ~Card();

            virtual int getId() const;
            virtual int getColor() const;
            virtual int getRank() const;
    };
}

#endif
