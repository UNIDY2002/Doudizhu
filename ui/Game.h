#ifndef DOUDIZHU_GAME_H
#define DOUDIZHU_GAME_H

#include <QtWidgets/QWidget>
#include <network/NetworkPolicy.h>
#include "ui_game.h"

namespace Ui {
    class Game;
}

class Game : public QWidget {

    NetworkPolicy *policy;

    Ui::Game *ui;

public:
    explicit Game(NetworkPolicy *policy, QWidget *parent = nullptr);

    ~Game() override;

};

#endif //DOUDIZHU_GAME_H
