/*
 * Author: Liam
*/

#ifndef INCOMETAXSQUARE_H
#define INCOMETAXSQUARE_H


#include "Player/Money.h"
#include "Square.h"


class IncomeTaxSquare : public Square
{
public:
    /* Constructors */
    IncomeTaxSquare(const Money tax, const int pos = 0) : Square(pos), m_tax{ tax } { }

    /* Getters */
    TYPE getType() const override { return TYPE::INCOME_TAX; }

    /* Methods */
    void doEffect(Player* player) override;

private:
    const Money m_tax;
};

#endif // INCOMETAXSQUARE_H
