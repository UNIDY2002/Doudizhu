#include "threading.h"

WaitForConnectionThread::WaitForConnectionThread(QObject *parent, QTcpSocket *socket, DecentralizedClient *policy) :
        QThread(nullptr), socket(socket), policy(policy) {}

WaitForConnectionThread::~WaitForConnectionThread() = default;

void WaitForConnectionThread::run() {
    emit policy->postMessageToWelcome("已发送连接请求");
    if (socket->waitForConnected() && socket->waitForReadyRead(100)) {
        auto message = read(socket);
        if (message.type == CONFIRM_CONNECTION) {
            bool ok;
            auto number = message.payload.toInt(&ok);
            if (ok) {
                if (number < 2) {
                    emit policy->postMessageToWelcome("连接成功，请稍候");
                    policy->waitForGameStarts();
                } else {
                    emit policy->postMessageToWelcome("人数已满，请重试");
                }
            } else {
                emit policy->postMessageToWelcome("连接失败，请重试");
            }
        } else {
            emit policy->postMessageToWelcome("连接失败，请重试");
        }
    } else {
        emit policy->postMessageToWelcome("连接超时，请重试");
        // TODO: why cannot exit with code 0 in this case?
    }
}

WaitForReadyReadThread::WaitForReadyReadThread(DecentralizedClient *parent, QTcpSocket *socket, GameLogic *logic) :
        QThread(nullptr), policy(parent), socket(socket), logic(logic) {}

WaitForReadyReadThread::~WaitForReadyReadThread() = default;

// TODO: why isn't readyRead signal available?
void WaitForReadyReadThread::run() {
    while (true) {
        if (socket->waitForReadyRead(100)) {
            Message message;
            while ((message = read(socket)).type) {
                if (message.type == GAME_STARTS) {
                    policy->processGameStartsMessage(message, logic);
                    return;
                } else {
                    logic->onMessage(message);
                }
            }
        }
    }
}