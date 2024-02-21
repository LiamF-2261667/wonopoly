
#include "Board.h"

#include "Squares/CompanySquare.h"

#include <Board/Squares/FreeSquare.h>
#include <Board/Squares/IncomeTaxSquare.h>
#include <Board/Squares/JailSquare.h>
#include <Board/Squares/PoliceSquare.h>
#include <Board/Squares/StationSquare.h>

#include "Jail.h"

#include <Cards/CompanyCard.h>
#include <Cards/StationCard.h>

/* Author: Liam */
Board::~Board() {
    for (int i = 0; i < m_squares.size(); ++i) {
        delete m_squares[i];
    }

    delete m_jail;
}

/* Author: Liam */
void Board::initialize(QVector<Square*>& squares, Jail* jail) {
    m_squares = squares;
    m_jail = jail;

    // Initialize all the squares
    for (int i = 0; i < m_squares.size(); ++i) {
        emit createdSquare(m_squares[i]);
    }

    emit boardCreated(m_jail);
}

/* Author: Liam */
/* Execute effect of the square a player is standing on */
void Board::doEffect(Player* player) {
    Square* activeSquare = m_squares[player->getPosition()];

    // Active the effect of the square
    activeSquare->doEffect(player);

    // If the player is standing on a policy square, send him to jail
    if (activeSquare->getType() == Square::TYPE::POLICE)
        sendPlayerToJail(player);
}

/* Author: Jesse */
/* Execute effect from passing a square when moving */
void Board::doPassingEffect(Player* player, int currPos, const int rollAmount) {
    Square* passingSquare{ nullptr };

    for (int i = 0; i < rollAmount - 1; ++i) {
        currPos = (currPos + 1) % TILE_AMOUNT;
        passingSquare = {m_squares[currPos]};

        if (passingSquare->getType() == Square::TYPE::START)
            passingSquare->doEffect(player);
    }
}

/* Author: Jesse */
/* Execute effect from rolling on a square */
void Board::doRollEffect(Player* player, const int rollAmount, const bool isDouble) {
    Square* square = m_squares[player->getPosition()];

    // If in Company square effect
    if (square->getType() == Square::TYPE::COMPANY)
        (static_cast<CompanySquare*>(square))->executeEffectWithDice(rollAmount, player);

    if (square->getType() == Square::TYPE::JAIL) {
        m_jail->checkRoll(player, rollAmount, isDouble);
        square->doEffect(player);
    }

}

/* Author: Liam */
/* Returns a square from a given index */
Square* Board::getSquare(int index) const {
    if (index < TILE_AMOUNT)
        return m_squares[index];

    return nullptr;
}
