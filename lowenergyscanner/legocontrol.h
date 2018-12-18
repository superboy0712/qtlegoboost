#ifndef LEGOCONTROL_H
#define LEGOCONTROL_H

#include <QLowEnergyController>
#include <QBluetoothServiceInfo>

#include <QObject>

class legoControl : public QObject
{
    Q_OBJECT

public:
    legoControl();
    static void control(QLowEnergyService *service, const QLowEnergyCharacteristic &characteristic, const QByteArray &command);

public slots:
    void setActive(int value);
    void setStatus(int value);

private:
    static bool active;
    static bool status;
};

#endif // LEGOCONTROL_H
