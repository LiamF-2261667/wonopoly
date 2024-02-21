/*
 * Author: Jesse
*/

#include "Trade.h"
#include "Cards/BuyableCard.h"
#include "Cards/StreetCard.h"

/* Exchange items upon confirming the trade */
void Trade::start(QPair<Player*, Player*>& traders,
                  QPair<QVector<BuyableCard*>, QVector<BuyableCard*>>& cards,
                  QPair<Money, Money>& money,
                  QPair<int, int>& jailCards)
{
    // Initialize players
    m_players = traders;

    // Check if cards exchanged are viable
    if (!checkCardViability(cards.first)) return;
    if (!checkCardViability(cards.second)) return;

    // Exchange items
    exchangeCards(cards, jailCards);
    exchangeMoney(money);

    emit tradeSuccessful();
}

// Check whether the cards selected for trading are allowed to be traded
bool Trade::checkCardViability(const QVector<BuyableCard*>& cards) const {
    // First, check if they are trying to trade a property
    QVector<StreetCard*> cardsToCheck{};
    for (int i = 0; i < cards.size(); ++i) {
        if (cards[i]->getType() == BuyableCard::TYPE::STREET)
            cardsToCheck.push_back(static_cast<StreetCard*>(cards[i]));
    }

    // Then, check if these properties are tradable
    for (int i = 0; i < cardsToCheck.size(); ++i) {
        if (!cardsToCheck[i]->canTrade(cardsToCheck)) {
            emit tradeFailed("Properties with houses on them may not be traded unless the full set is traded!");
            return false;
        }
    }

    return true;
}

// Exchange money between players
void Trade::exchangeMoney(QPair<Money, Money>& money) {
    Money payment = money.first - money.second;
    m_players.first->pay(payment, m_players.second);
}

// Exchange cards between players
void Trade::exchangeCards(QPair<QVector<BuyableCard*>, QVector<BuyableCard*>>& cards, QPair<int, int>& jailCards) {
    // Cards
    for (BuyableCard* card : cards.second) {
        m_players.first->addCard(card);
    }
    
    for (BuyableCard* card : cards.first) {
        m_players.second->addCard(card);
    }

    // Get out of jail cards
    for (int i = 0; i < jailCards.first; ++i) {
        m_players.first->removeJailCard();
        m_players.second->addJailCard();
    }

    for (int i = 0; i < jailCards.second; ++i) {
        m_players.second->removeJailCard();
        m_players.first->addJailCard();
    }
}
