#ifndef LEGOCDMOTOR_H
#define LEGOCDMOTOR_H

#include <legomotor.h>

enum class MORTOR_PORT {LEGO_C_MOTOR, LEGO_D_MOTOR};

class legoCDmotor : public legoMotor
{
public:
    legoCDmotor();
    static void rotate(QLowEnergyService *service, const QLowEnergyCharacteristic &characteristic, qint32 angle = 360, qint8 speed_primary = 100, MORTOR_PORT port = MORTOR_PORT::LEGO_D_MOTOR);
};

#endif // LEGOCDMOTOR_H
