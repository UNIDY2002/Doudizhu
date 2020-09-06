#ifndef DOUDIZHU_WELCOME_H
#define DOUDIZHU_WELCOME_H

#include <QtWidgets/QWidget>
#include <network/NetworkPolicy.h>
#include <network/DecentralizedServer.h>
#include <network/DecentralizedClient.h>
#include "ui_welcome.h"

#define PORT 9042

namespace Ui {
    class Welcome;
}

class Welcome : public QWidget {
Q_OBJECT

    Ui::Welcome *ui;

    NetworkPolicy *networkPolicy = nullptr;

public:
    explicit Welcome(int param = 0, QWidget *parent = nullptr);

    ~Welcome() override;

public slots:

    void disableButtons();

    void setupDecentralizedServer();

    void setupDecentralizedClient();

    void startGame(int order, const QStringList &cards);
};

#endif //DOUDIZHU_WELCOME_H
