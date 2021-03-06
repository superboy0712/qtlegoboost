/***************************************************************************
**
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
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

#include "device.h"

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QMetaEnum>
#include <QTimer>
#include <legoabmotor.h>
#include <legocdmotor.h>

Device::Device()
    : m_service("{00001623-1212-efde-1623-785feabcd123}")
    , m_characteristic("{00001624-1212-efde-1623-785feabcd123}")
{
    //! [les-devicediscovery-1]
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &Device::addDevice);
    connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &Device::deviceScanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Device::deviceScanFinished);
    //! [les-devicediscovery-1]

    setUpdate("Search");
}

Device::~Device()
{
    delete discoveryAgent;
    delete controller;
    qDeleteAll(devices);
    qDeleteAll(m_services);
    qDeleteAll(m_characteristics);
    devices.clear();
    m_services.clear();
    m_characteristics.clear();
}

void Device::startDeviceDiscovery()
{
    qDeleteAll(devices);
    devices.clear();
    emit devicesUpdated();

    setUpdate("Scanning for devices ...");
    //! [les-devicediscovery-2]
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    //! [les-devicediscovery-2]

    if (discoveryAgent->isActive()) {
        m_deviceScanState = true;
        Q_EMIT stateChanged();
    }
}

//! [les-devicediscovery-3]
void Device::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        if (info.name() == "LEGO Move Hub") {
            qDebug() << Q_FUNC_INFO
                     << ", name: " << info.name()
                     << ", address: " << info.address()
                     << ", deviceUuid: " << info.deviceUuid();
            auto d = new DeviceInfo(info);
            devices.append(d);
            setUpdate("Last device added: " + d->getName());
        }
    }
}
//! [les-devicediscovery-3]

void Device::deviceScanFinished()
{
    emit devicesUpdated();
    m_deviceScanState = false;
    emit stateChanged();
    if (devices.isEmpty())
        setUpdate("No Low Energy devices found...");
    else
        setUpdate("Done! Scan Again!");
}

QVariant Device::getDevices()
{
    return QVariant::fromValue(devices);
}

QVariant Device::getServices()
{
    return QVariant::fromValue(m_services);
}

QVariant Device::getCharacteristics()
{
    return QVariant::fromValue(m_characteristics);
}

QString Device::getUpdate()
{
    return m_message;
}

void Device::scanServices(const QString &address)
{
    qDebug() << Q_FUNC_INFO << " address: " << address;
    // We need the current device for service discovery.

    for (auto d: qAsConst(devices)) {
        auto device = qobject_cast<DeviceInfo *>(d);
        if (!device)
            continue;

        if (device->getAddress() == address)
            currentDevice.setDevice(device->getDevice());
    }

    if (!currentDevice.getDevice().isValid()) {
        qWarning() << "Not a valid device";
        return;
    }

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();
    qDeleteAll(m_services);
    m_services.clear();
    emit servicesUpdated();

    setUpdate("Back\n(Connecting to device...)");

    m_currentAddress = currentDevice.getAddress();

    if (controller && m_previousAddress != m_currentAddress) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }

    //! [les-controller-1]
    if (!controller) {
        // Connecting signals and slots for connecting to LE services.
        controller = QLowEnergyController::createCentral(currentDevice.getDevice());
        connect(controller, &QLowEnergyController::connected,
                this, &Device::deviceConnected);
        connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
                this, &Device::errorReceived);
        connect(controller, &QLowEnergyController::disconnected,
                this, &Device::deviceDisconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered,
                this, &Device::addLowEnergyService);
        connect(controller, &QLowEnergyController::discoveryFinished,
                this, &Device::serviceScanDone);
    }

    if (isRandomAddress())
        controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    else
        controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    controller->connectToDevice();
    //! [les-controller-1]

    m_previousAddress = m_currentAddress;
}

void Device::addLowEnergyService(const QBluetoothUuid &serviceUuid)
{
    qDebug() << Q_FUNC_INFO << " serviceUuid: " << serviceUuid;
    if (serviceUuid.toString() != m_service)
        return;

    //! [les-service-1]
    QLowEnergyService *service = controller->createServiceObject(serviceUuid);
    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }
    //! [les-service-1]
    auto serv = new ServiceInfo(service);
    m_services.append(serv);

    emit servicesUpdated();
}
//! [les-service-1]

void Device::serviceScanDone()
{
    setUpdate("Back\n(Service scan done!)");
    // force UI in case we didn't find anything
    if (m_services.isEmpty())
        emit servicesUpdated();
}

void Device::connectToService(const QString &uuid)
{
    QLowEnergyService *service = nullptr;
    for (auto s: qAsConst(m_services)) {
        auto serviceInfo = qobject_cast<ServiceInfo *>(s);
        if (!serviceInfo)
            continue;

        if (serviceInfo->getUuid() == uuid) {
            service = serviceInfo->service();
            break;
        }
    }

    if (!service)
        return;

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();

    if (service->state() == QLowEnergyService::DiscoveryRequired) {
        //! [les-service-3]
        connect(service, &QLowEnergyService::stateChanged,
                this, &Device::serviceDetailsDiscovered);
        connect(service, &QLowEnergyService::characteristicChanged, this, &Device::updateValue);
        connect(service, &QLowEnergyService::descriptorWritten, this, &Device::confirmedDescriptorWrite);
        service->discoverDetails();
        setUpdate("Back\n(Discovering details...)");
        //! [les-service-3]
        return;
    }

    //discovery already done
    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    for (const QLowEnergyCharacteristic &ch : chars) {
        auto cInfo = new CharacteristicInfo(ch);
        m_characteristics.append(cInfo);
    }

    QTimer::singleShot(0, this, &Device::characteristicsUpdated);
}

void Device::deviceConnected()
{
    setUpdate("Back\n(Discovering services...)");
    connected = true;
    //! [les-service-2]
    controller->discoverServices();
    //! [les-service-2]
}

void Device::errorReceived(QLowEnergyController::Error /*error*/)
{
    qWarning() << "Error: " << controller->errorString();
    setUpdate(QString("Back\n(%1)").arg(controller->errorString()));
}

