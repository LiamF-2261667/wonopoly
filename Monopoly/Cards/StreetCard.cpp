/*
 * Author: Jesse
*/

#include "StreetCard.h"
#include "Player/Player.h"

/* Decide the price a player should pay (takes into account building) */
Money StreetCard::getPrice() const {
    if (getOwner() == nullptr)
        return m_price;

    if (m_tier == BUILD_TIER::HOTEL)
        return m_hotelCost;

    return m_houseCost;
}

/* Decide the sell value of a property (takes into account downgrading) */
Money StreetCard::getSellValue() const {
    if (m_tier == BUILD_TIER::HOTEL)
        return (int)(m_hotelCost / 2);

    if (m_tier != BUILD_TIER::NO_HOUSE)
        return (int)(m_houseCost / 2);

    return (int)(m_price / 2);
}

/* Returns the amount of money that can be gained from selling all assets and this property */
Money StreetCard::getTotalValue() const {
    Money total{ 0 };

    // Calculate downgrade value
    if (m_tier == BUILD_TIER::HOTEL) {
        total += ((int)m_tier - 1) * (int)(m_houseCost / 2);
        total += (int)(m_hotelCost / 2);
    }
    else {
        total += (int)m_tier * (int)(m_houseCost / 2);
    }

    // Add property value
    total += (int)(m_price / 2);

    return total;
}

/* Decide the rent a player should pay */
Money StreetCard::getRent() const {
    if (isMortgaged()) return 0;

    Money result = m_rentCosts[(int)m_tier];

    if (m_tier == BUILD_TIER::NO_HOUSE) {
        for (BuyableCard* card : m_set) {
            if (card->getOwner() != m_owner)
                return result;
        }

        // If all cards of set are owned by same player & no house is built here, double rent
        return result * 2;
    }

    return result;
}

/* Checks whether a user can build on this property */
bool StreetCard::canBuild() const {
    if (m_tier == BUILD_TIER::HOTEL) return false;

    // Check if the building is valid with the other streets in the set
    for (int i = 0; i < m_set.size(); ++i) {
        if (m_set[i]->getOwner() != m_owner) return false;
        if (m_set[i]->isMortgaged()) return false;
        if ((int)m_set[i]->getTier() + 1 == (int)m_tier) return false;
    }

    // Check if the owner has enough to pay for the upgrade
    if (m_tier == BUILD_TIER::HOUSE_4) {
        if (!m_owner->canAfford(m_hotelCost)) return false;
    }
    else if (!m_owner->canAfford(m_houseCost)) return false;

    return true;
}

/* Checks whether a user can sell this property */
bool StreetCard::canSell() const {
    if (isMortgaged()) return false;
    if (m_tier != BUILD_TIER::NO_HOUSE) return false;

    for (int i = 0; i < m_set.size(); ++i) {
        if (m_set[i]->getTier() != BUILD_TIER::NO_HOUSE) return false;
    }

    return true;
}

/* Checks whether or not you can sell a house on this property */
bool StreetCard::canDowngrade() const {
    if (m_tier == BUILD_TIER::NO_HOUSE) return false;

    for (int i = 0; i < m_set.size(); ++i) {
        if ((int)m_set[i]->getTier() - 1 == (int)m_tier) return false;
    }

    return true;
}

/* Checks whether or not you can sell this property */
bool StreetCard::canTrade(QVector<StreetCard*>& cards) const {
    /* Check whether this set has any houses */
    bool setHasHouses{ false };
    for (int i = 0; i < m_set.size(); ++i) {
        if (m_set[i]->getTier() != BUILD_TIER::NO_HOUSE)
            setHasHouses = true;
    }

    if (!setHasHouses) return true;

    /* Check if the traded cards include the entire set */
    int count{ 0 };
    for (int i = 0; i < cards.size(); ++i) {      
        if (m_set.contains(cards[i]))
            count++;
    }

    if (count == m_set.size()) return true;

    return false;
}

/* Builds a house on this property */
void StreetCard::build() {
    m_owner->pay(getPrice());
    m_tier = BUILD_TIER((int)m_tier + 1);

    if (m_tier == BUILD_TIER::HOTEL) {
        m_owner->changeHouseAmount(-4);
        m_owner->changeHotelAmount(1);
    }
    else
        m_owner->changeHouseAmount(1);
}

/* Sells a house on this property */
void StreetCard::downgrade() {
    if (m_tier == BUILD_TIER::HOTEL) {
        m_owner->changeHouseAmount(4);
        m_owner->changeHotelAmount(-1);
    }
    else
        m_owner->changeHouseAmount(-1);

    m_owner->gain(getSellValue());
    m_tier = BUILD_TIER((int)m_tier - 1);
}

/* Returns all actions the user is able to take */
QVector<BuyableCard::ACTION> StreetCard::getActions(Player* user) const {
    QVector<BuyableCard::ACTION> cardActions{};

    if (user != m_owner) return cardActions;

    if (canSell()) cardActions.push_back(BuyableCard::ACTION::SELL);
    if (canBuild()) cardActions.push_back(BuyableCard::ACTION::BUILD);
    if (canDowngrade()) cardActions.push_back(BuyableCard::ACTION::DOWNGRADE);

    if (isMortgaged() && m_owner->canAfford(getMortgagePrice()))
        cardActions.push_back(BuyableCard::ACTION::UNMORTGAGE);
    else if (!isMortgaged() && canSell())
        cardActions.push_back(BuyableCard::ACTION::MORTGAGE);

    return cardActions;
}

/* Executes the given action */
void StreetCard::executeAction(BuyableCard::ACTION action) {
    if (action == BuyableCard::ACTION::SELL) sell();

    if (action == BuyableCard::ACTION::MORTGAGE) mortgage();
    if (action == BuyableCard::ACTION::UNMORTGAGE) mortgage(false);

    if (action == BuyableCard::ACTION::BUILD) build();

    if (action == BuyableCard::ACTION::DOWNGRADE) downgrade();

    emit cardActionPerformed();
}
