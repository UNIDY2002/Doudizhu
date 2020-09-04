#include "GameLogic.h"
#include "utils.h"

GameLogic::GameLogic(int order, const QStringList &cards, QObject *parent) :
        QObject(parent), order(order), cardPile(cards),
        myCards(cards.begin() + (order * 17), cards.begin() + ((order + 1) * 17)) {
    std::sort(myCards.begin(), myCards.end(), &cardCmp);
}

GameLogic::~GameLogic() = default;

void GameLogic::onMessage(const Message &message) {
    cout << "On message: " << message.type << " " << message.payload.toStdString() << endl;
    switch (message.type) {
        case CALL: {
            auto id = message.payload.toInt();
            auto lastSomeoneCalled = someoneCalled;
            someoneCalled = true;
            emit callingStatusUpdated(id, true, lastSomeoneCalled, order);
            callingStatus[id] = 1;
            break;
        }
        case NOT_CALL: {
            auto id = message.payload.toInt();
            emit callingStatusUpdated(id, false, someoneCalled, order);
            callingStatus[id] = -1;
            break;
        }
        default:
            break;
    }
}

void GameLogic::processButtons(QPushButton *positive, QPushButton *negative) {
    positive->setText(someoneCalled ? "抢" : "叫");
    negative->setText(someoneCalled ? "不抢" : "不叫");
    positive->setEnabled(true);
    negative->setEnabled(true);
    connect(positive, &QPushButton::clicked, [=]() {
        call(true);
        positive->setEnabled(false);
        negative->hide();
    });
    connect(negative, &QPushButton::clicked, [=]() {
        call(false);
        negative->setEnabled(false);
        positive->hide();
    });
}

void GameLogic::call(bool choice) {
    if (choice) {
        someoneCalled = true;
        callingStatus[order] = 1;
        emit sendMessage({CALL, QString::number(order)});
    } else {
        callingStatus[order] = -1;
        emit sendMessage({NOT_CALL, QString::number(order)});
    }
}