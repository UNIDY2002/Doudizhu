#include <QtWidgets/QMessageBox>
#include <QtCore/QRandomGenerator>
#include <game/utils.h>
#include <network/utils.h>
#include "DecentralizedServer.h"

DecentralizedServer::DecentralizedServer(const QHostAddress &address, quint16 port, QObject *parent) :
        NetworkPolicy(parent), server(new QTcpServer(parent)) {
    if (server->listen(address, port)) {
        connect(server, &QTcpServer::newConnection, this, &DecentralizedServer::addClient);
    } else {
        QMessageBox::critical(nullptr, "警告", "连接未成功。");
    }
}

DecentralizedServer::~DecentralizedServer() {
    server->close();
    delete server;
}

QPair<int, QStringList> DecentralizedServer::sendInitMessages() {
    int perms[6][3]{0, 1, 2,
                    0, 2, 1,
                    1, 0, 2,
                    1, 2, 0,
                    2, 0, 1,
                    2, 1, 0};
    int randPerm = QRandomGenerator::global()->bounded(6);
    auto cards = genCards();
    for (int i = CARD_NUM; i; --i) {
        int j = QRandomGenerator::global()->bounded(i);
        swap(cards[i - 1], cards[j]);
    }
    auto stringifiedCards = QString();
    for (const auto &card:cards) {
        stringifiedCards.append(',');
        stringifiedCards.append(card);
    }
    for (int i = 0; i < 2; ++i) {
        write(clients[i], {GAME_STARTS, QString::number(perms[randPerm][i]) + stringifiedCards});
    }
    return qMakePair(perms[randPerm][2], cards);
}

void DecentralizedServer::addClient() {
    auto client = server->nextPendingConnection();
    write(client, {CONFIRM_CONNECTION, QString::number(clientNumber)});
    if (clientNumber < 2) {
        clients[clientNumber++] = client;
        emit postMessageToWelcome("等待连接 (" + QString::number(clientNumber) + "/2)");
        if (clientNumber == 2) {
            auto initData = sendInitMessages();
            emit gameStarts(initData.first, initData.second);
        }
    }
}

void DecentralizedServer::linkWithWelcome() {
    emit postMessageToWelcome("等待连接 (0/2)");
}

void DecentralizedServer::linkWithLogic(GameLogic *logic) {
    for (int i = 0; i < 2; ++i) {
        connect(clients[i], &QTcpSocket::readyRead, [=]() {
            Message message;
            while ((message = read(clients[i])).type) {
                write(clients[1 - i], message);
                logic->onMessage(message);
            }
        });
    }
    connect(logic, &GameLogic::sendMessage, [&](const Message &message) {
        for (auto &client : clients) write(client, message);
    });
}

void DecentralizedServer::unlink() {
    for (const auto &client : clients) client->disconnect();
}
