#include "GameLogic.h"
#include "utils.h"

#include <iostream>

using std::cout;
using std::endl;

GameLogic::GameLogic(int order, const QStringList &cards, QObject *parent) :
        QObject(parent), order(order), cardPile(cards),
        myCards(cards.begin() + (order * 17), cards.begin() + ((order + 1) * 17)) {
    std::sort(myCards.begin(), myCards.end(), &cardCmp);
}

GameLogic::~GameLogic() = default;
