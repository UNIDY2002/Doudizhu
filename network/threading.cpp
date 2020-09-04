#include "threading.h"

WaitForConnectionThread::WaitForConnectionThread(QObject *parent, QTcpSocket *socket, DecentralizedClient *policy) :
        QThread(parent), socket(socket), policy(policy) {}

WaitForConnectionThread::~WaitForConnectionThread() = default;

void WaitForConnectionThread::run() {
    emit policy->updateMessage("已发送连接请求");
    if (socket->waitForConnected() && socket->waitForReadyRead()) {
        auto message = read(socket);
        if (message.type == CONFIRM_CONNECTION) {
            bool ok;
            auto number = message.payload.toInt(&ok);
            if (ok) {
                if (number < 2) {
                    emit policy->updateMessage("连接成功，请稍候");
                    policy->waitForGameStarts();
                } else {
                    emit policy->updateMessage("人数已满，请重试");
                }
            } else {
                emit policy->updateMessage("连接失败，请重试");
            }
        } else {
            emit policy->updateMessage("连接失败，请重试");
        }
    } else {
        emit policy->updateMessage("连接超时，请重试");
        // TODO: why cannot exit with code 0 in this case?
    }
}

WaitForReadyReadThread::WaitForReadyReadThread(QObject *parent, QTcpSocket *socket, GameLogic *logic) :
        QThread(parent), socket(socket), logic(logic) {}

WaitForReadyReadThread::~WaitForReadyReadThread() = default;

// TODO: why isn't readyRead signal available?
[[noreturn]] void WaitForReadyReadThread::run() {
    while (true) {
        if (socket->waitForReadyRead()) {
            Message message;
            while ((message = read(socket)).type) {
                logic->onMessage(message);
            }
        }
    }
}