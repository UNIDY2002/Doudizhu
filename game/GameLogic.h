#ifndef DOUDIZHU_GAMELOGIC_H
#define DOUDIZHU_GAMELOGIC_H

#include <QtCore/QObject>
#include <network/utils.h>
#include <QtWidgets/QPushButton>

class GameLogic : public QObject {
Q_OBJECT

    int order;

    bool someoneCalled = false;
    int callingStatus[3]{}; // 1: positive; -1: negative

    QStringList cardPile;

    QStringList myCards;

    friend class Game;

public:
    explicit GameLogic(int order, const QStringList &cards, QObject *parent = nullptr);

    ~GameLogic() override;

signals:

    void sendMessage(const Message &message);

    void callingStatusUpdated(int id, bool call, bool someoneCalled, int myOrder);

public slots:

    void onMessage(const Message &message);

private:

    void processButtons(QPushButton *positive, QPushButton *negative);

private slots:

    void call(bool choice);

};

#endif //DOUDIZHU_GAMELOGIC_H
