/*
 * Author: Liam
*/

#ifndef STATIONCARD_H
#define STATIONCARD_H

#include "BuyableCard.h"



class StationCard : public BuyableCard
{
public:
    /* Constructors */
    StationCard(Money baseRent, const Money price, const Money mortgageCost, const int id, const bool isMortgaged)
        : BuyableCard(id, price, mortgageCost, isMortgaged), m_baseRent{ baseRent } { }

    /* Getters */
    Money getRent() const override { return isMortgaged() ? 0 : m_baseRent; }
    BuyableCard::TYPE getType() const override { return BuyableCard::TYPE::STATION; }
    QVector<ACTION> getActions(Player* user) const override; // Author: Jesse

    /* Methods */
    void executeAction(ACTION action) override; // Author: Jesse

private:
    Money m_baseRent{};
};

#endif // STATIONCARD_H
