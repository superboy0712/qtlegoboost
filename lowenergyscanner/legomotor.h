#ifndef LEGOMOTOR_H
#define LEGOMOTOR_H

#include <QLowEnergyController>
#include <QBluetoothServiceInfo>

class legoMotor
{
public:
    legoMotor();
    static void control(QLowEnergyService *service, const QLowEnergyCharacteristic &characteristic, const QByteArray &command);
};

#endif // LEGOMOTOR_H
