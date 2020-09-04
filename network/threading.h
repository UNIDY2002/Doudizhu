#ifndef DOUDIZHU_THREADING_H
#define DOUDIZHU_THREADING_H

#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>
#include "DecentralizedClient.h"

class WaitForConnectionThread : public QThread {

    QTcpSocket *socket;

    DecentralizedClient *policy;

protected:
    void run() override;

public:
    WaitForConnectionThread(QObject *parent, QTcpSocket *socket, DecentralizedClient *policy);

    ~WaitForConnectionThread() override;
};

#endif //DOUDIZHU_THREADING_H
