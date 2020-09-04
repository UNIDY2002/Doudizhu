#include "utils.h"

void write(QTcpSocket *socket, const Message &message) {
    auto array = QByteArray();
    auto data = QString::number(message.type) + " " + message.payload;
    array.append(QString("%1").arg(data.size(), 4, 16, QLatin1Char('0')));
    array.append(data);
    // cout << "Writing " << array.toStdString() << endl;
    socket->write(array);
}

Message read(QTcpSocket *socket) {
    bool ok;
    auto length = socket->read(4).toInt(&ok, 16);
    if (ok) {
        auto array = socket->read(length);
        // cout << "Received " << array.toStdString() << endl;
        auto pieces = array.split(' ');
        if (!pieces.empty()) {
            auto type = static_cast<Type>(pieces[0].toInt());
            auto payload = pieces.size() > 1 ? QString(pieces[1]) : "";
            return {type, payload};
        }
    }
    return {};
}


