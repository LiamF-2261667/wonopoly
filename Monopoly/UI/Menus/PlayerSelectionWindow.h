#ifndef PLAYERSELECTIONWINDOW_H
#define PLAYERSELECTIONWINDOW_H

#include <QDialog>
#include <QGroupBox>
#include <QCheckBox>
#include <QGraphicsView>

#include "Player/Player.h"
class PlayerIconGraphicsItem;

namespace Ui {
class PlayerSelectionWindow;
}

class PlayerSelectionWindow : public QDialog
{
    Q_OBJECT

public:
    /* Constructors */
    PlayerSelectionWindow(QVector<Player*>* players, QWidget *parent = nullptr);
    ~PlayerSelectionWindow();

    /* Methods */
    void call(int selectionAmount);

signals:
    void selectedPlayers(QVector<Player*>& players);

public slots:
    void removePlayer(Player* player);

private slots:
    void selected();

private:
    Ui::PlayerSelectionWindow *ui;

    QVector<Player*>* m_players;
    QVector<QGroupBox*> m_groupBoxes{};
    QVector<QCheckBox*> m_checkBoxes{};
    QVector<QGraphicsView*> m_playerViews{};
    QVector<PlayerIconGraphicsItem*> m_playerItems{};
    QVector<QGraphicsScene*> m_scenes{};

    int m_selectionAmount{};
    QVector<Player*> m_selectedPlayers{};

    void setupUiLists();
};

#endif // PLAYERSELECTIONWINDOW_H
