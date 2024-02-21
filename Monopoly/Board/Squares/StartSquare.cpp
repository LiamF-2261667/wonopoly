
#include "StartSquare.h"
#include "Player/Player.h"

/* Author: Jesse */
/* Give a player the money you gain from landing on the start square */
void StartSquare::doEffect(Player* player) {
    player->gain(M_MONEY_GAIN);
}
