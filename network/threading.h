#ifndef DOUDIZHU_THREADING_H
#define DOUDIZHU_THREADING_H

#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>
#include "NetworkPolicy.h"

class WaitForConnectionThread : public QThread {

    QTcpSocket *socket;

    NetworkPolicy *policy;

protected:
    void run() override;

public:
    WaitForConnectionThread(QObject *parent, QTcpSocket *socket, NetworkPolicy *policy);

    ~WaitForConnectionThread() override;
};

#endif //DOUDIZHU_THREADING_H
