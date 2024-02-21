#include "TradeWindow.h"
#include "ui_TradeWindow.h"
#include "Game/Trade.h"
#include "UI/Player/PlayerIconGraphicsItem.h"
#include "UI/Cards/CardInspectButton.h"

TradeWindow::TradeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TradeWindow)
{
    ui->setupUi(this);
    ui->failureLabel->setText("");

    m_trade = new Trade;

    connect(m_trade, &Trade::tradeFailed, this, &TradeWindow::onTradeFailure);
    connect(m_trade, &Trade::tradeSuccessful, this, &TradeWindow::onTradeSuccess);

    connect(ui->confirmButton, &QPushButton::released, this, &TradeWindow::on_confirmButtonRelease);
    connect(ui->declineButton, &QPushButton::released, this, &TradeWindow::on_declineButtonRelease);
}

TradeWindow::~TradeWindow()
{
    delete ui;
    delete m_trade;
}

/* Readies and opens the trade window */
void TradeWindow::call(QVector<Player*> players) {
    // Clear previous settings
    clear();

    // Setup players
    m_players.first = players[0];
    m_players.second = players[1];

    // Setup ui
    setupPlayerDetails();
    setupCardLists();

    show();
}

/* Updates the trade window labels & views for the given players in call(...) */
void TradeWindow::setupPlayerDetails() {
    // Setup scenes
    ui->Player1_view->setScene(new QGraphicsScene(ui->Player1_view));
    ui->Player2_view->setScene(new QGraphicsScene(ui->Player2_view));

    // Setup icons
    PlayerIconGraphicsItem* icon1 = new PlayerIconGraphicsItem(m_players.first, ui->Player1_view->width() / 1.5);
    PlayerIconGraphicsItem* icon2 = new PlayerIconGraphicsItem(m_players.second, ui->Player2_view->width() / 1.5);
    ui->Player1_view->scene()->addItem(icon1);
    ui->Player2_view->scene()->addItem(icon2);

    // Setup name labels
    ui->Player1_label->setText("Player " + QString::number((int)m_players.first->getColor()));
    ui->Player2_label->setText("Player " + QString::number((int)m_players.second->getColor()));

    // Setup balance labels
    ui->Player1_balance->setText("$" + QString::number((int)m_players.first->getMoney()));
    ui->Player2_balance->setText("$" + QString::number((int)m_players.second->getMoney()));

    // Setup money spin boxes
    if (m_players.first->getMoney() >= 0)
        ui->Player1_money_spinBox->setMaximum(m_players.first->getMoney());
    else
        ui->Player1_money_spinBox->setMaximum(0);
    ui->Player1_money_spinBox->setValue(0);
    if (m_players.second->getMoney() >= 0)
        ui->Player2_money_spinBox->setMaximum(m_players.second->getMoney());
    else
        ui->Player2_money_spinBox->setMaximum(0);
    ui->Player2_money_spinBox->setValue(0);

    // Setup jail card spin boxes
    ui->Player1_jailcard_spinBox->setMaximum(m_players.first->jailCardsOwned());
    ui->Player1_jailcard_spinBox->setValue(0);
    ui->Player2_jailcard_spinBox->setMaximum(m_players.second->jailCardsOwned());
    ui->Player2_jailcard_spinBox->setValue(0);
}

/* Updates the card lists for the given players in call(...) */
void TradeWindow::setupCardLists() {
    // Create widgets for selectable lists
    for (int i = 0; i < 4; ++i) {
        m_currentWidgets.push_back( new QWidget );
        m_currentLayouts.push_back( new QHBoxLayout );
        m_currentWidgets[i]->setLayout(m_currentLayouts[i]);
    }

    ui->Player1_cardList_scrollArea->setWidget(m_currentWidgets[0]);
    ui->Player2_cardList_scrollArea->setWidget(m_currentWidgets[1]);
    ui->Player1_cardList_scrollArea_2->setWidget(m_currentWidgets[2]);
    ui->Player2_cardList_scrollArea_2->setWidget(m_currentWidgets[3]);

    // Add cards to the lists
    // For player 1
    for (int i = 0; i < m_players.first->cardsOwned(); ++i) {
        CardGroup newCardGroup{ createCardGroup(m_players.first->getCard(i)) };
        newCardGroup.groupBox->setMinimumWidth(CardGroup::BASE_WIDTH);

        m_currentLayouts[0]->addWidget(newCardGroup.groupBox);
        m_cardGroups.push_back(newCardGroup);
    }

    // For player 2
    for (int i = 0; i < m_players.second->cardsOwned(); ++i) {
        CardGroup newCardGroup{ createCardGroup(m_players.second->getCard(i)) };
        newCardGroup.groupBox->setMinimumWidth(CardGroup::BASE_WIDTH);

        m_currentLayouts[1]->addWidget(newCardGroup.groupBox);
        m_cardGroups.push_back(newCardGroup);
    }

}

