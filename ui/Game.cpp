#include "Game.h"

Game::Game(NetworkPolicy *policy, QWidget *parent) : QWidget(parent), policy(policy), ui(new Ui::Game) {
    ui->setupUi(this);
}

Game::~Game() {
    delete ui;
}
