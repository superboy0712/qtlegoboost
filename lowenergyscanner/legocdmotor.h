#ifndef LEGOCDMOTOR_H
#define LEGOCDMOTOR_H

#include <legomotor.h>

class legoCDmotor : public legoMotor
{
    Q_OBJECT

public:
    explicit legoCDmotor(Device *d);

    enum class MORTOR_PORT {LEGO_C_MOTOR, LEGO_D_MOTOR};
    Q_ENUM(MORTOR_PORT)

    Q_INVOKABLE void rotate(qint32 angle = 360, qint8 speed_primary = 100, MORTOR_PORT port = MORTOR_PORT::LEGO_D_MOTOR);
};

#endif // LEGOCDMOTOR_H
