/*
 * Author: Jesse
*/


#ifndef DICE_H
#define DICE_H

#include <vector>


class Dice
{
public:
    /* Constructors */
    Dice(const int count);

    /* Getters */
    int getDiceCount() const { return m_diceCount; };
    int getDiceRoll() const;

    /* Methods */
    bool isEqual() const; // Author: Liam
    std::vector<int> roll();

private:
    const int m_diceCount{};
    std::vector<int> m_rollResult{};
};

#endif // DICE_H
