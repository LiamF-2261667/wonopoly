/*
 * Author: Jesse
*/



#ifndef POLICESQUARE_H
#define POLICESQUARE_H

#include "Square.h"



class PoliceSquare : public Square
{
public:
    /* Constructors */
    PoliceSquare(const int pos = 0) : Square(pos) { }

    /* Getters */
    TYPE getType() const override { return TYPE::POLICE; }

    /* Methods */
    void doEffect(Player* player) override { }
};

#endif // POLICESQUARE_H
