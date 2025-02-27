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

    int otherNumber[3]{17, 17, 17};

    int restartCnt = 0;

    QStringList lastDiscards{};

    QStringList cardPile;

    QStringList myCards;

    friend class Game;

public:
    explicit GameLogic(int order, const QStringList &cards, QObject *parent = nullptr);

    ~GameLogic() override;

    void onMessage(const Message &message);

signals:

    void sendMessage(const Message &message);

    void callingStatusUpdated(int id, bool call, bool someoneCalled);

    void cardsUpdated();

    void cardsEnabled();

    void metaRefreshed();

    void messageUpdated(int id, const QString &message);

    void gameStops();

    void forceExit();

    void buttonsReset();

    void hotswap();

    void hotswapWithInitParams(int order, const QStringList &cards);

private:

    void setLandlord();

    void call(bool choice);

    void discard(const QStringList &cards);

    void pass();

    void restart();

    void quit();

};

#endif //DOUDIZHU_GAMELOGIC_H
