#ifndef LEGOABMOTOR_H
#define LEGOABMOTOR_H

#include <legomotor.h>

class legoABmotor : public legoMotor
{
    Q_OBJECT

public:
    explicit legoABmotor(Device *d);
    Q_INVOKABLE void move(quint16 ms = 1000, qint8 speed_primary = 100, qint8 speed_secondary = 100);
};

#endif // LEGOABMOTOR_H
