#include "legomotor.h"
#include <QtMath>

legoMotor::legoMotor(Device *d) : legoControl(d)
{
}

qint8 legoMotor::speedAbs(qint8 speed)
{
    if (speed < -100)
        speed = -100;

    if (speed > 100)
        speed = 100;

    return qCeil(speed);
}
