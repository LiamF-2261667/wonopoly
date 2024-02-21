
#include "PlayerListGraphicScene.h"

#include "Player/Player.h"
#include "UI/Game/PlayerListItemGraphicsItem.h"

#include <Game/Game.h>

#include <QMessageBox>

PlayerListGraphicScene::PlayerListGraphicScene(QRect rect, QObject* parent)
    : QGraphicsScene(parent)
{
    setSceneRect(rect);
}

void PlayerListGraphicScene::initializePlayerList(QVector<Player*>& players) {
    QRectF rect{ sceneRect() };

    // Calculate size per playerListItem
    QRectF workArea{rect.x(), rect.y(),
                   rect.width() - M_MARGIN,
                   rect.height() - M_MARGIN};

    QRectF playerListItemArea{rect.x(), rect.y(),
                             workArea.width(),
                             workArea.height()/Game::MAX_PLAYER_COUNT - M_MARGIN/2};

    // Add a playerListItem for each player
    for (int i = 0; i < players.size(); ++i) {
        PlayerListItemGraphicsItem* newPlayerListItem = new PlayerListItemGraphicsItem{playerListItemArea, players[i]};
        addItem(newPlayerListItem);

        // Set the position in the list
        newPlayerListItem->setPos(M_MARGIN/2,
                                  i * (workArea.height()/Game::MAX_PLAYER_COUNT + M_MARGIN/8) + M_MARGIN/2);

        m_playersListItems += newPlayerListItem;
    }
}

/* Remove a player from the list after going bankrupt */
void PlayerListGraphicScene::removePlayer(Player* player) {
    for (int i = 0; i < m_playersListItems.size(); ++i) {
        if (m_playersListItems[i]->getPlayer() == player) {
            delete m_playersListItems[i];
            m_playersListItems.erase(m_playersListItems.begin() + i);
            // Shift the positions of the players below upwards ?
        }
    }
}

/* Set the current player inside the player list */
void PlayerListGraphicScene::setCurrentPlayer(Player* player) {
    for (int i = 0; i < m_playersListItems.size(); ++i) {
        if (m_playersListItems[i]->getPlayer() == player) {
            m_playersListItems[i]->setCurrentPlayer();
        } else {
            m_playersListItems[i]->setCurrentPlayer(false);
        }
    }
}

/* Set a player to jail inside the player list */
void PlayerListGraphicScene::addToJail(Player* player) {
    for (int i = 0; i < m_playersListItems.size(); ++i) {
        if (m_playersListItems[i]->getPlayer() == player) {
            // Set the playerListItem in jail
            m_playersListItems[i]->setInJail();

            // Notify the player that he's been send to jail
            QMessageBox msgBox;
            msgBox.setText("Player " + QString::number(static_cast<int>(player->getColor()) + 1) + " has been send to jail!");
            msgBox.exec();
        }
    }
}

/* Remove a player from jail inside the player list */
void PlayerListGraphicScene::removeFromJail(Player* player) {
    for (int i = 0; i < m_playersListItems.size(); ++i) {
        if (m_playersListItems[i]->getPlayer() == player) {
            m_playersListItems[i]->setInJail(false);
        }
    }
}

/* Update the balance of a specific player */
void PlayerListGraphicScene::updatePlayerBalance(Player* player, int difference) {
    for (int i = 0; i < m_playersListItems.size(); ++i) {
        if (m_playersListItems[i]->getPlayer() == player) {
            m_playersListItems[i]->updateBalance(difference);
            return;
        }
    }
}
