#include "WinScreenWindow.h"
#include "Player/Player.h"
#include "ui_WinScreenWindow.h"

#include <UI/Player/PlayerIconGraphicsItem.h>

WinScreenWindow::WinScreenWindow(const Player* winner, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WinScreenWindow)
{
    ui->setupUi(this);

    // Disable the exit window button
    setWindowFlag(Qt::WindowCloseButtonHint, false);

    // Setup the winner label
    QString winnerText{ "Player " };
    winnerText += QString::number(static_cast<int>(winner->getColor()) + 1);
    winnerText += " Won!";
    ui->winnerLabel->setText(winnerText);

    // Setup winner icon
    m_winnerIconScene = new QGraphicsScene{ this };
    ui->winnerIconView->setScene(m_winnerIconScene);
    m_winnerIconItem = new PlayerIconGraphicsItem{ winner, ui->winnerIconView->minimumHeight() - 2.0 };
    m_winnerIconScene->addItem(m_winnerIconItem);

    show();
}

WinScreenWindow::~WinScreenWindow()
{
    delete ui;
}

