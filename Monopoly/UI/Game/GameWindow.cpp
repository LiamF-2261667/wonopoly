#include "GameWindow.h"
#include "PlayerListGraphicScene.h"
#include "UI/Cards/CardListWindow.h"
#include "ui_GameWindow.h"

#include "Player/Player.h"
#include "UI/Board/BoardGraphicsScene.h"
#include "Board/Board.h"
#include "Board/Jail.h"
#include "Board/Squares/Square.h"
#include "DiceGraphicsScene.h"
#include "UI/Menus/MainMenuWindow.h"
#include "UI/Menus/PurchaseWindow.h"
#include "UI/Menus/AuctionWindow.h"
#include "Game/Purchase.h"
#include "Cards/BuyableCard.h"
#include "UI/Menus/JailWindow.h"
#include "UI/Menus/PlayerSelectionWindow.h"
#include "UI/Cards/CardActionsWindow.h"
#include "TradeWindow.h"
#include "WinScreenWindow.h"
#include "Cards/ActionCard.h"
#include "UI/Cards/ActionCardwindow.h"

#include <Game/Auction.h>
#include <Game/GameLoader.h>

#include <QFileDialog>
#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    // Initialize the main menu
    m_mainMenu = new MainMenuWindow{ this };

    // Connect the main menu start signals
    connect(m_mainMenu, &MainMenuWindow::startedGame, this, &GameWindow::initGame);
    connect(m_mainMenu, &MainMenuWindow::closedMainMenu, this, &GameWindow::closeGameWindow);

    m_mainMenu->show();
}

GameWindow::~GameWindow()
{
    delete m_game;
    delete m_mainMenu;
    delete ui;
}


/* Close the game and gamewindow */
void GameWindow::closeGameWindow() {
    QCoreApplication::instance()->quit();
}

/* Start the game */
void GameWindow::initGame(int playerCount, const QString& loadFile) {
    m_game = new Game{ playerCount };

    // Make the initial game setup connection
    connect(m_game, &Game::gameStarted, this, &GameWindow::gameStarted);

    // Show the game window
    showMaximized();

    m_game->startGame(loadFile);
}

/* Initialize the GameWindow when the game starts */
void GameWindow::gameStarted(Board* board, Dice* dice, GameLoader* gameloader, Purchase* purchase, Auction* auction) {
    initializeWindows(board, dice);
    createConnections(board, gameloader, purchase, auction);
}

/* Setup the windows, widgets, ... that originate from the GameWindow */
void GameWindow::initializeWindows(Board* board, Dice* dice) {
    // Initialize the boardScene and put it inside the boardView
    QRect boardRect{ ui->BoardView->rect() };
    boardRect.setWidth(boardRect.width() - 2);
    boardRect.setHeight(boardRect.height() - 2);
    m_boardScene = new BoardGraphicsScene{boardRect, board, this};
    ui->BoardView->setScene(m_boardScene);

    // Initialize the diceScene and put it inside the diceView
    m_diceScene = new DiceGraphicsScene{ui->DiceView->rect(), dice, m_game};
    ui->DiceView->setScene(m_diceScene);

    // Initialize the shopping windows
    m_auctionWindow = new AuctionWindow{this};
    m_purchaseWindow = new PurchaseWindow{this};

    // Initialize the playerListScene and put it inside the playerListView
    m_playerListScene = new PlayerListGraphicScene{ui->playerListView->rect(), m_game};
    ui->playerListView->setScene(m_playerListScene);

    // Initialize the cardActionsWindow
    m_cardActionsWindow = new CardActionsWindow(this);

    // Initialize the cardListWindow
    m_cardListWindow = new CardListWindow{ m_cardActionsWindow, this };

    // Initialize the tradeWindow
    m_tradeWindow = new TradeWindow{ this };

    // Initialize the actioncardWindow
    m_actionCardWindow = new ActionCardWindow{ this };

    // Initiailize the fileDialog for saving
    m_fileDialog = new QFileDialog{ this, "Select the savefile location" };
    m_fileDialog->setDefaultSuffix(".json");
}

