
#ifndef PLAYERGRAPHICSITEM_H
#define PLAYERGRAPHICSITEM_H


#include "Player/Player.h"
#include "PlayerIconGraphicsItem.h"
#include <QGraphicsItem>


class PlayerGraphicsItem : public QGraphicsItem
{
public:
    /* Constructors */
    PlayerGraphicsItem(Player* player, QRectF squareRect, QGraphicsItem* parent = nullptr);

    /* Getters */
    Player::Color getColor() const { return m_player->getColor(); }
    Player* getPlayer() const { return m_player; };

    /* Required QGraphicsItem overrides */
    QRectF boundingRect() const override { return m_playerIconItem->boundingRect(); }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    PlayerIconGraphicsItem* m_playerIconItem{};
    Player* m_player{};
};

#endif // PLAYERGRAPHICSITEM_H