void Device::setUpdate(const QString &message)
{
    m_message = message;
    emit updateChanged();
}

void Device::disconnectFromDevice()
{
    // UI always expects disconnect() signal when calling this signal
    // TODO what is really needed is to extend state() to a multi value
    // and thus allowing UI to keep track of controller progress in addition to
    // device scan progress

    if (controller->state() != QLowEnergyController::UnconnectedState)
        controller->disconnectFromDevice();
    else
        deviceDisconnected();
}

void Device::readVoltage(bool r)
{
    for (const auto s : m_services) {
        auto sInfo = qobject_cast<ServiceInfo *>(s);
        if (sInfo->service()->serviceUuid().toString() == m_service) {
            for (const auto m : m_characteristics) {
                auto cInfo = qobject_cast<CharacteristicInfo *>(m);
                if (r) {
                    sInfo->service()->writeCharacteristic(cInfo->getCharacteristic(), QByteArray::fromHex("0a01413c000100000001"));
                } else {
                    sInfo->service()->writeCharacteristic(cInfo->getCharacteristic(), QByteArray::fromHex("0a01413c000000000000"));
                }
            }
        }
    }
}

void Device::readDistance(bool r)
{
    for (const auto s : m_services) {
        auto sInfo = qobject_cast<ServiceInfo *>(s);
        if (sInfo->service()->serviceUuid().toString() == m_service) {
            for (const auto m : m_characteristics) {
                auto cInfo = qobject_cast<CharacteristicInfo *>(m);
                if (r) {
                    sInfo->service()->writeCharacteristic(cInfo->getCharacteristic(), QByteArray::fromHex("0a014102080100000001"));
                } else {
                    sInfo->service()->writeCharacteristic(cInfo->getCharacteristic(), QByteArray::fromHex("0a014102080000000000"));
                }
            }
        }
    }
}

void Device::deviceDisconnected()
{
    qWarning() << "Disconnect from device";
    emit disconnected();
}