/* Setup the connections between for the game and ui */
void GameWindow::createConnections(Board* board, GameLoader* gameloader, Purchase* purchase, Auction* auction) {
    // Signals from the board
    connect(board, &Board::createdSquare, m_boardScene, &BoardGraphicsScene::addSquareItem);
    connect(board, &Board::createdSquare, this, &GameWindow::connectSquare);
    connect(board, &Board::boardCreated, m_boardScene, &BoardGraphicsScene::allignSquares);
    connect(board, &Board::boardCreated, this, &GameWindow::connectSquareItems);
    connect(board, &Board::boardCreated, this, &GameWindow::connectJail);

    // Signals from the game
    connect(m_game, &Game::waitingForRoll, this, &GameWindow::enableRoll);
    connect(m_game, &Game::changedCurrentPlayer, m_playerListScene, &PlayerListGraphicScene::setCurrentPlayer);
    connect(m_game, &Game::inJailTurn, this, &GameWindow::disableRoll);
    connect(m_game, &Game::playerWon, this, &GameWindow::disableRoll);
    connect(m_game, &Game::playerWon, this, &GameWindow::showWinscreen);

    // Signals from a purchase
    connect(purchase, &Purchase::startingPurchase, m_purchaseWindow, &PurchaseWindow::openPurchaseWindow);
    connect(purchase, &Purchase::auctioningPurchase, m_purchaseWindow, &PurchaseWindow::closePurchaseWindow);
    connect(purchase, &Purchase::endingPurchase, m_purchaseWindow, &PurchaseWindow::closePurchaseWindow);
    connect(purchase, &Purchase::endingPurchase, this, &GameWindow::resumeGame);

    // Signals from an auction
    connect(auction, &Auction::startingAuction, m_auctionWindow, &AuctionWindow::openAuction);
    connect(auction, &Auction::startingAuction, this, &GameWindow::disableRoll);
    connect(auction, &Auction::endingAuction, m_auctionWindow, &AuctionWindow::closeAuction);
    connect(auction, &Auction::endingAuction, this, &GameWindow::resumeGame);
    connect(auction, &Auction::nextBid, m_auctionWindow, &AuctionWindow::changeBidder);

    // Signals from gameloader
    connect(gameloader, &GameLoader::finishedLoadingActionCards, this, &GameWindow::actionCardsLoaded);
    connect(gameloader, &GameLoader::finishedLoadingPlayers, this, &GameWindow::playersLoaded);

    // Signals from dialogs
    connect(m_actionCardWindow, &QDialog::rejected, this, &GameWindow::resumeGame);
    connect(m_cardListWindow, &QDialog::finished, this, &GameWindow::playersBankrupt);

    // Signals from UI elements
    connect(m_fileDialog, &QFileDialog::fileSelected, m_game, &Game::save);
}

void GameWindow::playersLoaded(QVector<Player*>& players) {
    m_playerListScene->initializePlayerList(players);
    m_playerSelectionWindow = new PlayerSelectionWindow(&players, this);

    // Signals from/to the player selection window
    connect(m_playerSelectionWindow, &PlayerSelectionWindow::selectedPlayers, this, &GameWindow::doPlayerSelectionEffect);

    connect(m_game, &Game::removedPlayerFromGame, m_playerSelectionWindow, &PlayerSelectionWindow::removePlayer);
    connect(m_game, &Game::removedPlayerFromGame, m_playerListScene, &PlayerListGraphicScene::removePlayer);
    connect(m_game, &Game::removedPlayerFromGame, m_boardScene, &BoardGraphicsScene::removePlayer);
    connect(m_game, &Game::removedPlayerFromGame, this, &GameWindow::announceBankruptcy);

    connect(m_game, &Game::playerIsBankrupt, m_cardListWindow, &CardListWindow::on_bankruptcy);
    connect(m_game, &Game::playerIsBankrupt, this, &GameWindow::disableRoll);

    // Signals from each player
    for (Player* pl : players) {
        connect(pl, &Player::playerSpawned, m_boardScene, &BoardGraphicsScene::addPlayer);
        connect(pl, &Player::playerMoved, m_boardScene, &BoardGraphicsScene::movePlayer);
        connect(pl, &Player::balanceChanged, m_playerListScene, &PlayerListGraphicScene::updatePlayerBalance);
        connect(pl, &Player::balanceChanged, m_auctionWindow, &AuctionWindow::updatePlayerSliders);
        connect(pl, &Player::recoveredFromBankruptcy, this, &GameWindow::resumeGame);
        connect(pl, &Player::recoveredFromBankruptcy, m_cardListWindow, &QDialog::close);
    }
}

void GameWindow::actionCardsLoaded(QVector<ActionCard*> communityChest, QVector<ActionCard*> chance) {
    for (ActionCard* card : communityChest) {
        connect(card, &ActionCard::doneEffect, m_actionCardWindow, &ActionCardWindow::call);
    }
    for (ActionCard* card : chance) {
        connect(card, &ActionCard::doneEffect, m_actionCardWindow, &ActionCardWindow::call);
    }
}

