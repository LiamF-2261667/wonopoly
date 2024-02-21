
#include "PlayerIconGraphicsItem.h"
#include "Player/Player.h"
#include "qbrush.h"
#include "qpen.h"
#include "qpainter.h"

PlayerIconGraphicsItem::PlayerIconGraphicsItem(const Player* player, qreal radius, QGraphicsItem* parent, bool withSecondBorder)
    : QGraphicsEllipseItem(0, 0, radius, radius, parent),
    m_player{ player }
{
    Player::Color pl_col = player->getColor();

    // Setup the correct player color
    if (pl_col == Player::Color::BLUE) setBrush(M_BLUE_COLOR);
    else if (pl_col == Player::Color::GREEN) setBrush(M_GREEN_COLOR);
    else if (pl_col == Player::Color::RED) setBrush(M_RED_COLOR);
    else if (pl_col == Player::Color::YELLOW) setBrush(M_YELLOW_COLOR);
    else if (pl_col == Player::Color::PURPLE) setBrush(M_PURPLE_COLOR);
    else if (pl_col == Player::Color::ORANGE) setBrush(M_ORANGE_COLOR);

    if (withSecondBorder) {
        // Setup the second border
        const qreal borderRadius = M_SECOND_BORDER_RATIO * radius;
        m_secondBorder = new QGraphicsEllipseItem{0, 0, borderRadius, borderRadius, parent};
        m_secondBorder->setPos((radius - borderRadius)/2, (radius - borderRadius)/2);
        m_secondBorder->setBrush(Qt::white);
        m_secondBorder->setPen(M_OPAQUE);

        setZValue(2);
    }
}

void PlayerIconGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