void Device::serviceDetailsDiscovered(QLowEnergyService::ServiceState s)
{
    qDebug() << Q_FUNC_INFO;
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug() << "Discovering services...";
        break;
    case QLowEnergyService::ServiceDiscovered:
    {
        qDebug() << "Service discovered.";

        auto service = qobject_cast<QLowEnergyService *>(sender());
        if (!service)
            return;

        ready = true;

        const QList<QLowEnergyCharacteristic> chars = service->characteristics();
        qDebug() << "chars size: " << chars.size();
        for (const QLowEnergyCharacteristic &ch : chars) {
            auto cInfo = new CharacteristicInfo(ch);
            m_characteristics.append(cInfo);
            if (ch.isValid()) {
                qDebug() << "value: " << ch.value().toHex() << "-" << ch.value().size();
                auto notificationDesc = ch.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
                if (notificationDesc.isValid()) {
                    service->writeDescriptor(notificationDesc, QByteArray::fromHex("0100"));
                }

                const QList<QLowEnergyDescriptor> descs = ch.descriptors();
                qDebug() << "descs size: " << descs.size();
                for (const QLowEnergyDescriptor &des : descs) {
                    qDebug() << "des: " << des.name()
                             << "-" << des.uuid()
                             << "-" << des.type()
                             << "-" << des.value()
                             << "-" << des.handle();
                }
            }
        }

        break;
    }
    default:
        //nothing for now
        break;
    }

    emit characteristicsUpdated();
}

void Device::updateValue(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    if (c.uuid().toString() != m_characteristic)
        return;

    unsigned char len = (unsigned char)value.at(0);
    qDebug() << Q_FUNC_INFO
             << ", value: " << value.toHex() << "-" << value.size() << "-" << len;

    if (value.size() != len) {
        qWarning() << "value: " << value.toHex() << " is broken!";
        return;
    }

    unsigned char msgType = (unsigned char)value.at(2);
    switch (msgType) {
    case PortInformation:
        portInformation(value);
        break;
    case Acknowledgement:
        acknowledgement(value);
        break;
    case SensorReading:
        sensorReading(value);
        break;
    case PortChanged:
        portChanged(value);
        break;
    default:
        break;
    }
}

void Device::confirmedDescriptorWrite()
{

}

void Device::acknowledgement(const QByteArray &value)
{
    unsigned char portType = (unsigned char)value.at(3);
    unsigned char deviceEvent = (unsigned char)value.at(5);
    qDebug() << "deviceEvent: " << deviceEvent;
    bool online = (deviceEvent != 0);

    switch (portType) {
    case PortVoltage:
        qDebug() << "Voltage subscription acknowledged"
                 << ", m_busy: " << m_busy
                 << ", online: " << online;
        if (!m_busy && online) {
            m_busy = true;
            emit busyChanged();
        } else if (m_busy && !online) {
            m_busy = false;
            emit busyChanged();
        }
        break;
    case PortD:
        qDebug() << "DCSensor subscription acknowledged"
                 << ", m_busy: " << m_busy
                 << ", online: " << online;
        if (!m_busy && online) {
            m_busy = true;
            emit busyChanged();
        } else if (m_busy && !online) {
            m_busy = false;
            emit busyChanged();
        }
        break;
    default:
        break;
    }
}

void Device::portChanged(const QByteArray &value)
{
    unsigned char portType = (unsigned char)value.at(3);
    unsigned char deviceEvent = (unsigned char)value.at(4);

    switch (portType) {
    case PortAB:
        qDebug() << "PortAB status " << deviceEvent;
        if (!m_busy && deviceEvent == 1) {
            m_busy = true;
            emit busyChanged();
        } else if (m_busy && deviceEvent == 10) {
            m_busy = false;
            emit busyChanged();
        }
        break;
    case PortC:
        qDebug() << "PortC status " << deviceEvent;
        if (!m_busy && deviceEvent == 1) {
            m_busy = true;
            emit busyChanged();
        } else if (m_busy && deviceEvent == 10) {
            m_busy = false;
            emit busyChanged();
        }
        break;
    default:
        break;
    }
}

void Device::voltageReading(const QByteArray &value)
{
    uint val = (unsigned char)value.at(4) * 256 + (unsigned char)value.at(5);

//    qDebug() << "val: " << val;

    m_value_voltage = val / 4096.0;
//    qDebug() << "Voltage value: " << m_value_voltage;

    emit valueVoltageChanged();
}

