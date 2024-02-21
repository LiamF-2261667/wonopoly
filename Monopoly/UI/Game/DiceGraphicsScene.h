
#ifndef DICEGRAPHICSSCENE_H
#define DICEGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


class Dice;
class Game;


class DiceGraphicsScene : public QGraphicsScene
{
public:
    /* Constructors */
    explicit DiceGraphicsScene(const QRectF& rect, Dice* dice, Game* game, QObject *parent = nullptr);

    void setEnabled(const bool enabled = true);

private:
    const QString M_PATH { "://Images/dice_" };
    const QColor M_ENABLED_COLOR{ 149, 201, 93 };
    const QColor M_DISABLED_COLOR{ 204, 204, 204 };
    const QColor M_OPAQUE{ 0, 0, 0, 0 };

    Dice* m_dice{};
    std::vector<QGraphicsPixmapItem*> m_diceItems{};

    QGraphicsRectItem* m_backgroundItem{};

    Game* m_game{};

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // DICEGRAPHICSSCENE_H
