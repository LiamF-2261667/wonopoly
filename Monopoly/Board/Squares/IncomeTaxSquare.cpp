
#include "IncomeTaxSquare.h"
#include "Player/Player.h"

/* Author: Liam */
/* Give a player the money you gain from landing on the start square */
void IncomeTaxSquare::doEffect(Player* player) {
    player->pay(m_tax);
}
