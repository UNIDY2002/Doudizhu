#include <QtWidgets/QMessageBox>
#include "DecentralizedServer.h"
#include "utils.h"

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
    }
}

void DecentralizedServer::afterLinking() {
    emit updateMessage("等待连接 (0/2)");
}
