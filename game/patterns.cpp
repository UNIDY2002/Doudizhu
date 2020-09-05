#include "patterns.h"
#include "utils.h"

Single::Single(const QStringList &other) : card(other[0]) {}

bool Single::operator<(const QStringList &other) {
    return other.size() == 1 && cardCmp(card, other[0]);
}

template<>
bool matches<Single>(const QStringList &cards) { return cards.size() == 1; }

Pattern *makePattern(const QStringList &cards) {
    if (matches<Single>(cards)) {
        return new Single(cards);
    }
    return nullptr;
}
