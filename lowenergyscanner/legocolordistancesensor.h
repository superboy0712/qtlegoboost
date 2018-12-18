#ifndef LEGOCOLORDISTANCESENSOR_H
#define LEGOCOLORDISTANCESENSOR_H

#include <legocontrol.h>

class legoColorDistanceSensor : public legoControl
{
    Q_OBJECT

public:
    explicit legoColorDistanceSensor(Device *d);
};

#endif // LEGOCOLORDISTANCESENSOR_H
