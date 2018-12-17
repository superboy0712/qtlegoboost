#include "legomotor.h"
#include <QtMath>

legoMotor::legoMotor()
{
}

void legoMotor::control(QLowEnergyService *service, const QLowEnergyCharacteristic &characteristic, const QByteArray &command)
{
    qDebug() << "legoMotor::control" << command.toHex();
    service->writeCharacteristic(characteristic, command);
}

qint8 legoMotor::speedAbs(qint8 speed)
{
    if (speed < -100)
        speed = -100;

    if (speed > 100)
        speed = 100;

    return qCeil(speed);
}
