/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DEVICE_H
#define DEVICE_H

#include <qbluetoothlocaldevice.h>
#include <QObject>
#include <QColor>
#include <QVariant>
#include <QList>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
#include "deviceinfo.h"
#include "serviceinfo.h"
#include "characteristicinfo.h"

QT_FORWARD_DECLARE_CLASS (QBluetoothDeviceInfo)
QT_FORWARD_DECLARE_CLASS (QBluetoothServiceInfo)

class Device: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant devicesList READ getDevices NOTIFY devicesUpdated)
    Q_PROPERTY(QVariant servicesList READ getServices NOTIFY servicesUpdated)
    Q_PROPERTY(QVariant characteristicList READ getCharacteristics NOTIFY characteristicsUpdated)
    Q_PROPERTY(QString update READ getUpdate WRITE setUpdate NOTIFY updateChanged)
    Q_PROPERTY(bool useRandomAddress READ isRandomAddress WRITE setRandomAddress NOTIFY randomAddressChanged)
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool controllerError READ hasControllerError)

    Q_PROPERTY(bool deviceVoltage READ deviceVoltage NOTIFY deviceVoltageChanged)
    Q_PROPERTY(double valueVoltage READ valueVoltage NOTIFY valueVoltageChanged)
    Q_PROPERTY(bool deviceDCSensor READ deviceDCSensor NOTIFY deviceDCSensorChanged)
    Q_PROPERTY(double valueDistance READ valueDistance NOTIFY valueDistanceChanged)
    Q_PROPERTY(QColor valueColor READ valueColor NOTIFY valueColorChanged)

    // https://github.com/JorgePe/BOOSTreveng/blob/master/Notifications.md
    enum MessageType {
        DeviceInformation = 0x01,
        DeviceShutdown = 0x02,
        PingPong = 0x03, //perhaps
        PortInformation = 0x04,
        ErrorOrMalformed = 0x05,
        Subscription = 0x41,
        SensorReading = 0x45,
        Acknowledgement = 0x47, //for Subscription
        PortChanged = 0x82
    };

    enum PortType {
        PortC = 0x01,
        PortD = 0x02,
        PortLED = 0x32,
        PortA = 0x37, //fixed motor
        PortB = 0x38, //fixed motor
        PortAB = 0x39, //fixed motors
        PortTiltSensor = 0x3a,
        PortAmperage = 0x3b,
        PortVoltage = 0x3c
    };

    enum DeviceType {
        DeviceVoltage = 0x14,
        DeviceAmperage = 0x15,
        DeviceLED = 0x17,
        DeviceDCSensor = 0x25, // Distance and Color
        DeviceInternalMotor = 0x26,
        DeviceExternalMotor = 0x27,
        DeviceTiltSensor = 0x28
    };

    enum DeviceEvent {
        None = 0x0, // detached
        Single = 0x01,
        Group = 0x02
    };

public:
    Device();
    ~Device();
    QVariant getDevices();
    QVariant getServices();
    QVariant getCharacteristics();
    QString getUpdate();
    bool state();
    bool hasControllerError() const;

    bool isRandomAddress() const;
    void setRandomAddress(bool newValue);

    // Lego
    void sendCommand(const QByteArray &command);

    bool deviceVoltage() const;
    double valueVoltage() const;

    bool deviceDCSensor() const;
    double valueDistance() const;
    QColor valueColor() const;

public slots:
    void startDeviceDiscovery();
    void scanServices(const QString &address);

    void connectToService(const QString &uuid);
    void disconnectFromDevice();

    void readVoltage(bool r);
    void readDistance(bool r);

private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanFinished();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);

    // QLowEnergyController realted
    void addLowEnergyService(const QBluetoothUuid &uuid);
    void deviceConnected();
    void errorReceived(QLowEnergyController::Error);
    void serviceScanDone();
    void deviceDisconnected();

    // QLowEnergyService related
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);

    void updateValue(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void confirmedDescriptorWrite();

private:
    void portInformation(const QByteArray &value);
    void acknowledgement(const QByteArray &value);
    void sensorReading(const QByteArray &value);
    void voltageReading(const QByteArray &value);
    void distanceReading(const QByteArray &value);
    void colorReading(const QByteArray &value);

Q_SIGNALS:
    void devicesUpdated();
    void servicesUpdated();
    void characteristicsUpdated();
    void updateChanged();
    void stateChanged();
    void disconnected();
    void randomAddressChanged();

    void deviceVoltageChanged();
    void valueVoltageChanged();

    void deviceDCSensorChanged();
    void valueDistanceChanged();
    void valueColorChanged();

private:
    void setUpdate(const QString &message);
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    DeviceInfo currentDevice;
    QList<QObject *> devices;
    QList<QObject *> m_services;
    QList<QObject *> m_characteristics;
    QString m_currentAddress;
    QString m_service;
    QString m_characteristic;
    QString m_previousAddress;
    QString m_message;
    bool connected = false;
    bool ready = false;
    QLowEnergyController *controller = nullptr;
    bool m_deviceScanState = false;
    bool randomAddress = false;

    bool m_device_motorAB = false;
    bool m_device_motorC = false; // external
    bool m_device_voltage = false;
    bool m_device_dcsensor = false;

    double m_value_voltage = 0.0;
    double m_value_distance = 0.0;
    QColor m_value_color;
};

#endif // DEVICE_H
