#ifndef DOUDIZHU_GAMELOGIC_H
#define DOUDIZHU_GAMELOGIC_H

#include <QtCore/QObject>

class GameLogic : public QObject {
Q_OBJECT

    int order;

    QStringList cardPile;

    QStringList myCards;

    friend class Game;

public:
    explicit GameLogic(int order, const QStringList &cards, QObject *parent = nullptr);

    ~GameLogic() override;

};

#endif //DOUDIZHU_GAMELOGIC_H
