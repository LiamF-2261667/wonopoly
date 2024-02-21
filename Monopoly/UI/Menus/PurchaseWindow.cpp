#include "PurchaseWindow.h"
#include "ui_PurchaseWindow.h"

#include "Game/Purchase.h"
#include "UI/Cards/CardWidget.h"
#include "Player/Player.h"
#include "Cards/BuyableCard.h"


PurchaseWindow::PurchaseWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PurchaseWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
}

PurchaseWindow::~PurchaseWindow()
{
    delete ui;
}

/* Open a new purchase window */
void PurchaseWindow::openPurchaseWindow(Purchase* purchase, BuyableCard* card, Player* currentBuyer) {
    m_purchase = purchase;

    // Initializing the cardWidget
    ui->cardWidget = new CardWidget(card, this);

    // Setting the buyer text
    QString newInfoText = M_BASE_INFO_TEXT;
    newInfoText += QString::number(static_cast<int>(currentBuyer->getColor()) + 1);
    ui->infoText->setText(newInfoText);

    // Setting the price text
    ui->buyButton->setText("Buy $" + QString::number(card->getPrice()));

    // Deciding whether the player can buy the card
    if (!m_purchase->canBuy())
        ui->buyButton->setDisabled(true);
    else
        ui->buyButton->setDisabled(false);

    setVisible(true);
}

/* Close a purchase window */
void PurchaseWindow::closePurchaseWindow() {
    setVisible(false);
}

/* Buy a purchase */
void PurchaseWindow::on_buyButton_released()
{
    m_purchase->buy();
    closePurchaseWindow();
}

/* Don't buy the purchase -> auction the purchase */
void PurchaseWindow::on_auctionButton_released()
{
    m_purchase->auction();
    closePurchaseWindow();
}

