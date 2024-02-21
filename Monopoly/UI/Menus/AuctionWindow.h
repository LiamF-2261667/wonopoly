#ifndef AUCTIONWINDOW_H
#define AUCTIONWINDOW_H

#include <QDialog>

class Auction;
class BuyableCard;
class QSlider;
class QSpinBox;
class QGraphicsView;
class QGroupBox;
class QGraphicsScene;
class PlayerIconGraphicsItem;
class Player;

namespace Ui {
class AuctionWindow;
}

class AuctionWindow : public QDialog
{
    Q_OBJECT

public:
    /* Constructors */
    explicit AuctionWindow(QWidget *parent = nullptr);
    ~AuctionWindow();

public slots:
    void openAuction(Auction* auction);
    void closeAuction();
    void changeBidder() { setupPlayerIcon(); resetPlayerSliders(); }
    void updatePlayerSliders(Player* player);

private slots:
    void on_PlaceButton_released();
    void on_CancelButton_released();

private:
    const qreal M_PLAYER_ICON_MARGIN{ 10 };

    Ui::AuctionWindow *ui;
    QGraphicsScene* m_playerIconScene{};

    Auction* m_auction{};

    void setupPlayerIcon();
    void resetPlayerSliders();

};

#endif // AUCTIONWINDOW_H
