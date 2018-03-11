#include "utils/util.h"

#include <QIODevice>
#include <QJsonDocument>
#include <QDataStream>

QByteArray toStream(int cmdType, QJsonObject &jo)
{
    QByteArray msgSend;
    QDataStream stream(&msgSend, QIODevice::WriteOnly);
    QByteArray msgJo = QJsonDocument(jo).toJson();
    stream << cmdType;
    stream.writeRawData(msgJo.toStdString().c_str(), msgJo.size());
    return msgSend;
}
