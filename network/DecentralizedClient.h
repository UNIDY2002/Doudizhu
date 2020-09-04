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

    /* possibly blocking method */
    void waitForGameStarts();

    void prepare(GameLogic *logic) override;

};

#endif //DOUDIZHU_DECENTRALIZEDCLIENT_H
