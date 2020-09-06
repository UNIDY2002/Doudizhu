#include <QApplication>
#include <QtWidgets/QMessageBox>
#include "GameLogic.h"
#include "utils.h"

GameLogic::GameLogic(int order, const QStringList &cards, QObject *parent) :
        QObject(parent), order(order), cardPile(cards),
        myCards(cards.begin() + (order * 17), cards.begin() + ((order + 1) * 17)) {
    std::sort(myCards.begin(), myCards.end(), &cardCmp);
    cout << "You are " << order << endl;
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
            emit callingStatusUpdated(id, true, lastSomeoneCalled);
            break;
        }
        case NOT_CALL: {
            auto id = message.payload.toInt();
            callingStatus[id] = -1;
            emit callingStatusUpdated(id, false, someoneCalled);
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
            otherNumber[lastDiscardId] -= lastDiscards.size();
            if (otherNumber[lastDiscardId] <= 0) {
                cout << lastDiscardId << " won." << endl;
                emit messageUpdated(order, lastDiscardId == landlordId ? "地主胜利" : "农民胜利");
                emit messageUpdated(lastDiscardId, lastDiscards.join(" "));
                emit metaRefreshed();
                emit gameStops();
                break;
            } else if ((lastDiscardId + 1) % 3 == order) {
                emit cardsEnabled();
            }
            emit metaRefreshed();
            emit messageUpdated(lastDiscardId, lastDiscards.join(" "));
            emit messageUpdated(order, lastDiscards.join(" "));
            break;
        }
        case PASS: {
            auto id = message.payload.toInt();
            if ((id + 1) % 3 == order) emit cardsEnabled();
            emit messageUpdated(id, "不要");
            break;
        }
        case RESTART: {
            if (++restartCnt == 3) emit hotswap();
            break;
        }
        case QUIT: {
            emit messageUpdated(order, "有玩家退出，游戏结束");
            emit buttonsReset();
            break;
        }
        default:
            break;
    }
}

void GameLogic::setLandlord() {
    auto state = (callingStatus[1] + 1) + (callingStatus[2] + 1) / 2;
    int landlord[4]{0, 2, 1, 2};
    otherNumber[lastDiscardId = landlordId = landlord[state]] = 20;
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
        emit metaRefreshed();
    }
}

void GameLogic::discard(const QStringList &cards) {
    lastDiscardId = order;
    for (const auto &card : cards) myCards.removeOne(card);
    emit sendMessage({DISCARD, QString::number(order) + "," + cards.join(',')});
    emit cardsUpdated();
    if (myCards.empty()) {
        emit messageUpdated(order, "你赢了");
        emit gameStops();
    } else {
        emit messageUpdated(order, cards.join(' '));
    }
}

void GameLogic::pass() {
    emit sendMessage({PASS, QString::number(order)});
}

void GameLogic::restart() {
    emit sendMessage({RESTART});
    emit buttonsReset();
    if (++restartCnt == 3) emit hotswap();
}

void GameLogic::quit() {
    emit sendMessage({QUIT});
    emit forceExit();
}
