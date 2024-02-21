#include "CardListWindow.h"
#include "CardWidget.h"
#include "Cards/CardList.h"
#include "CardActionsWindow.h"
#include "ui_CardListWindow.h"
#include "CardInspectButton.h"

#include "Cards/BuyableCard.h"
#include "Player/Player.h"

#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>

#include <Cards/ActionCard.h>

CardListWindow::CardListWindow(CardActionsWindow* cardActionsWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardListWindow),
    m_cardActionsWindow{ cardActionsWindow }
{
    ui->setupUi(this);
    connect(m_cardActionsWindow, &CardActionsWindow::cardSold, this, &CardListWindow::refresh);
}

CardListWindow::~CardListWindow()
{
    delete ui;
    delete m_currentLayout;
}

/* Open the the CardListWindow of a given CardList for an owner */
void CardListWindow::call(Player* owner) {
    // Reset the window
    QLayoutItem* item{};
    if (m_currentLayout != nullptr)
        while((item = m_currentLayout->takeAt(0)))
            delete item->widget();
    m_cardGroups.clear();
    m_cardList.clear();
    delete m_currentLayout;
    delete m_currentWidget;

    // Setting the new variables
    m_owner = owner;
    m_currentLayout = new QHBoxLayout{};
    m_currentWidget = new QWidget{};

    // Getting a list of all cards from the owner
    for (int i = 0; i < m_owner->cardsOwned(); ++i)
        m_cardList.append(m_owner->getCard(i));
    sortCardList();

    // Setting up the layout
    m_currentWidget->setLayout(m_currentLayout);
    ui->CardListScrollArea->setWidget(m_currentWidget);

    // Adding each card
    for (int i = 0; i < m_cardList.size(); ++i) {
        CardGroup newCardGroup{ createCardGroup(m_cardList[i]) };
        newCardGroup.groupBox->setParent(ui->CardListScrollArea);

        m_currentLayout->addWidget(newCardGroup.groupBox);
        m_cardGroups.append(newCardGroup);
    }

    // Setting other information
    ui->PlayerInfo->setText("Showing cards for player "
                            + QString::number(static_cast<int>(m_owner->getColor()) + 1));
    ui->jailCardLabel->setText("Get Out Of Jail cards owned: " + QString::number(m_owner->jailCardsOwned()));
    if (m_owner->jailCardsOwned() == 0)
        ui->sellJailCard->setDisabled(true);
    else
        ui->sellJailCard->setDisabled(false);
    m_open = true;
    show();
}

/* Sort the cards in the stored list */
void CardListWindow::sortCardList() {
    for (int i = 0; i < m_cardList.size(); ++i) {
        for (int j = 0; j < m_cardList.size() - 1; ++j) {
            // First, sort by type
            if (m_cardList[j]->getType() > m_cardList[j + 1]->getType())
                m_cardList.swapItemsAt(j, j+1);

            // Then sort by id
            else if (m_cardList[j]->getType() == m_cardList[j + 1]->getType() &&
                m_cardList[j]->getId() > m_cardList[j + 1]->getId())
                m_cardList.swapItemsAt(j, j+1);
        }
    }
}

/* Create a GroupBox with the corresponding CardWidget and inspectButton*/
CardListWindow::CardGroup CardListWindow::createCardGroup(BuyableCard* card) {
    // Creating the widgets
    CardGroup newGroup{};
    newGroup.groupBox = new QGroupBox{""};
    newGroup.cardWidget = new CardWidget{ card, newGroup.groupBox };
    newGroup.pushButton = new CardInspectButton{ card, newGroup.groupBox };

    // Setting the layout
    QVBoxLayout* layout{ new QVBoxLayout{ newGroup.groupBox } };
    layout->addWidget(newGroup.cardWidget);
    layout->addWidget(newGroup.pushButton);

    // Setting sizes
    newGroup.groupBox->setMinimumWidth(CardGroup::BASE_WIDTH);
    newGroup.pushButton->setMaximumWidth(newGroup.cardWidget->width());

    // Adding functionality
    connect(newGroup.pushButton, &CardInspectButton::clicked, m_cardActionsWindow, &CardActionsWindow::setCard);
    connect(newGroup.pushButton, &CardInspectButton::clicked, this, &CardListWindow::on_cardInspection);

    return newGroup;
}

/* Open the card actions window on inspection */
void CardListWindow::on_cardInspection() {
    m_cardActionsWindow->call(m_owner);
}

/* Open the window for selling items */
void CardListWindow::on_bankruptcy(Player* player) {
    ui->ExitButton->setDisabled(true);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    call(player);

    QMessageBox msgBox;
    msgBox.setText("Player " + QString::number(static_cast<int>(player->getColor()) + 1) + " has gone bankrupt!\n"
                   "Sell your cards until you're back above $0.");
    msgBox.exec();
}

/* Sell jail card */
void CardListWindow::on_sellJailCard_released() {
    m_owner->removeJailCard();
    m_owner->gain(ActionCard::JAIL_CARD_SELL_VALUE);

    ui->jailCardLabel->setText("Get Out Of Jail cards owned: " + QString::number(m_owner->jailCardsOwned()));
    if (m_owner->jailCardsOwned() == 0)
        ui->sellJailCard->setDisabled(true);
}

/* Refresh the list window */
void CardListWindow::refresh(Player* user) {
     if (m_open) call(user);

     /* Bankrupty functionality */
     if (m_owner->getMoney() >= 0) {
        ui->ExitButton->setDisabled(false);
        setWindowFlag(Qt::WindowCloseButtonHint, true);
     }

     else if (m_owner->cardsOwned() == 0 && m_owner->jailCardsOwned() == 0) {
        done(0);
        close();
     }
}

void CardListWindow::on_CardListWindow_rejected()
{
    m_open = false;
}

