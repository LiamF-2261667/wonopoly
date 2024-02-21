/*
 * Author: Liam
*/

#include "Purchase.h"

#include "Player/Player.h"
#include "Cards/BuyableCard.h"
#include "Auction.h"

/* Check if a player can buy the purchase */
bool Purchase::canBuy(Player* player) const {
    if (player == nullptr) player = m_currentBuyer;

    return player->getMoney() >= m_card->getPrice();
}

/* Buy the purchase for the card price */
void Purchase::buy(Player* player) {
    if (player == nullptr) player = m_currentBuyer;

    if (!canBuy(player)) return;

    player->pay(m_card->getPrice());
    player->addCard(m_card);

    emit endingPurchase();
}

/* Auction the purchase */
void Purchase::auction() {
    m_auction->startAuction(m_card);
    emit auctioningPurchase();
}
