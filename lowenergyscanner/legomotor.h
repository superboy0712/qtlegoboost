#ifndef LEGOMOTOR_H
#define LEGOMOTOR_H

#include <legocontrol.h>

class legoMotor : public legoControl
{
public:
    explicit legoMotor(Device *d);
    static qint8 speedAbs(qint8 speed);
};

#endif // LEGOMOTOR_H
