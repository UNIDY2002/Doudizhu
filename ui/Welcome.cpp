#include "Welcome.h"

Welcome::Welcome(QWidget *parent) : QWidget(parent), ui(new Ui::Welcome) {
    ui->setupUi(this);
}

Welcome::~Welcome() {
    delete ui;
    if (!networkPolicy) delete networkPolicy;
}

void Welcome::linkNetworkPolicy() {
    connect(networkPolicy, &NetworkPolicy::updateMessage, ui->msg, &QLabel::setText);
    networkPolicy->afterLinking();
}

void Welcome::disableButtons() {
    ui->serverButton->setEnabled(false);
    ui->clientButton->setEnabled(false);
}

void Welcome::setupDecentralizedServer() {
    if (!networkPolicy)
        networkPolicy = new DecentralizedServer(QHostAddress::Any, PORT, this);
    linkNetworkPolicy();
}

void Welcome::setupDecentralizedClient() {
    if (!networkPolicy)
        networkPolicy = new DecentralizedClient(QHostAddress::LocalHost, PORT, this);
    linkNetworkPolicy();
}