/* Creates a card group to put in the list */
TradeWindow::CardGroup TradeWindow::createCardGroup(BuyableCard* card) {
    // Creating the widgets
    CardGroup newGroup{};
    newGroup.groupBox = new QGroupBox{""};
    newGroup.cardWidget = new CardWidget{ card, newGroup.groupBox };
    newGroup.pushButton = new CardInspectButton{ card, newGroup.groupBox };

    newGroup.cardWidget->setScale(0.35);
    newGroup.groupBox->setMaximumWidth(CardGroup::BASE_WIDTH);
    newGroup.pushButton->setText("Add");

    // Setting the layout
    QVBoxLayout* layout{ new QVBoxLayout{ newGroup.groupBox } };
    layout->addWidget(newGroup.cardWidget);
    layout->addWidget(newGroup.pushButton);

    // Adding functionality
    connect(newGroup.pushButton, &CardInspectButton::clicked, this, &TradeWindow::on_cardPress);

    return newGroup;
}

void TradeWindow::on_cardPress(BuyableCard* card) {
    CardGroup* selectedCard{};
    for (int i = 0; i < m_cardGroups.size(); ++i) {
        if (m_cardGroups[i].cardWidget->getCard() == card) {
            selectedCard = &m_cardGroups[i];
        }
    }

    if (!selectedCard->selected) onAdd(selectedCard);
    else if (selectedCard->selected) onRemove(selectedCard);
}

void TradeWindow::onAdd(CardGroup* selectedCard) {
    if (!selectedCard->pushButton->isDown()) return;

    selectedCard->pushButton->setText("Remove");

    if (selectedCard->cardWidget->getCard()->getOwner() == m_players.first) {
        m_selectedCardGroups1.push_back(selectedCard);
        m_currentLayouts[0]->removeWidget(selectedCard->groupBox);
        m_currentLayouts[2]->addWidget(selectedCard->groupBox);
    }
    else {
        m_selectedCardGroups2.push_back(selectedCard);
        m_currentLayouts[1]->removeWidget(selectedCard->groupBox);
        m_currentLayouts[3]->addWidget(selectedCard->groupBox);
    }

    selectedCard->selected = true;
}

void TradeWindow::onRemove(CardGroup* selectedCard) {
    if (!selectedCard->pushButton->isDown()) return;

    selectedCard->pushButton->setText("Add");

    if (selectedCard->cardWidget->getCard()->getOwner() == m_players.first) {
        m_selectedCardGroups1.removeAt(m_selectedCardGroups1.indexOf(selectedCard));
        m_currentLayouts[2]->removeWidget(selectedCard->groupBox);
        m_currentLayouts[0]->addWidget(selectedCard->groupBox);
    }
    else {
        m_selectedCardGroups2.removeAt(m_selectedCardGroups2.indexOf(selectedCard));
        m_currentLayouts[3]->removeWidget(selectedCard->groupBox);
        m_currentLayouts[1]->addWidget(selectedCard->groupBox);
    }

    selectedCard->selected = false;
}

void TradeWindow::on_confirmButtonRelease() {
    // Initialize variables
    QPair<QVector<BuyableCard*>, QVector<BuyableCard*>> cards{};
    QPair<Money, Money> money{};
    QPair<int, int> jailCards{};

    for (int i = 0; i < m_selectedCardGroups1.size(); ++i)
        cards.first.push_back(m_selectedCardGroups1[i]->cardWidget->getCard());

    for (int i = 0; i < m_selectedCardGroups2.size(); ++i)
        cards.second.push_back(m_selectedCardGroups2[i]->cardWidget->getCard());

    money.first = ui->Player1_money_spinBox->value();
    money.second = ui->Player2_money_spinBox->value();

    jailCards.first = ui->Player1_jailcard_spinBox->value();
    jailCards.second = ui->Player2_jailcard_spinBox->value();

    // Start trading exchange
    m_trade->start(m_players, cards, money, jailCards);
}

void TradeWindow::onTradeFailure(QString reason) {
    ui->failureLabel->setText(reason);
}

/* Removes all changes of previous iteration */
void TradeWindow::clear() {
    if (m_players.first == nullptr) return;

    delete ui->Player1_view->scene();
    delete ui->Player2_view->scene();

    for (QLayout* l : m_currentLayouts) delete l;
    for (QWidget* w : m_currentWidgets) delete w;
    m_currentLayouts.clear();
    m_currentWidgets.clear();

    m_cardGroups.clear();
    m_selectedCardGroups1.clear();
    m_selectedCardGroups2.clear();

    ui->Player1_jailcard_spinBox->setValue(0);
    ui->Player2_jailcard_spinBox->setValue(0);
    ui->Player1_money_spinBox->setValue(0);
    ui->Player2_money_spinBox->setValue(0);

    ui->failureLabel->setText("");
}
