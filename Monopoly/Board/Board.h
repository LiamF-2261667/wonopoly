/*
 * Author: Liam
*/

#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include <QObject>
#include "Squares/Square.h"
#include "Player/Player.h"
#include "Jail.h"

class Purchase;
class Jail;

class Board : public QObject
{
    Q_OBJECT

public:
    static const int TILES_PER_SIDE{ 11 };
    static const int SIDES{ 4 };
    static const int TILE_AMOUNT{ SIDES * (TILES_PER_SIDE - 1) };

    /* Constructors */
    Board() : QObject() { }
    ~Board();

    /* Getters */
    Square* getSquare(const int index) const;

    /* Methods */
    void initialize(QVector<Square*>& squares, Jail* jail);
    void doEffect(Player* player);
    void doPassingEffect(Player* player, int currPos, const int rollAmount); // Author: Jesse
    void doRollEffect(Player* player, const int rollAmount, const bool isDouble); // Author: Jesse
    void sendPlayerToJail(Player* player) { m_jail->addToJail(player); }

signals:
    void boardCreated(Jail* jail);
    void createdSquare(Square* square);

private:
    QVector<Square*> m_squares;
    Jail* m_jail;

};

#endif // BOARD_H
