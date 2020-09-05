#ifndef DOUDIZHU_PATTERNS_H
#define DOUDIZHU_PATTERNS_H

#include <QtCore/QStringList>

// Assumes that all string lists below are sorted.

class Pattern {
public:
    virtual ~Pattern() = 0;

    virtual bool operator<(const QStringList &other) = 0;
};


class Single : public Pattern {
    QString card;

public:

    explicit Single(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

class Pair : public Pattern {
    QString card;

public:

    explicit Pair(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

class Triple : public Pattern {
    QString major;

    int subpattern;

public:

    explicit Triple(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

class Straight : public Pattern {
    QString major;

    int length;

public:

    explicit Straight(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

class DoubleStraight : public Pattern {
    QString major;

    int pairs;

public:

    explicit DoubleStraight(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

class Quartet : public Pattern {
    QString major;

public:

    explicit Quartet(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

class Plane : public Pattern {
    QString major;

    int groups;

    int subpattern;

public:

    explicit Plane(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

class Bomb : public Pattern {
    QString card;

public:

    explicit Bomb(const QStringList &other);

    bool operator<(const QStringList &other) override;
};

class JokerBomb : public Pattern {
public:
    bool operator<(const QStringList &other) override;
};

template<typename T>
bool matches(const QStringList &cards);

Pattern *makePattern(const QStringList &cards);

#endif //DOUDIZHU_PATTERNS_H
