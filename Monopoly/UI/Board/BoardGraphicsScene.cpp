
#include "BoardGraphicsScene.h"
#include "SquareGraphicsItem.h"
#include "Board/Board.h"
#include "UI/Player/PlayerGraphicsItem.h"
#include "UI/Menus/PlayerSelectionWindow.h"
#include "UI/Cards/CardActionsWindow.h"

#include <iostream>

BoardGraphicsScene::BoardGraphicsScene(QRect rect, Board* board, QObject* parent)
    : QGraphicsScene(parent), m_board{board}
{
    // Set the scene size
    setSceneRect(rect);

    // Add a background
    m_background = new QGraphicsRectItem{sceneRect()};
    m_background->setBrush(M_BACKGROUND_COLOR);
    m_background->setPen(M_OPAQUE);
    addItem(m_background);
}

SquareGraphicsItem* BoardGraphicsScene::getSquare(int pos) {
    if (pos >= m_squareItemList.size() || pos < 0)
        return nullptr;

    else return m_squareItemList[pos];
}

/* Create a new SquareGraphicsItems and store it in m_squareItemList */
void BoardGraphicsScene::addSquareItem(Square* square) {
    // Create a new square with the correct width and height
    qreal length = sceneRect().width() / Board::TILES_PER_SIDE;

    SquareGraphicsItem* squareItem{};
    squareItem = new SquareGraphicsItem{square, length, length};

    m_squareItemList.push_back(squareItem);
    addItem(squareItem);
}

/* Align all squares upon board getting initialised */
void BoardGraphicsScene::allignSquares() {
    qreal length = m_squareItemList[0]->rect().width();
    for (int i = 0; i < Board::TILES_PER_SIDE - 1; ++i) {
        /* Align bottom row */
        m_squareItemList[i]->setPos(sceneRect().width() - (i + 1) * length, sceneRect().width() - length);

        /* Align left row */
        m_squareItemList[i + Board::TILES_PER_SIDE - 1]->setPos(0, sceneRect().width() - length - i * length);

        /* Align top row */
        m_squareItemList[i + 2 * (Board::TILES_PER_SIDE - 1)]->setPos(i * length, 0);

        /* Align right row */
        m_squareItemList[i + 3 * (Board::TILES_PER_SIDE - 1)]->setPos(sceneRect().width() - length, i * length);
    }
}

/* Add a player graphic on the board */
void BoardGraphicsScene::addPlayer(Player* player) {
    SquareGraphicsItem* spawnSquare = m_squareItemList[player->getPosition()];

    m_playerItemList.append(new PlayerGraphicsItem{player, spawnSquare->rect(), spawnSquare});
}

/* Move a player graphic to a position */
void BoardGraphicsScene::movePlayer(Player* player, int toPos) {
    // Search for the player graphic
    PlayerGraphicsItem* playerGraphic{};
    for (int i = 0; i < m_playerItemList.size(); ++i) {
        if (m_playerItemList[i]->getColor() == player->getColor())
            playerGraphic = m_playerItemList[i];
    }

    if (playerGraphic == nullptr) {
        std::cerr << "Cannot find the player graphic\n";
        return;
    }

    // Get the square graphic
    SquareGraphicsItem* squareGraphic{ m_squareItemList[toPos] };

    // Move the player
    playerGraphic->setParentItem(squareGraphic);
}

/* Remove a player graphic from the board */
void BoardGraphicsScene::removePlayer(Player* player) {
    for (int i = 0; i < m_playerItemList.size(); ++i) {

        if (m_playerItemList[i]->getPlayer() == player) {
            delete m_playerItemList[i];
            m_playerItemList.erase(m_playerItemList.begin() + i);
        }
    }
}
