
#include "Dice.h"
#include <cstdlib>
#include <vector>
#include <ctime>

/* Author: Jesse */
Dice::Dice(const int count)
    : m_diceCount{ count }
{
    std::srand(std::time(nullptr));
    for (int i = 0; i < m_diceCount; ++i) {
        m_rollResult.push_back(0);
    }
}

/* Author: Jesse */
/* Returns the values of the rolls of all dice */
std::vector<int> Dice::roll() {

    for (int i = 0; i < m_diceCount; ++i) {
        m_rollResult[i] =  1 + std::rand() % 6;
    }

    return m_rollResult;
}

/* Author: Jesse */
/* Returns the sum of all dice rolls */
int Dice::getDiceRoll() const {
    int sum{ 0 };

    for (int i = 0; i < m_diceCount; ++i) {
        sum += m_rollResult[i];
    }

    // return sum;
    return sum;
}

/* Author: Liam */
/* Get if all the dice equal eachother */
bool Dice::isEqual() const {
    for (size_t i = 0; i < m_rollResult.size(); ++i) {
        for (size_t j = i + 1; j < m_rollResult.size(); ++j) {
            if (m_rollResult[i] != m_rollResult[j]) return false;
        }
    }

    return true;
}
