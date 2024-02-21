/*
 * Author: Jesse
*/


#ifndef JAILSQUARE_H
#define JAILSQUARE_H

#include "Square.h"



class JailSquare : public Square
{
public:
    /* Constructors */
    JailSquare(const int pos = 0) : Square(pos) { }

    /* Getters */
    TYPE getType() const override { return TYPE::JAIL; }

    /* Methods */
    void doEffect(Player* player) override { }
};

#endif // JAILSQUARE_H
