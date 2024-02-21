
#include "Game/Dice.h"
#include "Game/Game.h"
#include "DiceGraphicsScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>

DiceGraphicsScene::DiceGraphicsScene(const QRectF& rect, Dice* dice, Game* game, QObject* parent)
    : QGraphicsScene(parent),
    m_dice{ dice },
    m_game{ game }
{
    // Dice images taken from https://icons8.com/

    QGraphicsPixmapItem* diceItem{};

    // Setting the dice images
    for (int i = 0; i < m_dice->getDiceCount(); ++i) {
        QPixmap texture{ M_PATH + "1.png" };

        /* Add items to a list */
        m_diceItems.push_back( new QGraphicsPixmapItem( texture ) );
        diceItem = m_diceItems.back();

        /* Scale the dice */
        diceItem->setTransformationMode(Qt::SmoothTransformation);
        diceItem->setScale(1.5);

        // Setting the position
        diceItem->setPos(65 + i * m_dice->getDiceCount() + i * 120, 0);

        addItem( diceItem );
    }

    // Setting up the background
    m_backgroundItem = new QGraphicsRectItem{ rect.x(), rect.y(), rect.width() - 2, rect.height() - 3 };
    m_backgroundItem->setPen(M_OPAQUE);
    m_backgroundItem->setBrush(M_ENABLED_COLOR);
    m_backgroundItem->setZValue(-1);
    addItem(m_backgroundItem);
}

/* Change the color + interactability of the scene */
void DiceGraphicsScene::setEnabled(const bool enabled) {
    if (enabled)
        m_backgroundItem->setBrush(M_ENABLED_COLOR);
    else
        m_backgroundItem->setBrush(M_DISABLED_COLOR);
}

/* Rolls the dice upon pressing the scene */
void DiceGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    std::vector<int> roll = m_dice->roll();

    /* Update textures */
    for (int i = 0; i < m_dice->getDiceCount(); ++i) {
        m_diceItems[i]->setPixmap( QPixmap(M_PATH + QChar(int('0') + roll[i])) );
    }

    m_game->applyRoll();

    QGraphicsScene::mouseReleaseEvent(event);
}
