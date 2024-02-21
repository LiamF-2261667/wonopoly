#include "PlayerSelectionWindow.h"
#include "ui_PlayerSelectionWindow.h"

#include "Player/Player.h"
#include "UI/Player/PlayerIconGraphicsItem.h"

PlayerSelectionWindow::PlayerSelectionWindow(QVector<Player*>* players, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerSelectionWindow),
    m_players{players}
{
    ui->setupUi(this);
    setupUiLists();
}

PlayerSelectionWindow::~PlayerSelectionWindow()
{
    delete ui;
}


void PlayerSelectionWindow::call(int selectionAmount) {
    for (int i = 0; i < m_checkBoxes.size(); ++i) {
        connect(m_checkBoxes[i], &QCheckBox::clicked, this, &PlayerSelectionWindow::selected);
    }

    m_selectionAmount = selectionAmount;

    show();
}

/* Put all the boxes, ... from the ui into lists */
void PlayerSelectionWindow::setupUiLists() {
    QVector<QGroupBox*> boxes {
        ui->player1Box,
        ui->player2Box,
        ui->player3Box,
        ui->player4Box,
        ui->player5Box,
        ui->player6Box
    };

    QVector<QCheckBox*> checkBoxes {
        ui->Player1_button,
        ui->Player2_button,
        ui->Player3_button,
        ui->Player4_button,
        ui->Player5_button,
        ui->Player6_button
    };

    QVector<QGraphicsView*> views {
        ui->Player1_view,
        ui->Player2_view,
        ui->Player3_view,
        ui->Player4_view,
        ui->Player5_view,
        ui->Player6_view
    };

    QVector<QGraphicsScene*> scenes{};
    for (QGraphicsView* view : views) {
        QGraphicsScene* newScene = new QGraphicsScene{view};
        view->setScene(newScene);

        scenes += newScene;
    }

    // Hide the boxes for players that are not in the game
    for (int i = boxes.size() - 1; i >= m_players->size(); --i)
        boxes[i]->hide();

    // Setup the icons for the players that are
    for (int i = 0; i < m_players->size(); ++i) {
        PlayerIconGraphicsItem* icon = new PlayerIconGraphicsItem((*m_players)[i], ui->Player1_view->width() / 1.5);
        scenes[i]->addItem(icon);
        m_playerItems.push_back(icon);
    }

    m_groupBoxes = boxes;
    m_checkBoxes = checkBoxes;
    m_playerViews = views;
    m_scenes = scenes;
}

/* Return the selected players via an emit */
void PlayerSelectionWindow::selected() {
    QVector<Player*> playersSelected{};

    for (int i = 0; i < m_checkBoxes.size(); ++i) {
        if (m_checkBoxes[i]->checkState())
            playersSelected.push_back((*m_players)[i]);
    }

    if (playersSelected.size() >= m_selectionAmount) {
        for (int i = 0; i < m_checkBoxes.size(); ++i) {
            if (m_checkBoxes[i]->checkState())
                m_checkBoxes[i]->setCheckState(Qt::Unchecked);
        }
        close();

        emit selectedPlayers(playersSelected);
    };
}

/* Remove a player from the selection once they go bankrupt */
void PlayerSelectionWindow::removePlayer(Player* player) {
    int i{0};
    while (i < m_playerItems.size()) {
        if (m_playerItems[i]->getPlayer() == player) {
            m_groupBoxes[i]->hide();
            m_groupBoxes.erase(m_groupBoxes.begin() + i);
            m_checkBoxes.erase(m_checkBoxes.begin() + i);
            m_playerItems.removeAt(i);
        }
        else ++i;
    }
}
