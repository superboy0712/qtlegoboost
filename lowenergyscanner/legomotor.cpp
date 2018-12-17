#include "legomotor.h"

legoMotor::legoMotor()
{
}

void legoMotor::control(QLowEnergyService *service, const QLowEnergyCharacteristic &characteristic, const QByteArray &command)
{
    qDebug() << "legoMotor::control" << command.toHex();
    service->writeCharacteristic(characteristic, command);
}
