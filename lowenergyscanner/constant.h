#ifndef CONSTANT_H
#define CONSTANT_H

#include <QString>

namespace LEGO_BOOST
{
    const QString PACKET_VER = "01";
    const QString MSG_SET_PORT_VAL = "81";
    const QString PORT_AB = "39";
    const QString MOVEMENT_TYPE = "11";
    const QString TIMED_SINGLE = "09";
    const QString TIMED_GROUP = "0a";
    const QString TRAILER = "647f03";
}

namespace LEGO_CONSTANTS = LEGO_BOOST;

#endif // CONSTANT_H
