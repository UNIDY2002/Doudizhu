#include "DecentralizedClient.h"

DecentralizedClient::DecentralizedClient(const QHostAddress &address, quint16 port, QObject *parent) :
        NetworkPolicy(parent), client(new QTcpSocket(parent)) {
    client->connectToHost(address, port);
}

DecentralizedClient::~DecentralizedClient() {
    client->close();
    delete client;
}
