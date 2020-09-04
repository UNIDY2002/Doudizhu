#include "DecentralizedClient.h"
#include "utils.h"
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
        emit updateMessage("游戏开始");
    } else {
        emit updateMessage("连接失败，请重试");
    }
}
