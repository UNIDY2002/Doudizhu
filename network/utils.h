#ifndef DOUDIZHU_NETWORK_UTILS_H
#define DOUDIZHU_NETWORK_UTILS_H

#include <QtCore/QString>
#include <QtNetwork/QTcpSocket>

#include <iostream>

using std::cout;
using std::endl;

enum Type {
    undefined,
    DUMMY_MESSAGE,
    CONFIRM_CONNECTION,
    GAME_STARTS,
};

struct Message {
    Type type;
    QString payload;
};

void write(QTcpSocket *socket, const Message &message);

Message read(QTcpSocket *socket);

#endif //DOUDIZHU_NETWORK_UTILS_H
