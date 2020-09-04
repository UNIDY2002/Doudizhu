#ifndef DOUDIZHU_GAME_UTILS_H
#define DOUDIZHU_GAME_UTILS_H

#include <QtCore/QStringList>

#include <iostream>

using std::cout;
using std::endl;

const int CARD_NUM = 54;

QStringList genCards();

int cardWeight(const QString &c);

bool cardCmp(const QString &x, const QString &y);

#endif //DOUDIZHU_GAME_UTILS_H
