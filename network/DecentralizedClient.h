#ifndef DOUDIZHU_DECENTRALIZEDCLIENT_H
#define DOUDIZHU_DECENTRALIZEDCLIENT_H

#include <QtNetwork/QTcpSocket>
#include "NetworkPolicy.h"

class DecentralizedClient : public NetworkPolicy {

    QTcpSocket *client;

public:
    explicit DecentralizedClient(const QHostAddress &address, quint16 port, QObject *parent = nullptr);

    ~DecentralizedClient() override;

    void afterLinking() override;

};

#endif //DOUDIZHU_DECENTRALIZEDCLIENT_H
