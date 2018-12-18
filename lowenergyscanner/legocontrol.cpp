#include "legocontrol.h"

bool legoControl::active = true;
bool legoControl::status = false;

legoControl::legoControl()
{

}

void legoControl::control(QLowEnergyService *service, const QLowEnergyCharacteristic &characteristic, const QByteArray &command)
{
    if (active) {
        qDebug() << "legoControl::control" << command.toHex();
        service->writeCharacteristic(characteristic, command);
    } else {
        qWarning() << "legoControl::control non-Active";
    }
}

void legoControl::setActive(int value)
{
    qDebug() << "legoControl::setActive" << value;
    active = value;
}

void legoControl::setStatus(int value)
{
    qDebug() << "legoControl::setStatus" << value;
    status = value;
}
