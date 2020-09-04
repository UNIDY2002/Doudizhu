#include "utils.h"

QStringList genCards() {
    QStringList result = QStringList();
    // Auto-generated code. DO NOT EDIT!
    result.push_back("A");
    result.push_back("A");
    result.push_back("A");
    result.push_back("A");
    result.push_back("2");
    result.push_back("2");
    result.push_back("2");
    result.push_back("2");
    result.push_back("3");
    result.push_back("3");
    result.push_back("3");
    result.push_back("3");
    result.push_back("4");
    result.push_back("4");
    result.push_back("4");
    result.push_back("4");
    result.push_back("5");
    result.push_back("5");
    result.push_back("5");
    result.push_back("5");
    result.push_back("6");
    result.push_back("6");
    result.push_back("6");
    result.push_back("6");
    result.push_back("7");
    result.push_back("7");
    result.push_back("7");
    result.push_back("7");
    result.push_back("8");
    result.push_back("8");
    result.push_back("8");
    result.push_back("8");
    result.push_back("9");
    result.push_back("9");
    result.push_back("9");
    result.push_back("9");
    result.push_back("10");
    result.push_back("10");
    result.push_back("10");
    result.push_back("10");
    result.push_back("J");
    result.push_back("J");
    result.push_back("J");
    result.push_back("J");
    result.push_back("Q");
    result.push_back("Q");
    result.push_back("Q");
    result.push_back("Q");
    result.push_back("K");
    result.push_back("K");
    result.push_back("K");
    result.push_back("K");
    result.push_back("Joker");
    result.push_back("Joker");
    return result;
}

int cardWeight(const QString &c) {
    switch (c[0].unicode()) {
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return c[0].unicode() - '3';
        case '1': // 10
            return 7;
        case 'J':
            return 8;
        case 'Q':
            return 9;
        case 'K':
            return 10;
        case 'A':
            return 11;
        case '2':
            return 12;
        default:
            return -1;
    }
}

bool cardCmp(const QString &x, const QString &y) {
    return cardWeight(x) < cardWeight(y);
}
