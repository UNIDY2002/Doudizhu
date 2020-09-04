#ifndef DOUDIZHU_NETWORKPOLICY_H
#define DOUDIZHU_NETWORKPOLICY_H

#include <QtCore/QObject>
#include <network/utils.h>
#include <game/GameLogic.h>

class NetworkPolicy : public QObject {
Q_OBJECT

public:
    explicit NetworkPolicy(QObject *parent = nullptr);

    ~NetworkPolicy() override;

    virtual void afterLinking() = 0;

    virtual void prepare(GameLogic *logic) = 0;

signals:

    // This message refers to the message shown on the preparation screen.
    void updateMessage(const QString &msg);

    void gameStarts(int order, const QStringList &cards);

};

#endif //DOUDIZHU_NETWORKPOLICY_H
