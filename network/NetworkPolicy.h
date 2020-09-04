#ifndef DOUDIZHU_NETWORKPOLICY_H
#define DOUDIZHU_NETWORKPOLICY_H

#include <QtCore/QObject>

#include <iostream>

using std::cout;
using std::endl;

class NetworkPolicy : public QObject {
Q_OBJECT

public:
    explicit NetworkPolicy(QObject *parent = nullptr);

    ~NetworkPolicy() override;

    virtual void afterLinking() = 0;

signals:

    void updateMessage(const QString &msg);

};

#endif //DOUDIZHU_NETWORKPOLICY_H
