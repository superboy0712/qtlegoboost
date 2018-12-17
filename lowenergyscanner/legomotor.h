#ifndef LEGOMOTOR_H
#define LEGOMOTOR_H

#include <QLowEnergyController>
#include <QBluetoothServiceInfo>

class legoMotor
{
public:
    legoMotor();
    static void control(QLowEnergyService *service, const QLowEnergyCharacteristic &characteristic, const QByteArray &command);
    static qint8 speedAbs(qint8 speed);
};

#endif // LEGOMOTOR_H
