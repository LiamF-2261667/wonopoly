#ifndef PURCHASEWINDOW_H
#define PURCHASEWINDOW_H

#include <QDialog>

class Purchase;
class BuyableCard;
class Player;

namespace Ui {
class PurchaseWindow;
}

class PurchaseWindow : public QDialog
{
    Q_OBJECT

public:
    /* Constructors */
    explicit PurchaseWindow(QWidget *parent = nullptr);
    ~PurchaseWindow();

public slots:
    void openPurchaseWindow(Purchase* purchase, BuyableCard* card, Player* currentBuyer);
    void closePurchaseWindow();

private slots:
    void on_buyButton_released();
    void on_auctionButton_released();

private:
    const QString M_BASE_INFO_TEXT{ "Buying for: \nPlayer " };

    Ui::PurchaseWindow *ui;

    Purchase* m_purchase{};
};

#endif // PURCHASEWINDOW_H
