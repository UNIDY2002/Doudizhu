#include "Welcome.h"

Welcome::Welcome(QWidget *parent) : QWidget(parent), ui(new Ui::Welcome) {
    ui->setupUi(this);
}

Welcome::~Welcome() {
    delete ui;
    if (!networkPolicy) delete networkPolicy;
}

void Welcome::setupDecentralizedServer() {
    if (!networkPolicy)
        networkPolicy = new DecentralizedServer(QHostAddress::Any, PORT, this);
}

void Welcome::setupDecentralizedClient() {
    if (!networkPolicy)
        networkPolicy = new DecentralizedClient(QHostAddress::LocalHost, PORT, this);
}
