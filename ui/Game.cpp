#include <QtWidgets/QMessageBox>
#include <QCloseEvent>
#include <network/DecentralizedServer.h>
#include <network/DecentralizedClient.h>
#include <game/patterns.h>
#include <game/utils.h>
#include "Game.h"

Game::Game(NetworkPolicy *policy, int order, const QStringList &cards, QWidget *parent) :
        QWidget(parent), policy(policy), logic(new GameLogic(order, cards)), ui(new Ui::Game) {
    ui->setupUi(this);
    for (auto &cardButton : cardButtons) {
        ui->cardContainer->addWidget(cardButton = new QPushButton(this));
        cardButton->setMaximumWidth(40);
        cardButton->setEnabled(false);
        cardButton->hide();
        cardButton->setCheckable(true);
        connect(cardButton, &QPushButton::clicked, this, &Game::checkValidity);
    }
    init(logic);
}

Game::~Game() {
    delete ui;
}

void Game::init(GameLogic *gameLogic) {
    updateCards();
    policy->prepare(gameLogic);
    connect(gameLogic, &GameLogic::callingStatusUpdated, this, &Game::updateCallingStatus);
    connect(gameLogic, &GameLogic::cardsUpdated, this, &Game::updateCards);
    connect(gameLogic, &GameLogic::cardsEnabled, this, &Game::enableCards);
    connect(gameLogic, &GameLogic::metaRefreshed, this, &Game::refreshMeta);
    connect(gameLogic, &GameLogic::messageUpdated, this, &Game::updateMessage);
    connect(gameLogic, &GameLogic::buttonsReset, this, &Game::resetButtons);
    connect(gameLogic, &GameLogic::gameStops, this, &Game::onGameStops);
    connect(gameLogic, &GameLogic::forceExit, this, &Game::onForceExit);
    connect(gameLogic, &GameLogic::hotswap, this, &Game::onHotswap);
    connect(gameLogic, &GameLogic::hotswapWithInitParams, this, &Game::onHotswapWithInitParams);

    connect(ui->positiveAction, &QPushButton::clicked, [=]() {
        resetButtons();
        gameLogic->discard(pickedCardsCache);
    });

    connect(ui->negativeAction, &QPushButton::clicked, [=]() {
        resetButtons();
        gameLogic->pass();
    });

    // To be refactored...
    switch (gameLogic->order) {
        case 0:
            ui->status0->hide();
            ui->positive0->show();
            ui->negative0->show();
            ui->status1->show();
            ui->positive1->hide();
            ui->negative1->hide();
            ui->status2->show();
            ui->positive2->hide();
            ui->negative2->hide();
            ui->positive0->setText("?");
            ui->negative0->setText("?");
            ui->status1->setText("?");
            ui->status2->setText("?");
            gameLogic->processButtons(ui->positive0, ui->negative0);
            break;
        case 1:
            ui->status0->show();
            ui->positive0->hide();
            ui->negative0->hide();
            ui->status1->hide();
            ui->positive1->show();
            ui->negative1->show();
            ui->status2->show();
            ui->positive2->hide();
            ui->negative2->hide();
            ui->positive1->setText("?");
            ui->negative1->setText("?");
            ui->status0->setText("?");
            ui->status2->setText("?");
            break;
        case 2:
            ui->status0->show();
            ui->positive0->hide();
            ui->negative0->hide();
            ui->status1->show();
            ui->positive1->hide();
            ui->negative1->hide();
            ui->status2->hide();
            ui->positive2->show();
            ui->negative2->show();
            ui->positive2->setText("?");
            ui->negative2->setText("?");
            ui->status0->setText("?");
            ui->status1->setText("?");
            break;
        default:
            break;
    }

    ui->positiveAction->setText("出牌");
    ui->negativeAction->setText("不要");

    ui->upper->clear();
    ui->lower->clear();
    ui->dipai->clear();
    ui->upperMessage->clear();
    ui->lowerMessage->clear();
    ui->myMessage->clear();
}

void Game::closeEvent(QCloseEvent *event) {
    if (!readyToQuit && QMessageBox::information(this, "确认", "您真的要退出吗？", "确认", "取消")) {
        event->ignore();
    } else {
        event->accept();
    }
}

void Game::updateCards() {
    for (int i = 0; i < logic->myCards.size(); ++i) {
        cardButtons[i]->setText(logic->myCards[i]);
        cardButtons[i]->show();
    }
    for (int i = logic->myCards.size(); i < 20; ++i) {
        cardButtons[i]->hide();
    }
}

void Game::enableCards() {
    if (logic->lastDiscardId != logic->order)
        ui->negativeAction->setEnabled(true);
    for (const auto &cardButton : cardButtons) {
        cardButton->setEnabled(true);
    }
}

void Game::resetButtons() {
    ui->negativeAction->setEnabled(false);
    ui->positiveAction->setEnabled(false);
    for (const auto &cardButton : cardButtons) {
        cardButton->setEnabled(false);
        cardButton->setChecked(false);
    }
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
        refreshMeta();
    }
}

void Game::checkValidity() {
    pickedCardsCache.clear();
    for (int i = 0; i < logic->myCards.size(); ++i) {
        if (cardButtons[i]->isChecked()) {
            pickedCardsCache.append(logic->myCards[i]);
        }
    }

    std::sort(pickedCardsCache.begin(), pickedCardsCache.end(), cardCmp);

    if (logic->lastDiscardId == logic->order) {
        ui->positiveAction->setEnabled(makePattern(pickedCardsCache));
    } else {
        if (auto lastPattern = makePattern(logic->lastDiscards)) {
            ui->positiveAction->setEnabled(*lastPattern < pickedCardsCache);
        } else {
            ui->positiveAction->setEnabled(false);
        }
    }
}

void Game::refreshMeta() {
    ui->dipai->setText(logic->cardPile[51] + " " + logic->cardPile[52] + " " + logic->cardPile[53]);
    auto lowerId = (logic->order + 1) % 3;
    ui->lower->setText((lowerId == logic->landlordId ? "[地主] " : "[农民] ") +
                       QString::number(logic->otherNumber[lowerId]));
    auto upperId = (logic->order + 2) % 3;
    ui->upper->setText((upperId == logic->landlordId ? "[地主] " : "[农民] ") +
                       QString::number(logic->otherNumber[upperId]));
}

void Game::updateMessage(int id, const QString &message) {
    (id == logic->order ? ui->myMessage : id == (logic->order + 1) % 3 ? ui->lowerMessage : ui->upperMessage)
            ->setText(message);
}

void Game::onGameStops() {
    ui->negativeAction->setText("退出游戏");
    ui->positiveAction->setText("再来一局");
    ui->positiveAction->disconnect();
    ui->negativeAction->disconnect();
    ui->positiveAction->setEnabled(true);
    ui->negativeAction->setEnabled(true);
    connect(ui->positiveAction, &QPushButton::clicked, logic, &GameLogic::restart);
    connect(ui->negativeAction, &QPushButton::clicked, logic, &GameLogic::quit);
}

void Game::onForceExit() {
    readyToQuit = true;
    QApplication::closeAllWindows();
}

void Game::onHotswap() {
    if (auto decentralizedServer = dynamic_cast<DecentralizedServer *>(policy)) {
        decentralizedServer->unlink();
        delete logic;
        auto initData = decentralizedServer->sendInitMessages();
        init(logic = new GameLogic(initData.first, initData.second));
    }
}

void Game::onHotswapWithInitParams(int order, const QStringList &cards) {
    if (dynamic_cast<DecentralizedClient *>(policy)) {
        delete logic;
        init(logic = new GameLogic(order, cards));
    }
}
