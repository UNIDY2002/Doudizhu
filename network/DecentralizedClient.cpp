#include <game/utils.h>
#include "DecentralizedClient.h"
#include "threading.h"

DecentralizedClient::DecentralizedClient(const QHostAddress &address, quint16 port, QObject *parent) :
        NetworkPolicy(parent), client(new QTcpSocket(parent)) {
    client->connectToHost(address, port);
}

DecentralizedClient::~DecentralizedClient() {
    client->close();
    delete client;
}

void DecentralizedClient::afterLinking() {
    auto thread = new WaitForConnectionThread(this, client, this);
    thread->start();
}

void DecentralizedClient::waitForGameStarts() {
    auto message = read(client);
    if (message.type == undefined) {
        if (client->waitForReadyRead()) {
            message = read(client);
        } else {
            emit updateMessage("等待超时，请重试");
            return;
        }
    }
    if (message.type == GAME_STARTS) {
        // auto segments = message.payload.split(',');
        // QString::split is only available after 5.14
        // The following serves as a workaround:
        auto byteArray = QByteArray();
        byteArray.append(message.payload);
        auto segments = byteArray.split(',');
        if (segments.size() == CARD_NUM + 1) {
            bool ok;
            auto order = segments[0].toInt(&ok);
            segments.pop_front();
            auto list = QStringList();
            for (const auto &s: segments) list.append(s);
            if (ok) {
                emit updateMessage("游戏开始");
                emit gameStarts(order, list);
                return;
            }
        }
    }
    emit updateMessage("连接失败，请重试");
}