void Device::distanceReading(const QByteArray &value)
{
    uint val = (unsigned char)value.at(4) * 256 + (unsigned char)value.at(5);

//    qDebug() << "val: " << val;

    m_value_voltage = val / 4096.0;
//    qDebug() << "Voltage value: " << m_value_voltage;

    emit valueDistanceChanged();
}

void Device::colorReading(const QByteArray &value)
{
    unsigned char val = (unsigned char)value.at(4);

    qDebug() << "val: " << val;

    if (val == 255)
        return;

    switch (val) {
    case 0x00:
        qDebug() << "black";
        m_value_color = Qt::black;
        break;
    case 0x02:
        qDebug() << "cyan";
        m_value_color = Qt::cyan;
        break;
    case 0x03:
        qDebug() << "blue";
        m_value_color = Qt::blue;
        break;
    case 0x05:
        qDebug() << "green";
        m_value_color = Qt::green;
        break;
    case 0x07:
        qDebug() << "yellow";
        m_value_color = Qt::yellow;
        break;
    case 0x09:
        qDebug() << "red";
        m_value_color = Qt::red;
        break;
    case 0x0a:
        qDebug() << "white";
        m_value_color = Qt::white;
        break;
    default:
        break;
    }

    emit valueColorChanged();
}

void Device::sensorReading(const QByteArray &value)
{
    unsigned char portType = (unsigned char)value.at(3);
    switch (portType) {
    case PortVoltage:
        voltageReading(value);
        break;
    case PortD:
        distanceReading(value);
        colorReading(value);
        break;
    default:
        break;
    }
}

void Device::portInformation(const QByteArray &value)
{
    unsigned char portType = (unsigned char)value.at(3);
    unsigned char deviceEvent = (unsigned char)value.at(4);
    bool online = (deviceEvent != 0);

    switch (portType) {
    case PortAB:
        m_device_motorAB = online;
        break;
    case PortVoltage:
        m_device_voltage = online;
        emit deviceVoltageChanged();
        break;
    case PortC:
        m_device_motorC = online;
        break;
    case PortD:
        m_device_dcsensor = online;
        emit deviceDCSensorChanged();
        break;
    default:
        break;
    }
}

void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        setUpdate("The Bluetooth adaptor is powered off, power it on before doing discovery.");
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        setUpdate("Writing or reading from the device resulted in an error.");
    else {
        static QMetaEnum qme = discoveryAgent->metaObject()->enumerator(
                    discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        setUpdate("Error: " + QLatin1String(qme.valueToKey(error)));
    }

    m_deviceScanState = false;
    emit devicesUpdated();
    emit stateChanged();
}

bool Device::state()
{
    return m_deviceScanState;
}

bool Device::hasControllerError() const
{
    return (controller && controller->error() != QLowEnergyController::NoError);
}

bool Device::isRandomAddress() const
{
    return randomAddress;
}

void Device::setRandomAddress(bool newValue)
{
    randomAddress = newValue;
    emit randomAddressChanged();
}

void Device::sendCommand(const QByteArray &command)
{
    qDebug() << "Device::sendCommand...";
    for (const auto s : m_services) {
        auto sInfo = qobject_cast<ServiceInfo *>(s);
        qDebug() << "sInfo" << sInfo->getUuid();
        if (sInfo->service()->serviceUuid().toString() == m_service) {
            for (const auto m : m_characteristics) {
                auto cInfo = qobject_cast<CharacteristicInfo *>(m);
                qDebug() << "cInfo" << cInfo->getUuid();
                sInfo->service()->writeCharacteristic(cInfo->getCharacteristic(), command);
            }
        }
    }
}

bool Device::busy() const
{
    return m_busy;
}

bool Device::deviceVoltage() const
{
    return m_device_voltage;
}

double Device::valueVoltage() const
{
    return m_value_voltage;
}

bool Device::deviceDCSensor() const
{
    return m_device_dcsensor;
}

double Device::valueDistance() const
{
    return m_value_distance;
}

QColor Device::valueColor() const
{
    return m_value_color;
}