/* Connect the signals for a square when it gets added to the board */
void GameWindow::connectSquare(Square* square) {
    connect(square, &Square::inEffect, this, &GameWindow::disableRoll);
}

/* Connect the signals for the jail when it gets added to the board */
void GameWindow::connectJail(Jail* jail) {
    // Initialize the jail window
    m_jailWindow = new JailWindow{jail, this};

    // Connects
    connect(jail, &Jail::addedPlayerToJail, m_playerListScene, &PlayerListGraphicScene::addToJail);
    connect(jail, &Jail::removedPlayerFromJail, m_playerListScene, &PlayerListGraphicScene::removeFromJail);
    connect(jail, &Jail::removedPlayerFromJail, this, &GameWindow::enableRoll);
    connect(jail, &Jail::attemptedExit, this, &GameWindow::enableRoll);

    connect(m_game, &Game::inJailTurn, m_jailWindow, &JailWindow::showPrompt);
}

/* Connect clicking on a square item to opening a card action */
void GameWindow::connectSquareItems() {
    for (int i = 0; i < Board::TILE_AMOUNT; ++i)
        connect(m_boardScene->getSquare(i), &SquareGraphicsItem::selected, this, &GameWindow::showCardAction);
}

/* Show to winner */
void GameWindow::showWinscreen(const Player* winner) {
    m_winScreenWindow = new WinScreenWindow{ winner, this };

    connect(m_winScreenWindow, &WinScreenWindow::closeGame, this, &GameWindow::closeGameWindow);
}

/* Disables the ability to roll */
void GameWindow::disableRoll() {
    ui->DiceView->setDisabled(true);
    m_diceScene->setEnabled(false);
}

/* Enables the ability to roll */
void GameWindow::enableRoll() {
    ui->DiceView->setEnabled(true);
    m_diceScene->setEnabled(true);
}

/* Resume play of the game by switching control */
void GameWindow::resumeGame() {
    m_game->resumeGame();
}

/* Manage players after going bankrupt in the card list window */
void GameWindow::playersBankrupt() {
    m_game->managePlayers();
}

/* Show a message box upon the bankruptcy of a player */
void GameWindow::announceBankruptcy(Player* player) {
    QMessageBox msgBox;
    if (player->getIndebtedPlayer() != nullptr) {
        msgBox.setText("Player " + QString::number(static_cast<int>(player->getColor()) + 1) + " has gone bankrupt and lost the game"
                        " to player " + QString::number(static_cast<int>(player->getIndebtedPlayer()->getColor()) + 1) +  "!");
    }
    else {
        msgBox.setText("Player " + QString::number(static_cast<int>(player->getColor()) + 1) + " has gone bankrupt and lost the game!");
    }

    msgBox.exec();

}

/* Opens the player selection window */
void GameWindow::showCardAction(Square* square) {
    m_cardActionsWindow->setCard(square->getCard());
    m_playerSelectionWindow->call(1);
}

/* Decide what to do when a player got selected from the playerSelectionWindow */
void GameWindow::doPlayerSelectionEffect(QVector<Player*> players) {
    if (m_currentPlayerSelectionEffect == PLAYER_SELECTION_EFFECT::SHOW_LIST) {
        m_cardListWindow->call(players.first());
        m_currentPlayerSelectionEffect = PLAYER_SELECTION_EFFECT::SHOW_CARD_ACTION;
    }

    else if (m_currentPlayerSelectionEffect == PLAYER_SELECTION_EFFECT::TRADE) {
        m_tradeWindow->call(players);
        m_currentPlayerSelectionEffect = PLAYER_SELECTION_EFFECT::SHOW_CARD_ACTION;
    }

    else if (m_currentPlayerSelectionEffect == PLAYER_SELECTION_EFFECT::SHOW_CARD_ACTION)
        m_cardActionsWindow->call(players);
}

/* Open the cardlist of a given player */
void GameWindow::on_showCardsButton_released() {
    m_currentPlayerSelectionEffect = PLAYER_SELECTION_EFFECT::SHOW_LIST;
    m_playerSelectionWindow->call(1);
}

/* Open the trade menu */
void GameWindow::on_tradeButton_released() {
    m_currentPlayerSelectionEffect = PLAYER_SELECTION_EFFECT::TRADE;
    m_playerSelectionWindow->call(2);
}
