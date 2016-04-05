#include "../../include/Game/Card.h"


namespace game {

    Card::Card(int rank, int color)
    {
        if (rank < 1) {
            rank = 1;
        }
        else if (rank >  13) {
            rank = 13;
        }

        if (color < 0) {
            color = 0;
        }
        else if (color > 3) {
            color = 3;
        }

        m_id = ((color * 13) + (rank - 1));

    }

    Card::~Card()
    {

    }

    int Card::getId() const
    {
        return m_id;
    }

    int Card::getColor() const
    {
        return ((getId() - ((getId() % 13) + 1)) / 10);
    }

    int Card::getRank() const
    {
        return ((getId() % 13) + 1);
    }
}
