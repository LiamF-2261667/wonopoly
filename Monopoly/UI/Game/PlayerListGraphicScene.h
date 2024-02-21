
#ifndef PLAYERLISTGRAPHICSCENE_H
#define PLAYERLISTGRAPHICSCENE_H

#include <QGraphicsScene>


class Player;
class PlayerListItemGraphicsItem;


class PlayerListGraphicScene : public QGraphicsScene
{
public:
    /* Constructors */
    PlayerListGraphicScene(QRect rect, QObject* parent = nullptr);

    /* Methods */
    void initializePlayerList(QVector<Player*>& players);

public slots:
    void removePlayer(Player* player);
    void setCurrentPlayer(Player* player);
    void updatePlayerBalance(Player* player, int difference);
    void addToJail(Player* player);
    void removeFromJail(Player* player);

private:
    const qreal M_MARGIN{ 7.5 };

    QVector<PlayerListItemGraphicsItem*> m_playersListItems;
};

#endif // PLAYERLISTGRAPHICSCENE_H
