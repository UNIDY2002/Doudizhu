#ifndef DOUDIZHU_GAME_H
#define DOUDIZHU_GAME_H

#include <QtWidgets/QWidget>
#include <network/NetworkPolicy.h>
#include <game/GameLogic.h>
#include "ui_game.h"

namespace Ui {
    class Game;
}

class Game : public QWidget {

    NetworkPolicy *policy;

    GameLogic *logic;

    Ui::Game *ui;

    QPushButton *cardButtons[20]{};

    QStringList pickedCardsCache{};

    bool readyToQuit = false;

    void init();

    void processCallingButtons();

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    explicit Game(NetworkPolicy *policy, int order, const QStringList &cards, QWidget *parent = nullptr);

    ~Game() override;

private slots:

    void updateCards();

    void enableCards();

    void resetButtons();

    void updateCallingStatus(int id, bool call, bool someoneCalled);

    void checkValidity();

    void refreshMeta();

    void updateMessage(int id, const QString &message);

    void onGameStops();

    void onForceExit();

    void onHotswap();

    void onHotswapWithInitParams(int order, const QStringList &cards);

};

#endif //DOUDIZHU_GAME_H
