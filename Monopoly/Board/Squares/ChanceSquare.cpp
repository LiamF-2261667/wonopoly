
#include "ChanceSquare.h"
#include "Player/Player.h"

/* Author: Jesse */
ChanceSquare::ChanceSquare(QVector<ActionCard*>& pool, int pos)
    : Square(pos), m_pool{ pool }
{
    std::srand(std::time(nullptr));
}

/* Author: Jesse */
/* Give a random card to a player */
void ChanceSquare::doEffect(Player* player) {
    emit inEffect();
    player->inEffect(true);

    if (m_pool.size() != 0) {
        ActionCard* card = m_pool[ std::rand() % m_pool.size() ];
        card->doEffect(player);
    }
}
