#include "Game.h"

Game::Game(NetworkPolicy *policy, int order, const QStringList &cards, QWidget *parent) :
        QWidget(parent), policy(policy), logic(new GameLogic(order, cards)), ui(new Ui::Game) {
    ui->setupUi(this);
    updateCards(logic->myCards);
    policy->prepare(logic);
    connect(ui->dummy, &QPushButton::clicked, [&]() {
        logic->sendMessage({DUMMY_MESSAGE});
    });
}

Game::~Game() {
    delete ui;
}

void Game::updateCards(const QStringList &cards) {
    ui->cards->setText(cards.join(' '));
}
