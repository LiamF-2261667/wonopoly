
#include "StreetSquare.h"
#include "Cards/BuyableCard.h"
#include "Player/Player.h"

/* Author: Jesse */
/* Start purchase or pay rent to owner */
void StreetSquare::doEffect(Player* player) {
    BuyableCard* card = getCard();
    Player* cardOwner = card->getOwner();

    // If nobody owns the card, open the shop window
    if (cardOwner == nullptr) {
        emit inEffect();
        player->inEffect(true);
        startPurchase(player);
    }

    // If someone already owns the street
    else {
        // If it's from the player himself, don't do anyting
        if (cardOwner == player) return;

        player->pay(card->getRent(), cardOwner);
    }
}
