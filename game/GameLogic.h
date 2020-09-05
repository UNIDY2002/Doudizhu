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

    int landlordId = -1;

    int lastDiscardId = -1;

    QStringList lastDiscards{};

    QStringList cardPile;

    QStringList myCards;

    friend class Game;

public:
    explicit GameLogic(int order, const QStringList &cards, QObject *parent = nullptr);

    ~GameLogic() override;

signals:

    void sendMessage(const Message &message);

    void callingStatusUpdated(int id, bool call, bool someoneCalled, int myOrder);

    void cardsUpdated();

    void cardsEnabled();

public slots:

    void onMessage(const Message &message);

private:

    void processButtons(QPushButton *positive, QPushButton *negative);

    void setLandlord();

    void call(bool choice);

    void discard(const QStringList &cards);

    void pass();

};

#endif //DOUDIZHU_GAMELOGIC_H
