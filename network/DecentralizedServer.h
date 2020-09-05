#ifndef DOUDIZHU_DECENTRALIZEDSERVER_H
#define DOUDIZHU_DECENTRALIZEDSERVER_H

#include <QtNetwork/QTcpServer>
#include "NetworkPolicy.h"

class DecentralizedServer : public NetworkPolicy {

    QTcpServer *server;

    QTcpSocket *clients[2]{};

    int clientNumber = 0;

    void addClient();

public:
    // Assumes that the listening will always succeed.
    explicit DecentralizedServer(const QHostAddress &address = QHostAddress::Any,
                                 quint16 port = 0,
                                 QObject *parent = nullptr);

    ~DecentralizedServer() override;

    QPair<int, QStringList> sendInitMessages();

    void afterLinking() override;

    void prepare(GameLogic *logic) override;

    void unlink();

};

#endif //DOUDIZHU_DECENTRALIZEDSERVER_H
