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
        cardButton->setStyleSheet("QPushButton:hover{background-color:yellow;}"
                                  "QPushButton:checked{background-color:yellow;}");
        connect(cardButton, &QPushButton::clicked, this, &Game::checkValidity);
    }
    init();
}

Game::~Game() {
    delete ui;
    delete logic;
    for (const auto &cardButton : cardButtons) delete cardButton;
}

void Game::init() {
    connect(logic, &GameLogic::callingStatusUpdated, this, &Game::updateCallingStatus);
    connect(logic, &GameLogic::cardsUpdated, this, &Game::updateCards);
    connect(logic, &GameLogic::cardsEnabled, this, &Game::enableCards);
    connect(logic, &GameLogic::metaRefreshed, this, &Game::refreshMeta);
    connect(logic, &GameLogic::messageUpdated, this, &Game::updateMessage);
    connect(logic, &GameLogic::buttonsReset, this, &Game::resetButtons);
    connect(logic, &GameLogic::gameStops, this, &Game::onGameStops);
    connect(logic, &GameLogic::forceExit, this, &Game::onForceExit);
    connect(logic, &GameLogic::hotswap, this, &Game::onHotswap);
    connect(logic, &GameLogic::hotswapWithInitParams, this, &Game::onHotswapWithInitParams);
    policy->linkWithLogic(logic);

    ui->upper->setText("上家");
    ui->lower->setText("下家");
    ui->dipai->setText("底牌");
    ui->upperMessage->clear();
    ui->lowerMessage->clear();
    ui->myMessage->clear();
    ui->positiveAction->setText("？");
    ui->negativeAction->setText("？");
    ui->positiveAction->show();
    ui->negativeAction->show();
    if (logic->order == 0) processCallingButtons();
    updateCards();
}

void Game::processCallingButtons() {
    ui->positiveAction->setText(logic->someoneCalled ? "抢" : "叫");
    ui->negativeAction->setText(logic->someoneCalled ? "不抢" : "不叫");
    ui->positiveAction->setEnabled(true);
    ui->negativeAction->setEnabled(true);
    ui->positiveAction->disconnect();
    ui->negativeAction->disconnect();
    connect(ui->positiveAction, &QPushButton::clicked, [=]() {
        resetButtons();
        updateMessage(logic->order, ui->positiveAction->text());
        logic->call(true);
    });
    connect(ui->negativeAction, &QPushButton::clicked, [=]() {
        resetButtons();
        updateMessage(logic->order, ui->negativeAction->text());
        logic->call(false);
    });
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
    ui->positiveAction->show();
    ui->negativeAction->show();
    ui->positiveAction->setText("出牌");
    ui->negativeAction->setText("不要");
    ui->positiveAction->disconnect();
    ui->negativeAction->disconnect();
    connect(ui->positiveAction, &QPushButton::clicked, [=]() {
        resetButtons();
        logic->discard(pickedCardsCache);
    });
    connect(ui->negativeAction, &QPushButton::clicked, [=]() {
        resetButtons();
        logic->pass();
    });
    if (logic->lastDiscardId != logic->order)
        ui->negativeAction->setEnabled(true);
    for (const auto &cardButton : cardButtons) {
        cardButton->setEnabled(true);
    }
}

void Game::resetButtons() {
    ui->negativeAction->setEnabled(false);
    ui->positiveAction->setEnabled(false);
    ui->negativeAction->hide();
    ui->positiveAction->hide();
    for (const auto &cardButton : cardButtons) {
        cardButton->setEnabled(false);
        cardButton->setChecked(false);
    }
}

void Game::updateCallingStatus(int id, bool call, bool someoneCalled) {
    auto text = call ? (someoneCalled ? "抢" : "叫") : (someoneCalled ? "不抢" : "不叫");
    if (id == (logic->order + 1) % 3) {
        ui->lowerMessage->setText(text);
    } else if (id == (logic->order + 2) % 3) {
        ui->upperMessage->setText(text);
    }
    if (logic->order == id + 1) {
        processCallingButtons();
    } else if (id == 2) {
        logic->setLandlord();
        refreshMeta();
    }
}

void Game::checkValidity() {
    pickedCardsCache.clear();
    for (int i = 0; i < logic->myCards.size(); ++i)
        if (cardButtons[i]->isChecked())
            pickedCardsCache.append(logic->myCards[i]);

    std::sort(pickedCardsCache.begin(), pickedCardsCache.end(), cardCmp);

    if (logic->lastDiscardId == logic->order) {
        auto pattern = makePattern(pickedCardsCache);
        ui->positiveAction->setEnabled(pattern);
        delete pattern;
    } else {
        if (auto lastPattern = makePattern(logic->lastDiscards)) {
            ui->positiveAction->setEnabled(*lastPattern < pickedCardsCache);
            delete lastPattern;
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
        logic = new GameLogic(initData.first, initData.second);
        init();
    }
}

void Game::onHotswapWithInitParams(int order, const QStringList &cards) {
    if (dynamic_cast<DecentralizedClient *>(policy)) {
        delete logic;
        logic = new GameLogic(order, cards);
        init();
    }
}
