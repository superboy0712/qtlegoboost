#ifndef LEGOCONTROL_H
#define LEGOCONTROL_H

#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
#include <QObject>

#include <device.h>

class legoControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ readActive)
    Q_PROPERTY(bool status READ readStatus)

public:
    explicit legoControl(Device *d);
    void control(const QByteArray &command);
    bool readActive() {return active;}
    bool readStatus() {return status;}

public slots:
    void setActive(int value);
    void setStatus(int value);

private:
    Device *device = nullptr;
    bool active = true;
    bool status = false;
};

#endif // LEGOCONTROL_H
