
#include "Jail.h"
#include "Squares/JailSquare.h"
#include "Player/Player.h"

/* Author: Liam */
/* Move a player to the jail */
void Jail::addToJail(Player* player) {
    if (player->getPosition() != m_jailSquare->getPos() || !player->isInJail()) {
        player->goToJail();
        player->moveTo(m_jailSquare->getPos());

        emit addedPlayerToJail(player);
    }
}

/* Author: Liam */
/* Remove a given player from the jail */
void Jail::removeFromJail(Player* player) {
    player->exitJail();

    emit removedPlayerFromJail(player);
}

/* Author: Jesse */
/* Attempt to exit the jail by rolling the dice */
void Jail::attemptExit(Player* player) {
    player->tryExitJail();
    emit attemptedExit();
}

/* Author: Jesse */
/* Check if the player is allowed to leave jail */
void Jail::checkRoll(Player* player, const int rollAmount, const bool isDouble) {
    if (isDouble) {
        removeFromJail(player);
        player->goForward(rollAmount);
    }
    else if (player->exitAttempts() >= M_MAX_EXIT_ATTEMPTS) {
        removeFromJail(player);
        player->pay(M_EXIT_JAIL_COST);
        player->goForward(rollAmount);
    }
}
