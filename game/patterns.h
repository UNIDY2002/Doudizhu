#ifndef DOUDIZHU_PATTERNS_H
#define DOUDIZHU_PATTERNS_H

#include <QtCore/QStringList>

// Assumes that all string lists below are sorted.

class Pattern {
public:
    virtual bool operator<(const QStringList &other) = 0;
};


class Single : public Pattern {
    QString card;

public:

    explicit Single(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

template<typename T>
bool matches(const QStringList &cards);

Pattern *makePattern(const QStringList &cards);

#endif //DOUDIZHU_PATTERNS_H
