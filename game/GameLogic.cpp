#include "GameLogic.h"
#include "utils.h"

GameLogic::GameLogic(int order, const QStringList &cards, QObject *parent) :
        QObject(parent), order(order), cardPile(cards),
        myCards(cards.begin() + (order * 17), cards.begin() + ((order + 1) * 17)) {
    std::sort(myCards.begin(), myCards.end(), &cardCmp);
    cout << "I am " << order << endl;
}

GameLogic::~GameLogic() = default;

void GameLogic::onMessage(const Message &message) {
    cout << "On message: " << message.type << " " << message.payload.toStdString() << endl;
    switch (message.type) {
        case CALL: {
            auto id = message.payload.toInt();
            auto lastSomeoneCalled = someoneCalled;
            someoneCalled = true;
            callingStatus[id] = 1;
            emit callingStatusUpdated(id, true, lastSomeoneCalled, order);
            break;
        }
        case NOT_CALL: {
            auto id = message.payload.toInt();
            callingStatus[id] = -1;
            emit callingStatusUpdated(id, false, someoneCalled, order);
            break;
        }
        case DISCARD: {
            auto bytes = QByteArray();
            bytes.append(message.payload);
            auto segments = bytes.split(',');
            if (segments.size() < 2) break;
            lastDiscardId = segments[0].toInt();
            segments.pop_front();
            lastDiscards = QStringList();
            for (const auto &segment: segments) {
                lastDiscards.append(segment);
            }
            if ((lastDiscardId + 1) % 3 == order) {
                emit cardsEnabled();
            }
        }
        case PASS: {
            if ((message.payload.toInt() + 1) % 3 == order) {
                emit cardsEnabled();
            }
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

void GameLogic::setLandlord() {
    auto state = (callingStatus[1] + 1) + (callingStatus[2] + 1) / 2;
    int landlord[4]{0, 2, 1, 2};
    lastDiscardId = landlordId = landlord[state];
    cout << "Landlord: " << landlordId << endl;
    if (landlordId == order) {
        myCards.append(cardPile[51]);
        myCards.append(cardPile[52]);
        myCards.append(cardPile[53]);
        std::sort(myCards.begin(), myCards.end(), &cardCmp);
        emit cardsUpdated();
        emit cardsEnabled();
    }
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
    if (order == 2) {
        setLandlord();
    }
}

void GameLogic::discard(const QStringList &cards) {
    lastDiscardId = order;
    emit sendMessage({DISCARD, QString::number(order) + "," + cards.join(',')});
}

void GameLogic::pass() {
    emit sendMessage({PASS, QString::number(order)});
}
