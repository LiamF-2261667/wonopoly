
#include "CompanyCard.h"
#include "Player/Player.h"

/* Author: Liam */
/* Decide the rent a player should pay */
Money CompanyCard::getRent() const {
    if (isMortgaged()) return 0;

    // Count the amount of comany cards the owner owns
    int amountOfCompanyCards{};
    for (int i = 0; i < m_owner->cardsOwned(); ++i) {
        if (m_owner->getCard(i)->getType() == TYPE::COMPANY)
            amountOfCompanyCards++;
    }

    // If there are more company cards owned then the given rentPrices, return the last
    if (amountOfCompanyCards >= m_rents.size())
        return m_rents.back();

    return m_rents[amountOfCompanyCards - 1];
}

/* Author: Jesse */
/* Returns all actions the user is able to take */
QVector<BuyableCard::ACTION> CompanyCard::getActions(Player* user) const {
    QVector<BuyableCard::ACTION> cardActions{};

    if (user != m_owner) return cardActions;

    if (!isMortgaged())
        cardActions.push_back(BuyableCard::ACTION::SELL);

    if (isMortgaged() && m_owner->canAfford(isMortgaged()))
        cardActions.push_back(BuyableCard::ACTION::UNMORTGAGE);
    else if (!isMortgaged())
        cardActions.push_back(BuyableCard::ACTION::MORTGAGE);

    return cardActions;
}

/* Author: Jesse */
/* Executes the given action */
void CompanyCard::executeAction(BuyableCard::ACTION action) {
    if (action == BuyableCard::ACTION::SELL) sell();

    if (action == BuyableCard::ACTION::MORTGAGE) mortgage();
    if (action == BuyableCard::ACTION::UNMORTGAGE) mortgage(false);

    emit cardActionPerformed();
}
