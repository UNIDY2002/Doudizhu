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
