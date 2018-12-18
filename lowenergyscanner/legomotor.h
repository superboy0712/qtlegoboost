#ifndef LEGOMOTOR_H
#define LEGOMOTOR_H

#include <legocontrol.h>

class legoMotor : public legoControl
{
public:
    legoMotor();
    static qint8 speedAbs(qint8 speed);
};

#endif // LEGOMOTOR_H
