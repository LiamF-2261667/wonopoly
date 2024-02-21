/*
 * Author: Jesse
*/


#ifndef STATIONSQUARE_H
#define STATIONSQUARE_H

#include "BuyableSquare.h"


class StationSquare : public BuyableSquare
{
public:
    /* Constructors */
    StationSquare(BuyableCard* card, Purchase* purchase, const int pos = 0) : BuyableSquare(card, purchase, pos) { }

    /* Getters */
    TYPE getType() const override { return TYPE::STATION; }

    /* Methods */
    void doEffect(Player* player) override;
};

#endif // STATIONSQUARE_H
