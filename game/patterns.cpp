#include "patterns.h"
#include "utils.h"

Pattern::~Pattern() = default;

/* Bomb */

template<>
bool matches<Bomb>(const QStringList &cards) {
    return cards.size() == 4 && cards[0] == cards[1] && cards[1] == cards[2] && cards[2] == cards[3];
}

Bomb::Bomb(const QStringList &other) : card(other[0]) {}

bool Bomb::operator<(const QStringList &other) {
    return matches<Bomb>(other) && cardCmp(card, other[0]);
}

/* Single */

template<>
bool matches<Single>(const QStringList &cards) { return cards.size() == 1; }

Single::Single(const QStringList &other) : card(other[0]) {}

bool Single::operator<(const QStringList &other) {
    return matches<Bomb>(other) || matches<Single>(other) && cardCmp(card, other[0]);
}

/* Pair */

template<>
bool matches<Pair>(const QStringList &cards) { return cards.size() == 2 && cards[0] == cards[1]; }

Pair::Pair(const QStringList &other) : card(other[0]) {}

bool Pair::operator<(const QStringList &other) {
    return matches<Bomb>(other) || matches<Pair>(other) && cardCmp(card, other[0]);
}

/* Triple */

template<>
bool matches<Triple>(const QStringList &cards) {
    switch (cards.size()) {
        case 3:
            return cards[0] == cards[1] && cards[1] == cards[2];
        case 4:
            return cards.count(cards[2]) == 3;
        case 5:
            return cards.count(cards[2]) == 3 &&
                   (cards.count(cards[0]) == 2 || cards.count(cards[4]) == 2);
        default:
            return false;
    }
}

Triple::Triple(const QStringList &other) : major(other[2]), subpattern(other.size()) {}

bool Triple::operator<(const QStringList &other) {
    return matches<Bomb>(other) || matches<Triple>(other) && subpattern == other.size() && cardCmp(major, other[2]);
}

/* Straight */

template<>
bool matches<Straight>(const QStringList &cards) {
    if (cards.size() >= 5 && cards.size() <= 12) {
        for (int i = 1; i < cards.size(); ++i)
            if (cardWeight(cards[i]) != cardWeight(cards[i - 1]) + 1 || cards[i] == "2")
                return false;
        return true;
    } else {
        return false;
    }
}

Straight::Straight(const QStringList &other) : major(other[0]), length(other.size()) {}

bool Straight::operator<(const QStringList &other) {
    return matches<Bomb>(other) || matches<Straight>(other) && length == other.size() && cardCmp(major, other[0]);
}

/* DoubleStraight */

template<>
bool matches<DoubleStraight>(const QStringList &cards) {
    if (cards.size() >= 6 && cards.size() <= 20 && cards.size() % 2 == 0) {
        for (int i = 0; i < cards.size() / 2; ++i)
            if (cards[i * 2] != cards[i * 2 + 1] || cards[i * 2] == "2")
                return false;
        for (int i = 1; i < cards.size() / 2; ++i)
            if (cardWeight(cards[i * 2]) != cardWeight(cards[i * 2 - 1]) + 1)
                return false;
        return true;
    } else {
        return false;
    }
}

DoubleStraight::DoubleStraight(const QStringList &other) : major(other[0]), pairs(other.size() / 2) {}

bool DoubleStraight::operator<(const QStringList &other) {
    return matches<Bomb>(other) ||
           matches<DoubleStraight>(other) && pairs == other.size() / 2 && cardCmp(major, other[0]);
}

/* Quartet */

template<>
bool matches<Quartet>(const QStringList &cards) {
    switch (cards.size()) {
        case 6:
            return cards.count(cards[2]) == 4;
        case 8:
            return cards.count(cards[0]) == 2 && (cards.count(cards[2]) == 4 && cards.count(cards[6]) == 2 ||
                                                  cards.count(cards[2]) == 2 && cards.count(cards[4]) == 4) ||
                   cards.count(cards[0]) == 4 && cards.count(cards[4]) == 2 && cards.count(cards[6]) == 2;
        default:
            return false;
    }
}

Quartet::Quartet(const QStringList &other) : major(other.count(other[0]) == 2 ? other[4] : other[0]) {}

bool Quartet::operator<(const QStringList &other) {
    return matches<Bomb>(other) ||
           matches<Quartet>(other) && cardCmp(major, (other.count(other[0]) == 2 ? other[4] : other[0]));
}

/* Plane */

template<>
bool matches<Plane>(const QStringList &cards) {
    QStringList singles, doubles, triples;
    for (const auto &card : cards) {
        switch (cards.count(card)) {
            case 1:
                singles.append(card);
                break;
            case 2:
                if (!doubles.contains(card))
                    doubles.append(card);
                break;
            case 3:
                if (!triples.contains(card))
                    triples.append(card);
                break;
            default:
                return false;
        }
    }
    if (!singles.isEmpty() && !doubles.isEmpty())
        return false;
    if (singles.size() + doubles.size() != triples.size() && singles.size() + doubles.size() != 0)
        return false;
    return triples.size() > 1 && cardWeight(triples[triples.size() - 1]) - cardWeight(triples[0]) == triples.size() - 1;
}

Plane::Plane(const QStringList &other) {
    QStringList triples;
    for (const auto &card : other)
        if (other.count(card) == 3 && !triples.contains(card))
            triples.append(card);
    major = triples[0];
    groups = triples.size();
    subpattern = other.size() / groups;
}

bool Plane::operator<(const QStringList &other) {
    if (matches<Bomb>(other)) {
        return true;
    } else if (matches<Plane>(other)) {
        Plane pattern = Plane(other);
        return cardCmp(major, pattern.major) && groups == pattern.groups && subpattern == pattern.subpattern;
    } else {
        return false;
    }
}

/* Factory */

Pattern *makePattern(const QStringList &cards) {
    if (matches<Single>(cards)) {
        return new Single(cards);
    } else if (matches<Pair>(cards)) {
        return new Pair(cards);
    } else if (matches<Triple>(cards)) {
        return new Triple(cards);
    } else if (matches<Straight>(cards)) {
        return new Straight(cards);
    } else if (matches<DoubleStraight>(cards)) {
        return new DoubleStraight(cards);
    } else if (matches<Quartet>(cards)) {
        return new Quartet(cards);
    } else if (matches<Plane>(cards)) {
        return new Plane(cards);
    } else if (matches<Bomb>(cards)) {
        return new Bomb(cards);
    } else {
        return nullptr;
    }
}