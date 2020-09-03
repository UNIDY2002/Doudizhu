#include "DecentralizedServer.h"

DecentralizedServer::DecentralizedServer(const QHostAddress &address, quint16 port, QObject *parent) :
        NetworkPolicy(parent), server(new QTcpServer(parent)) {
    server->listen(address, port);
    connect(server, &QTcpServer::newConnection, this, &DecentralizedServer::addClient);
}

DecentralizedServer::~DecentralizedServer() {
    server->close();
    delete server;
}

void DecentralizedServer::addClient() {
    if (clientNumber < 2) {
        clients[clientNumber++] = server->nextPendingConnection();
        cout << clientNumber << endl;
    }
}
