#ifndef DOUDIZHU_NETWORKPOLICY_H
#define DOUDIZHU_NETWORKPOLICY_H

#include <QtCore/QObject>

#include <iostream>

using std::cout;
using std::endl;

class NetworkPolicy : public QObject {

public:
    explicit NetworkPolicy(QObject *parent = nullptr);

    ~NetworkPolicy() override;
};

#endif //DOUDIZHU_NETWORKPOLICY_H
