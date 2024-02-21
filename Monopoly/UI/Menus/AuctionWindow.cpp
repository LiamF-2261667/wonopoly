#include "AuctionWindow.h"
#include "ui_AuctionWindow.h"

#include "Game/Auction.h"
#include "Player/Player.h"

#include <UI/Player/PlayerGraphicsItem.h>

#include <UI/Cards/CardWidget.h>

AuctionWindow::AuctionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuctionWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowCloseButtonHint, false);

    m_playerIconScene = new QGraphicsScene{ ui->PlayerIconView };
    ui->PlayerIconView->setScene(m_playerIconScene);
}

AuctionWindow::~AuctionWindow()
{
    delete ui;
}

/* Open a new auction window */
void AuctionWindow::openAuction(Auction* auction) {
    m_auction = auction;

    resetPlayerSliders();
    setupPlayerIcon();

    ui->cardWidget = new CardWidget{auction->getCard(), this};

    show();
}

/* Close the auction window */
void AuctionWindow::closeAuction() {
    m_auction = nullptr;

    setVisible(false);
}

/* Setup the player icon graphics */
void AuctionWindow::setupPlayerIcon() {
    qreal radius = qMin(ui->PlayerIconView->width(), ui->PlayerIconView->height()) - M_PLAYER_ICON_MARGIN;

    PlayerIconGraphicsItem* newPlayerIcon = new PlayerIconGraphicsItem{ m_auction->getCurrentBidder() , radius};
    m_playerIconScene->addItem(newPlayerIcon);
    newPlayerIcon->setPos(M_PLAYER_ICON_MARGIN/2, M_PLAYER_ICON_MARGIN/2);
}

/* Update the max value of the current player sliders if nessesary */
void AuctionWindow::updatePlayerSliders(Player* player) {
    // If there is no auction being held, do nothing
    if (m_auction == nullptr) return;

    // If the player isn't the current bidder, there isn't anything to update
    if (player != m_auction->getCurrentBidder()) return;

    // Else reset the max values
    ui->BiddingSlider->setMaximum(m_auction->getCurrentBidder()->getMoney());
    ui->BiddingSpinBox->setMaximum(m_auction->getCurrentBidder()->getMoney());
}

/* reset all player sliders and show only the ones for the players still left in the game */
void AuctionWindow::resetPlayerSliders() {
    // Set the max value of each slider
    ui->BiddingSlider->setMaximum(m_auction->getCurrentBidder()->getMoney());
    ui->BiddingSpinBox->setMaximum(m_auction->getCurrentBidder()->getMoney());

    // Set the minimum value of each slider
    ui->BiddingSlider->setMinimum(m_auction->getHighestBid() + 1);
    ui->BiddingSpinBox->setMinimum(m_auction->getHighestBid() + 1);

    // Set the sliders back to 0
    ui->BiddingSlider->setValue(0);
    ui->BiddingSpinBox->setValue(0);
}

/* Confirm all bids */
void AuctionWindow::on_PlaceButton_released()
{
    m_auction->bid(ui->BiddingSlider->value());
}


void AuctionWindow::on_CancelButton_released()
{
    m_auction->nextBidder();
}

