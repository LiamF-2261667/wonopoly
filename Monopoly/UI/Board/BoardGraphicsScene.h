
#ifndef BOARDGRAPHICSSCENE_H
#define BOARDGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "SquareGraphicsItem.h"
#include <vector>

class Board;
class Square;
class Player;
class PlayerGraphicsItem;
class PlayerSelectionWindow;

class BoardGraphicsScene : public QGraphicsScene
{
public:
    /* Constructors */
    BoardGraphicsScene(QRect rect, Board* board, QObject* parent = nullptr);

    /* Getters */
    SquareGraphicsItem* getSquare(int pos);

public slots:
    void addSquareItem(Square* square);
    void allignSquares();
    void addPlayer(Player* player);
    void movePlayer(Player* player, int toPos);
    void removePlayer(Player* player);

private:
    const QColor M_BACKGROUND_COLOR{ 89, 156, 92 };
    const QColor M_OPAQUE{ 0, 0, 0, 0 };

    Board* m_board{};
    std::vector<SquareGraphicsItem*> m_squareItemList{};

    QVector<PlayerGraphicsItem*> m_playerItemList{};

    QGraphicsRectItem* m_background{};
};

#endif // BOARDGRAPHICSSCENE_H
