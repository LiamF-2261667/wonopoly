
#include "CompanySquare.h"

#include "Cards/BuyableCard.h"
#include "Player/Player.h"

/* Author: Liam */
/* Start purchase or pay rent of this square */
void CompanySquare::doEffect(Player* player) {
    Player* cardOwner = getCard()->getOwner();

    player->inEffect(true);

    // If nobody owns the card, open the shop window
    if (cardOwner == nullptr) {
        emit inEffect();
        startPurchase(player);
    }

    // If someone already owns the company
    else {
        // If it's from the player himself, don't do anyting
        if (cardOwner == player) return;
    }
}

/* Author: Liam */
/* Pay the rent with a thrown amount (of the dice) */
void CompanySquare::executeEffectWithDice(const int diceSum, Player* player) {
    // Pay the money
    Money moneyPerDice = getCard()->getRent();
    player->pay(moneyPerDice * diceSum, getCard()->getOwner());

    player->inEffect(false);

}
