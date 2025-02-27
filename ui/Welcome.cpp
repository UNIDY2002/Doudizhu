#include "Welcome.h"
#include "Game.h"

Welcome::Welcome(int param, QWidget *parent) : QWidget(parent), ui(new Ui::Welcome) {
    ui->setupUi(this);
    if (param == 'S') {
        emit ui->serverButton->click();
    } else if (param == 'C') {
        emit ui->clientButton->click();
    }
}

Welcome::~Welcome() {
    delete ui;
    if (!networkPolicy) delete networkPolicy;
}

void Welcome::disableButtons() {
    ui->serverButton->setEnabled(false);
    ui->clientButton->setEnabled(false);
}

void Welcome::setupDecentralizedServer() {
    networkPolicy = new DecentralizedServer(QHostAddress::Any, PORT, this);
    connect(networkPolicy, &NetworkPolicy::postMessageToWelcome, ui->msg, &QLabel::setText);
    connect(networkPolicy, &NetworkPolicy::gameStarts, this, &Welcome::startGame);
    networkPolicy->linkWithWelcome();
}

void Welcome::setupDecentralizedClient() {
    networkPolicy = new DecentralizedClient(QHostAddress::LocalHost, PORT, this);
    connect(networkPolicy, &NetworkPolicy::postMessageToWelcome, ui->msg, &QLabel::setText);
    connect(networkPolicy, &NetworkPolicy::gameStarts, this, &Welcome::startGame);
    networkPolicy->linkWithWelcome();
}

void Welcome::startGame(int order, const QStringList &cards) {
    hide();
    (new Game(networkPolicy, order, cards))->show();
}
