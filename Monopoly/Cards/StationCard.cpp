
#include "StationCard.h"
#include "Player/Player.h"

/* Author: Jesse */
/* Returns all actions the user is able to take */
QVector<BuyableCard::ACTION> StationCard::getActions(Player* user) const {
    QVector<BuyableCard::ACTION> cardActions{};

    if (user != m_owner) return cardActions;

    if (!isMortgaged())
        cardActions.push_back(BuyableCard::ACTION::SELL);

    if (isMortgaged() && m_owner->canAfford(getMortgagePrice()))
        cardActions.push_back(BuyableCard::ACTION::UNMORTGAGE);
    else if (!isMortgaged())
        cardActions.push_back(BuyableCard::ACTION::MORTGAGE);

    return cardActions;
}

/* Author: Jesse */
/* Executes the given action */
void StationCard::executeAction(BuyableCard::ACTION action) {
    if (action == BuyableCard::ACTION::SELL) sell();

    if (action == BuyableCard::ACTION::MORTGAGE) mortgage();
    if (action == BuyableCard::ACTION::UNMORTGAGE) mortgage(false);

    emit cardActionPerformed();
}
