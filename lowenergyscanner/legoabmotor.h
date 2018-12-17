#ifndef LEGOABMOTOR_H
#define LEGOABMOTOR_H

#include <legomotor.h>

class legoABmotor : public legoMotor
{
public:
    legoABmotor();
    static void move(QLowEnergyService *service, const QLowEnergyCharacteristic &characteristic, quint16 ms = 1000, qint8 speed_primary = 100, qint8 speed_secondary = 100);
};

#endif // LEGOABMOTOR_H
