#include "legocontrol.h"

legoControl::legoControl(Device *d) : device(d)
{

}

void legoControl::control(const QByteArray &command)
{
    if (active && device) {
        qDebug() << "legoControl::control" << command.toHex();
        device->sendCommand(command);
    } else {
        qWarning() << "legoControl::control fail!";
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
