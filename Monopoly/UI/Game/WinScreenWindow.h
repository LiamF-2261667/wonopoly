#ifndef WINSCREENWINDOW_H
#define WINSCREENWINDOW_H

#include <QDialog>

class Player;
class QGraphicsScene;
class PlayerIconGraphicsItem;

namespace Ui {
class WinScreenWindow;
}

class WinScreenWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WinScreenWindow(const Player* winner, QWidget *parent = nullptr);
    ~WinScreenWindow();

signals:
    void closeGame();

private slots:
    void on_closeGameButton_released() { emit closeGame(); }

private:
    Ui::WinScreenWindow *ui;

    QGraphicsScene* m_winnerIconScene{};
    PlayerIconGraphicsItem* m_winnerIconItem{};
};

#endif // WINSCREENWINDOW_H
