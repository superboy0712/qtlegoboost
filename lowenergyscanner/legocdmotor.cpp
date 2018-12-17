#include "legocdmotor.h"
#include "constant.h"
#include <QtMath>
#include <QtEndian>

legoCDmotor::legoCDmotor()
{

}

void legoCDmotor::rotate(QLowEnergyService *service, const QLowEnergyCharacteristic
&characteristic, qint32 angle, qint8 speed_primary, MORTOR_PORT port)
{
    const QString length = "0e";

    angle = qint32(round(angle));
    if (angle < 0) {
        angle = -angle;
        speed_primary = - speed_primary;
    }

    QString motorPort = LEGO_CONSTANTS::PORT_D;
    if (port == MORTOR_PORT::LEGO_C_MOTOR)
        motorPort = LEGO_CONSTANTS::PORT_C;

    QString command =
            length + \
            LEGO_CONSTANTS::PACKET_VER + \
            LEGO_CONSTANTS::MSG_SET_PORT_VAL + \
            motorPort + \
            LEGO_CONSTANTS::MOVEMENT_TYPE + \
            LEGO_CONSTANTS::ANGLED_SINGLE + \
            QString::number(qToBigEndian<quint32>(angle), 16) + \
            QString("%1").arg(speedAbs(speed_primary), 2, 16, QLatin1Char('0')).right(2) + \
            LEGO_CONSTANTS::TRAILER;

    qDebug() << "legoCDmotor::rotate" << command;
    control(service, characteristic, QByteArray::fromHex(command.toUtf8()));
}
