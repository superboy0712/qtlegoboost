#ifndef LEGOCONTROL_H
#define LEGOCONTROL_H

#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
#include <QObject>

#include <device.h>

class legoControl : public QObject
{
    Q_OBJECT

public:
    explicit legoControl(Device *d);
    void control(const QByteArray &command);

public slots:
    void setActive(int value);
    void setStatus(int value);

private:
    Device *device = nullptr;
    bool active = true;
    bool status = false;
};

#endif // LEGOCONTROL_H
