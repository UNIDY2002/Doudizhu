#include "patterns.h"
#include "utils.h"

Pattern::~Pattern() = default;

/* Single */

template<>
bool matches<Single>(const QStringList &cards) { return cards.size() == 1; }

Single::Single(const QStringList &other) : card(other[0]) {}

bool Single::operator<(const QStringList &other) {
    return matches<Single>(other) && cardCmp(card, other[0]);
}

/* Pair */

template<>
bool matches<Pair>(const QStringList &cards) { return cards.size() == 2 && cards[0] == cards[1]; }

Pair::Pair(const QStringList &other) : card(other[0]) {}

bool Pair::operator<(const QStringList &other) {
    return matches<Pair>(other) && cardCmp(card, other[0]);
}

/* Factory */

Pattern *makePattern(const QStringList &cards) {
    if (matches<Single>(cards)) {
        return new Single(cards);
    } else if (matches<Pair>(cards)) {
        return new Pair(cards);
    }
    return nullptr;
}