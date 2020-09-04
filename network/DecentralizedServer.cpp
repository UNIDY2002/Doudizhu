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

void DecentralizedServer::addClient() {
    auto client = server->nextPendingConnection();
    write(client, {CONFIRM_CONNECTION, QString::number(clientNumber)});
    if (clientNumber < 2) {
        clients[clientNumber++] = client;
        emit updateMessage("等待连接 (" + QString::number(clientNumber) + "/2)");
        if (clientNumber == 2) {
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
            emit gameStarts(perms[randPerm][2], cards);
        }
    }
}

void DecentralizedServer::afterLinking() {
    emit updateMessage("等待连接 (0/2)");
}
