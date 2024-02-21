
#include "StationSquare.h"

#include "Cards/BuyableCard.h"
#include "Player/Player.h"

/* Author: Jesse */
/* Start purchase or pay rent of station */
void StationSquare::doEffect(Player* player) {
    BuyableCard* card = getCard();
    Player* cardOwner = card->getOwner();

    // If nobody owns the card, open the shop window
    if (cardOwner == nullptr) {
        emit inEffect();
        player->inEffect(true);
        startPurchase(player);
    }

    // If someone already owns the station
    else {
        // If it's from the player himself, don't do anyting
        if (cardOwner == player) return;

        // Cost is decided based on how many stations the owner owns
        int count{ 0 };
        for (int i = 0; i < cardOwner->cardsOwned(); ++i) {
            if (cardOwner->getCard(i)->getType() == BuyableCard::TYPE::STATION)
                ++count;
        }

        player->pay(card->getRent() * count, cardOwner);
    }
}

