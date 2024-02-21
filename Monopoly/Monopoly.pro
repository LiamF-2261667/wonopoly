QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Board/Board.cpp \
    Board/Squares/BuyableSquare.cpp \
    Board/Squares/ChanceSquare.cpp \
    Board/Squares/CommunityChestSquare.cpp \
    Board/Squares/CompanySquare.cpp \
    Board/Squares/IncomeTaxSquare.cpp \
    Board/Squares/JailSquare.cpp \
    Board/Squares/PoliceSquare.cpp \
    Board/Squares/Square.cpp \
    Board/Squares/FreeSquare.cpp \
    Board/Squares/StartSquare.cpp \
    Board/Squares/StationSquare.cpp \
    Board/Squares/StreetSquare.cpp \
    Cards/ActionCard.cpp \
    Cards/BuyableCard.cpp \
    Cards/CompanyCard.cpp \
    Cards/StationCard.cpp \
    Cards/StreetCard.cpp \
    Game/Auction.cpp \
    Board/Jail.cpp \
    Game/GameLoader.cpp \
    Game/Purchase.cpp \
    Game/Trade.cpp \
    Player/Balance.cpp \
    Game/Dice.cpp \
    Player/Player.cpp \
    UI/Cards/ActionCardwindow.cpp \
    UI/Cards/CardActionsWindow.cpp \
    UI/Cards/CardInspectButton.cpp \
    UI/Cards/CardListWindow.cpp \
    UI/Cards/CardWidget.cpp \
    UI/Game/DiceGraphicsScene.cpp \
    UI/Board/BoardGraphicsScene.cpp \
    Game/Game.cpp \
    UI/Board/SquareGraphicsItem.cpp \
    UI/Game/GameWindow.cpp \
    UI/Game/PlayerListGraphicScene.cpp \
    UI/Game/PlayerListItemGraphicsItem.cpp \
    UI/Game/TradeWindow.cpp \
    UI/Game/WinScreenWindow.cpp \
    UI/Menus/AuctionWindow.cpp \
    UI/Menus/JailWindow.cpp \
    UI/Menus/PlayerSelectionWindow.cpp \
    UI/Menus/PurchaseWindow.cpp \
    UI/Player/PlayerGraphicsItem.cpp \
    UI/Player/PlayerIconGraphicsItem.cpp \
    main.cpp \
    UI/Menus/MainMenuWindow.cpp

HEADERS += \
    Board/Board.h \
    Board/Squares/BuyableSquare.h \
    Board/Squares/ChanceSquare.h \
    Board/Squares/CommunityChestSquare.h \
    Board/Squares/CompanySquare.h \
    Board/Squares/IncomeTaxSquare.h \
    Board/Squares/JailSquare.h \
    Board/Squares/PoliceSquare.h \
    Board/Squares/Square.h \
    Board/Squares/FreeSquare.h \
    Board/Squares/StartSquare.h \
    Board/Squares/StationSquare.h \
    Board/Squares/StreetSquare.h \
    Cards/ActionCard.h \
    Cards/BuyableCard.h \
    Cards/CompanyCard.h \
    Cards/StationCard.h \
    Cards/StreetCard.h \
    Game/Auction.h \
    Board//Jail.h \
    Game/GameLoader.h \
    Game/Purchase.h \
    Game/Trade.h \
    Player/Balance.h \
    Cards/CardList.h \
    Game/Dice.h \
    Player/Money.h \
    Player/Player.h \
    UI/Cards/ActionCardwindow.h \
    UI/Cards/CardActionsWindow.h \
    UI/Cards/CardInspectButton.h \
    UI/Cards/CardListWindow.h \
    UI/Cards/CardWidget.h \
    UI/Game/DiceGraphicsScene.h \
    UI/Board/BoardGraphicsScene.h \
    Game/Game.h \
    UI/Game/GameWindow.h \
    UI/Game/PlayerListGraphicScene.h \
    UI/Game/PlayerListItemGraphicsItem.h \
    UI/Game/TradeWindow.h \
    UI/Game/WinScreenWindow.h \
    UI/Menus/AuctionWindow.h \
    UI/Menus/JailWindow.h \
    UI/Menus/PlayerSelectionWindow.h \
    UI/Menus/PurchaseWindow.h \
    UI/Player/PlayerGraphicsItem.h \
    UI/Menus/MainMenuWindow.h \
    UI/Board/SquareGraphicsItem.h \
    UI/Player/PlayerIconGraphicsItem.h

FORMS += \
    UI/Cards/ActionCardwindow.ui \
    UI/Cards/CardActionsWindow.ui \
    UI/Cards/CardListWindow.ui \
    UI/Cards/CardWidget.ui \
    UI/Game/GameWindow.ui \
    UI/Game/TradeWindow.ui \
    UI/Game/WinScreenWindow.ui \
    UI/Menus/AuctionWindow.ui \
    UI/Menus/JailWindow.ui \
    UI/Menus/MainMenuWindow.ui \
    UI/Menus/PlayerSelectionWindow.ui \
    UI/Menus/PurchaseWindow.ui

RESOURCES += Resources/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
