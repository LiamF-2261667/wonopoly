#include "CardActionsWindow.h"
#include "CardWidget.h"
#include "ui_CardActionsWindow.h"

#include "Cards/BuyableCard.h"
#include "Cards/StreetCard.h"

#include "Player/Player.h"

CardActionsWindow::CardActionsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardActionsWindow)
{
    ui->setupUi(this);
    ui->cardOwnerLabel->setText("");
    connectButtons();
}

CardActionsWindow::~CardActionsWindow()
{
    delete ui;
}

/* Call the card actions window */
void CardActionsWindow::call(Player* player) {
    m_user = player;
    ui->buildTierLabel->setText("");
    ui->cardWidget = new CardWidget{ m_card, this };

    // Set the owner label
    if (m_card->getOwner() != nullptr)
        ui->cardOwnerLabel->setText("Owner of this card: Player" + QString::number((int)m_card->getOwner()->getColor() + 1));
    else
        ui->cardOwnerLabel->setText("This card has no owner");

    // Set the build tier label for street squares
    if (m_card->getType() == BuyableCard::TYPE::STREET && m_card->getOwner() != nullptr) {

        StreetCard::BUILD_TIER tier = static_cast<StreetCard*>(m_card)->getTier();
        if (tier == StreetCard::BUILD_TIER::HOTEL)
            ui->buildTierLabel->setText("This property has a hotel");
        else
            ui->buildTierLabel->setText("This property has " + QString::number((int)tier) + " houses");
    }

    // Connecting signals
    connect(m_card, &BuyableCard::cardActionPerformed, this, &CardActionsWindow::on_success);

    // Open the window
    showButtons();
    show();
}

/* Show buttons depending on the type */
void CardActionsWindow::showButtons() {
    QVector<BuyableCard::ACTION> actions = m_card->getActions(m_user);

    // Hide all buttons first
    ui->mortgageBox->hide();
    ui->unmortgageBox->hide();
    ui->buildBox->hide();
    ui->downgradeBox->hide();
    ui->sellBox->hide();

    // Show all the useable buttons
    if (actions.contains(BuyableCard::ACTION::BUILD)) {
        ui->buildBox->show();
        ui->buildPriceLabel->setText("Price: " + QString::number(m_card->getPrice()));
    }
    if (actions.contains(BuyableCard::ACTION::MORTGAGE)) {
        ui->mortgageBox->show();
        ui->mortgageValueLabel->setText("Value: " + QString::number(m_card->getMortgagePrice()));
    }
    if (actions.contains(BuyableCard::ACTION::UNMORTGAGE)) {
        ui->unmortgageBox->show();
        ui->unmortgagePriceLabel->setText("Price: " + QString::number(m_card->getMortgagePrice()));
    }
    if (actions.contains(BuyableCard::ACTION::DOWNGRADE)) {
        ui->downgradeBox->show();
        ui->downgradeValueLabel->setText("Value: " + QString::number(m_card->getSellValue()));
    }
    if (actions.contains(BuyableCard::ACTION::SELL)) {
        ui->sellBox->show();
        ui->sellValueLabel->setText("Value: " + QString::number(m_card->getSellValue()));
    }
}

/* Connect button presses to functions */
void CardActionsWindow::connectButtons() {
    connect(ui->sellButton, &QPushButton::released, this, &CardActionsWindow::on_sell);
    connect(ui->buildButton, &QPushButton::released, this, &CardActionsWindow::on_build);
    connect(ui->mortgageButton, &QPushButton::released, this, &CardActionsWindow::on_mortgage);
    connect(ui->unmortgageButton, &QPushButton::released, this, &CardActionsWindow::on_unmortgage);
    connect(ui->downgradeButton, &QPushButton::released, this, &CardActionsWindow::on_downgrade);
    connect(ui->exitButton, &QPushButton::released, this, &CardActionsWindow::on_exit);
}

