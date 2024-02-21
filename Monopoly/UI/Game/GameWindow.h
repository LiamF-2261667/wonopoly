#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Game/Game.h"
#include "qfiledialog.h"
#include <QMainWindow>

class MainMenuWindow;
class BoardGraphicsScene;
class DiceGraphicsScene;
class PlayerListGraphicScene;
class Board;
class Square;
class CompanySquare;
class Dice;
class Player;
class PurchaseWindow;
class Jail;
class JailWindow;
class Purchase;
class Auction;
class AuctionWindow;
class GameLoader;
class PlayerSelectionWindow;
class CardActionsWindow;
class CardListWindow;
class TradeWindow;
class ActionCardWindow;
class QFileDialog;
class WinScreenWindow;

#include "Player/Player.h"
#include "Cards/ActionCard.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    /* Constructors */
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

public slots:
    void initGame(int playerCount, const QString& loadFile);
    void gameStarted(Board* board, Dice* dice, GameLoader* gameloader, Purchase* purchase, Auction* auction);
    void playersLoaded(QVector<Player*>& players);
    void actionCardsLoaded(QVector<ActionCard*> communityChest, QVector<ActionCard*> chance);
    void connectJail(Jail* jail);
    void connectSquare(Square* square);
    void connectSquareItems();
    void disableRoll();
    void enableRoll();
    void resumeGame();
    void playersBankrupt();
    void announceBankruptcy(Player* player);
    void showCardAction(Square* square);
    void doPlayerSelectionEffect(QVector<Player*> players);
    void closeGameWindow();
    void showWinscreen(const Player* winner);

private slots:
    void on_showCardsButton_released();
    void on_tradeButton_released();
    void on_saveButton_released() { m_fileDialog->open(); }

    void on_stopButton_released() { m_game->stop(); }

private:
    enum class PLAYER_SELECTION_EFFECT {
        SHOW_LIST,
        SHOW_CARD_ACTION,
        TRADE
    };

    Ui::GameWindow *ui;
    MainMenuWindow* m_mainMenu{};
    Game* m_game{};

    PlayerListGraphicScene* m_playerListScene{};
    BoardGraphicsScene* m_boardScene{};
    DiceGraphicsScene* m_diceScene{};

    PurchaseWindow* m_purchaseWindow{};
    AuctionWindow* m_auctionWindow{};
    JailWindow* m_jailWindow{};
    PlayerSelectionWindow* m_playerSelectionWindow{};
    CardActionsWindow* m_cardActionsWindow{};
    ActionCardWindow* m_actionCardWindow{};
    CardListWindow* m_cardListWindow{};
    TradeWindow* m_tradeWindow{};
    QFileDialog* m_fileDialog{};
    WinScreenWindow* m_winScreenWindow{};


    PLAYER_SELECTION_EFFECT m_currentPlayerSelectionEffect{ PLAYER_SELECTION_EFFECT::SHOW_CARD_ACTION };

    void createConnections(Board* board, GameLoader* gameloader, Purchase* purchase, Auction* auction);
    void initializeWindows(Board* board, Dice* dice);
};

#endif // GAMEWINDOW_H
