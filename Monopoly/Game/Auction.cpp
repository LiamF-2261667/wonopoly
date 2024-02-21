
#include "Auction.h"
#include "Player/Player.h"
#include "Cards/BuyableCard.h"
#include <iostream>

/* Author: Liam */
/* Start the bidding proces */
void Auction::startAuction(BuyableCard* card) {
    m_highestBid =  QPair<Player*, Money>(nullptr, M_STARTING_PRICE - 1);
    m_currentBidderIndex = 0;

    while (m_players[m_currentBidderIndex]->getMoney() < M_STARTING_PRICE) {
        ++m_currentBidderIndex;

        if (m_currentBidderIndex == m_players.size()) {
            card->changeOwner(nullptr);
            endAuction();
            return;
        }
    }

    m_card = card;

    emit startingAuction(this);
}

/* Author: Liam */
/* Place a bid */
void Auction::bid(const int amount) {
    if (amount > m_highestBid.second) {
        m_highestBid.second = amount;
        m_highestBid.first = m_players[m_currentBidderIndex];
    }

    nextBidder();
}

/* Author: Liam */
/* Go to the next bidder */
void Auction::nextBidder() {
    // Calculate the next index
    m_currentBidderIndex = (m_currentBidderIndex + 1) % m_players.size();

    // If current bidder already has the highest bid, end the bidding proces
    if (m_players[m_currentBidderIndex] == m_highestBid.first) {
        buyForBestBid();
        endAuction();
        return;
    }

    // If the bidding has reached the first player again and nobody has bidden, end the bidding proces
    if (m_currentBidderIndex == 0 && m_highestBid.first == nullptr) {
        endAuction();
        return;
    }

    // If the new bidder doesn't have enough money to bid anymore, go to the next bidder
    if (m_players[m_currentBidderIndex]->getMoney() <= m_highestBid.second) {
        nextBidder();
        return;
    }

    emit nextBid();
}

/* Author: Liam */
/* Buy the card for the highest bidder */
void Auction::buyForBestBid() {
    m_highestBid.first->pay(m_highestBid.second);
    m_highestBid.first->addCard(m_card);
}

/* Author: Jesse */
/* Starts the auction of multiple items in a row */
void Auction::startMassAuction(QVector<BuyableCard*>& cards) {
    m_hasMultipleItems = true;
    m_cards = cards;
    m_currentItemIndex = 0;

    startAuction(cards[0]);
}

/* Author: Jesse */
/* End process of an auction */
void Auction::endAuction() {

    if (!m_hasMultipleItems) {
        emit endingAuction();
        return;
    }

    /* Mass auction functionality */
    if (m_currentItemIndex == m_cards.size() - 1) {
        m_hasMultipleItems = false;
        emit endingAuction();
        return;
    }

    ++m_currentItemIndex;
    startAuction(m_cards[m_currentItemIndex]);

}
