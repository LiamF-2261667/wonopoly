
#include "BuyableCard.h"
#include "Player/Player.h"

/* Author: Liam */
/* Change the owner of the card */
void BuyableCard::changeOwner(Player* newOwner) {
    if (m_owner != nullptr) {
        m_owner->removeCard(this);
    }

    m_owner = newOwner;
}

/* Author: Liam */
/* Mortgage or remove mortgage of the this card */
void BuyableCard::mortgage(bool set) {
    if (set)
        m_owner->gain(m_mortgageCost);
    else
        m_owner->pay(m_mortgageCost);

    m_isMortgaged = set;
}

/* Author: Jesse */
/* Sell the this card */
void BuyableCard::sell() {
    m_owner->gain(getSellValue());
    m_owner->removeCard(this);
    m_owner = nullptr;
}
