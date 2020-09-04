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

public:
    explicit Game(NetworkPolicy *policy, int order, const QStringList &cards, QWidget *parent = nullptr);

    ~Game() override;

private slots:

    void updateCards(const QStringList &cards);

};

#endif //DOUDIZHU_GAME_H
