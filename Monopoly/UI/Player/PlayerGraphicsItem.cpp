
#include "PlayerGraphicsItem.h"
#include "Player/Player.h"
#include "qpainter.h"

#include <QtMath>

#include <Game/Game.h>

PlayerGraphicsItem::PlayerGraphicsItem(Player* player, QRectF squareRect, QGraphicsItem* parent)
    :   QGraphicsItem(parent),
    m_player{ player }
{
    const int pl_id = static_cast<int>(m_player->getColor());

    // Setup the transform
    int radius = qMin(squareRect.width() / 2, squareRect.height() / (Game::MAX_PLAYER_COUNT / 2));
    m_playerIconItem = new PlayerIconGraphicsItem{player, static_cast<qreal>(radius), this, true};

    qreal x, y;
    /* Every uneven player comes on the right-side */
    (pl_id % 2 == 0) ? x = 0 : x = squareRect.width() / 2;
    y = qFloor(pl_id / 2) * (squareRect.height() / (Game::MAX_PLAYER_COUNT / 2));

    setPos(x, y);
}

/* Paint the player icon */
void PlayerGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    m_playerIconItem->paint(painter, option, widget);
}
