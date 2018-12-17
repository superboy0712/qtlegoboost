#ifndef LEGOABMOTOR_H
#define LEGOABMOTOR_H

#include <legomotor.h>

class legoABmotor : public legoMotor
{
    static qint8 speedAbs(qint8 speed);
public:
    legoABmotor();
    static void move(QLowEnergyService *service, const QLowEnergyCharacteristic
    &characteristic, quint16 ms = 1000, qint8 speed_primary = 100, qint8 speed_secondary = 100);
};

#endif // LEGOABMOTOR_H
