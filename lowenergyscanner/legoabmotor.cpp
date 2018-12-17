#include "legoabmotor.h"
#include "constant.h"
#include <QtEndian>

legoABmotor::legoABmotor()
{

}

void legoABmotor::move(QLowEnergyService *service, const QLowEnergyCharacteristic
&characteristic, quint16 ms, qint8 speed_primary, qint8 speed_secondary)
{
    const QString length = "0d";
    QString command =
            length + \
            LEGO_CONSTANTS::PACKET_VER + \
            LEGO_CONSTANTS::MSG_SET_PORT_VAL + \
            LEGO_CONSTANTS::PORT_AB + \
            LEGO_CONSTANTS::MOVEMENT_TYPE + \
            LEGO_CONSTANTS::TIMED_GROUP + \
            QString::number(qToBigEndian<quint16>(ms), 16) + \
            QString("%1").arg(speedAbs(speed_primary), 2, 16, QLatin1Char('0')).right(2) + \
            QString("%1").arg(speedAbs(speed_secondary), 2, 16, QLatin1Char('0')).right(2) + \
            LEGO_CONSTANTS::TRAILER;

    qDebug() << "legoABmotor::move" << command;
    control(service, characteristic, QByteArray::fromHex(command.toUtf8()));
}
