#ifndef DOUDIZHU_WELCOME_H
#define DOUDIZHU_WELCOME_H

#include <QtWidgets/QWidget>
#include <network/NetworkPolicy.h>
#include <network/DecentralizedServer.h>
#include "ui_welcome.h"

#define PORT 9042

#include <iostream>
#include <network/DecentralizedClient.h>

using std::cout;
using std::endl;

namespace Ui {
    class Welcome;
}

class Welcome : public QWidget {
Q_OBJECT

    Ui::Welcome *ui;

    NetworkPolicy *networkPolicy = nullptr;

public:
    explicit Welcome(QWidget *parent = nullptr);

    ~Welcome() override;

public slots:

    void setupDecentralizedServer();

    void setupDecentralizedClient();
};

#endif //DOUDIZHU_WELCOME_H
