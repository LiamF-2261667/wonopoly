
#ifndef PLAYERICONGRAPHICSITEM_H
#define PLAYERICONGRAPHICSITEM_H

#include <QGraphicsEllipseItem>

class Player;


class PlayerIconGraphicsItem : public QGraphicsEllipseItem
{
public:
    /* Constructors */
    PlayerIconGraphicsItem(const Player* player, qreal radius, QGraphicsItem* parent = nullptr, bool withSecondBorder = false);

    /* Qt overriden methods */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    /* Getters */
    const Player* getPlayer() const { return m_player; };

private:
    const QColor
        M_BLUE_COLOR      { 62 , 101, 230 },
        M_GREEN_COLOR     { 62 , 230, 90  },
        M_RED_COLOR       { 230, 62 , 62  },
        M_YELLOW_COLOR    { 222, 230, 62  },
        M_PURPLE_COLOR    { 129, 62 , 230 },
        M_ORANGE_COLOR    { 230, 118, 62  },
        M_OPAQUE          { 0, 0, 0, 0 };

    const qreal M_SECOND_BORDER_RATIO{ 1.2 };

    QGraphicsEllipseItem* m_secondBorder{};
    const Player* m_player;
};

#endif // PLAYERICONGRAPHICSITEM_H
