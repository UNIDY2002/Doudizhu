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

public:
    explicit Game(NetworkPolicy *policy, int order, const QStringList &cards, QWidget *parent = nullptr);

    ~Game() override;

private slots:

    void updateCards();

    void enableCards();

    void resetButtons();

    void updateCallingStatus(int id, bool call, bool someoneCalled, int myOrder);

    void checkValidity();

};

#endif //DOUDIZHU_GAME_H
