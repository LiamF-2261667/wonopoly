/*
 * Author: Liam
*/

#ifndef FREESQUARE_H
#define FREESQUARE_H


#include "Square.h"

class FreeSquare : public Square
{
public:
    /* Constructors */
    FreeSquare(const int pos = 0) : Square(pos) { }

    /* Getters */
    TYPE getType() const override { return TYPE::FREE; }

    /* Methods */
    void doEffect(Player* player) override { }
};

#endif // FREESQUARE_H
