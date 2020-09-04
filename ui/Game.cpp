#include "Game.h"

Game::Game(NetworkPolicy *policy, int order, const QStringList &cards, QWidget *parent) :
        QWidget(parent), policy(policy), logic(new GameLogic(order, cards)), ui(new Ui::Game) {
    ui->setupUi(this);
    updateCards();
    policy->prepare(logic);
    connect(logic, &GameLogic::callingStatusUpdated, this, &Game::updateCallingStatus);
    connect(logic, &GameLogic::cardsUpdated, this, &Game::updateCards);

    // To be refactored...
    switch (order) {
        case 0:
            ui->status0->hide();
            ui->positive1->hide();
            ui->negative1->hide();
            ui->positive2->hide();
            ui->negative2->hide();
            logic->processButtons(ui->positive0, ui->negative0);
            break;
        case 1:
            ui->status1->hide();
            ui->positive0->hide();
            ui->negative0->hide();
            ui->positive2->hide();
            ui->negative2->hide();
            break;
        case 2:
            ui->status2->hide();
            ui->positive0->hide();
            ui->negative0->hide();
            ui->positive1->hide();
            ui->negative1->hide();
            break;
        default:
            break;
    }
}

Game::~Game() {
    delete ui;
}

void Game::updateCards() {
    ui->cards->setText(logic->myCards.join(' '));
}

void Game::updateCallingStatus(int id, bool call, bool someoneCalled, int myOrder) {
    auto text = call ? (someoneCalled ? "抢" : "叫") : (someoneCalled ? "不抢" : "不叫");
    switch (id) {
        case 0:
            ui->status0->setText(text);
            break;
        case 1:
            ui->status1->setText(text);
            break;
        case 2:
            ui->status2->setText(text);
            break;
        default:
            break;
    }
    if (myOrder == id + 1) {
        if (myOrder == 1) {
            logic->processButtons(ui->positive1, ui->negative1);
        } else {
            logic->processButtons(ui->positive2, ui->negative2);
        }
    } else if (id == 2) {
        logic->setLandlord();
    }
}
