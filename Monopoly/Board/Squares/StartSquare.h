/*
 * Author: Liam
*/

#ifndef STARTSQUARE_H
#define STARTSQUARE_H


#include "Square.h"
#include "Player/Money.h"


class StartSquare : public Square
{
public:
    /* Constructors */
    StartSquare(const Money moneyGain, const int pos = 0) : Square(pos), M_MONEY_GAIN{ moneyGain } { }

    /* Getters */
    TYPE getType() const override { return TYPE::START; }

    /* Methods */
    void doEffect(Player* player) override;

private:
    const Money M_MONEY_GAIN;
};

#endif // STARTSQUARE_H
