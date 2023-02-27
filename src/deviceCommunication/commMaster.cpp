/******************************************************************************
 * Copyright (C) 2022 by Gschwind, Weber, Schoch, Niederberger                *
 *                                                                            *
 * This file is part of linescaleGUI.                                         *
 *                                                                            *
 * LinescaleGUI is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * LinescaleGUI is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with linescaleGUI. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/
/**
 * @file commMaster.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `comm::CommMaster` implementation
 *
 */

#include "commMaster.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <cassert>
#include "commUSB.h"
#include "command.h"

namespace comm {
CommMaster::CommMaster(Notification* notification, Bluetooth* bluetooth)
    : bluetooth(bluetooth), notification(notification) {
    assert(notification != nullptr);
    assert(bluetooth != nullptr);

    connect(bluetooth, &Bluetooth::deviceDiscovered, this, &CommMaster::discoveredDeviceBluetooth);
    connect(bluetooth, &Bluetooth::scanStarted, this, &CommMaster::discoverDevicesStartedBluetooth);
    connect(bluetooth, &Bluetooth::scanStopped, this, &CommMaster::discoverDevicesStoppedBluetooth);
}

CommMaster::~CommMaster() {
    delete singleDevice;
}

void CommMaster::addConnection(DeviceInfo identifier) {
    if (singleDevice != nullptr) {
        removeConnection();
    }

    switch (identifier.type) {
        case ConnType::USB:
            singleDevice = new CommUSB(identifier);
            break;

        case ConnType::BLE:
            singleDevice = new BluetoothDevice(identifier);
            break;

        default:
            break;
    }

    if (singleDevice != nullptr) {
        connect(singleDevice, &CommDevice::newSampleDevice, this, &CommMaster::receiveSampleMaster);
        connect(singleDevice, &CommDevice::changedStateDevice, this, &CommMaster::getChangedState);
        singleDevice->connectDevice();
    }
}

void CommMaster::removeConnection() {
    if (singleDevice != nullptr) {
        singleDevice->disconnectDevice();
        disconnect(singleDevice);
    }

    delete singleDevice;
    singleDevice = nullptr;
}

void CommMaster::discoverDevices(void) {
    availableDevices.clear();
    notification->push("Scan for USB devices");

    // Discover all available devices connected by USB
    QList<QSerialPortInfo> listOfCOMPorts = QSerialPortInfo::availablePorts();
    for (int i = 0; i < listOfCOMPorts.length(); ++i) {
        // Check vendorID for LineScales or COM101 for debug
        /// @todo Check vendorID on multiple devices/batches
        if (listOfCOMPorts[i].vendorIdentifier() == 0x1a86 ||
            listOfCOMPorts[i].portName() == "COM101") {
            DeviceInfo deviceInfo;
            deviceInfo.ID = listOfCOMPorts[i].portName();
            deviceInfo.type = ConnType::USB;
            deviceInfo.baudRate = 230400;
            availableDevices.append(deviceInfo);
            emit discoveredDeviceMaster(deviceInfo);
        }
    }

    notification->push("Scan for USB devices finished");

    // Discover all available devices over BLE
    bluetooth->scanStart();

    /// @todo remove already connected devices from this list
}

void CommMaster::sendData(const QByteArray& rawData) {
    if (singleDevice != nullptr && rawData.length() > 0) {
        singleDevice->sendData(rawData);
    }
}

void CommMaster::sendData(const QString& rawData) {
    bool bStatus;
    QString payload4Bit = rawData.leftJustified(8, '0');
    uint32_t nHex = payload4Bit.toULong(&bStatus, 16);

    QByteArray rawHexData;
    rawHexData.append(uchar(nHex >> 24));
    rawHexData.append(uchar(nHex >> 16));
    rawHexData.append(uchar(nHex >> 8));
    rawHexData.append(uchar(nHex));
    sendData(rawHexData);
}

void CommMaster::receiveSampleMaster(Sample reading) {
    emit newSampleMaster(reading);
}

void CommMaster::getChangedState(bool connected) {
    ///@todo Add actual device names
    if (connected) {
        if (singleDevice->getConnType() == ConnType::USB) {
            notification->push("USB connected");
        } else {
            notification->push("BLE connected");
        }
    } else {
        if (singleDevice->getConnType() == ConnType::USB) {
            notification->push("USB disconnected");
        } else {
            notification->push("BLE disconnected");
        }
    }

    emit changedStateMaster(connected);
}

void CommMaster::setNewFreq(int newFreq) {
    ///@todo Change frequency tags to enum types
    ///@todo Whole frequency selection for BLE is bugged
    switch (newFreq) {
        case 10:
            sendData(command::SETSPEED10);
            break;
        case 40:
            sendData(command::SETSPEED40);
            break;
        case 640:
            sendData(command::SETSPEED640);
            break;
        case 1280:
            sendData(command::SETSPEED1280);
            break;
        default:
            break;
    }
}

void CommMaster::discoveredDeviceBluetooth(DeviceInfo& deviceInfo) {
    availableDevices.append(deviceInfo);
    emit discoveredDeviceMaster(deviceInfo);
}

void CommMaster::discoverDevicesStartedBluetooth(void) {
    notification->push("Scan for BLE devices");
}

void CommMaster::discoverDevicesStoppedBluetooth(void) {
    notification->push("Scan for BLE devices finished");
    emit discoverDevicesFinishedMaster();
}
}  // namespace comm
