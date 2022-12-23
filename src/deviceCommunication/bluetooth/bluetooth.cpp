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
 * @file bluetooth.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "bluetooth.h"

namespace comm {
Bluetooth::Bluetooth() {
    stateBefore = localDevice.hostMode();
    deviceDiscoveryAgent.setLowEnergyDiscoveryTimeout(5000);
    ScanStop();

    connect(&localDevice, &QBluetoothLocalDevice::error, this,
            &Bluetooth::LocalDeviceErrorOccurred);
    connect(&localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this,
            &Bluetooth::LocalDeviceHostModeStateChanged);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this,
            &Bluetooth::DeviceDiscoveryAgentCanceled);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this,
            &Bluetooth::DeviceDiscoveryAgentDeviceDiscovered);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceUpdated, this,
            &Bluetooth::DeviceDiscoveryAgentDeviceUpdated);
    connect(
        &deviceDiscoveryAgent,
        qOverload<QBluetoothDeviceDiscoveryAgent::Error>(&QBluetoothDeviceDiscoveryAgent::error),
        this, &Bluetooth::DeviceDiscoveryAgentErrorOccurred);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this,
            &Bluetooth::DeviceDiscoveryAgentFinished);
}

Bluetooth::~Bluetooth(void) {}

void Bluetooth::PowerOn(void) {
    /* May not work on iOS*/
    localDevice.setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void Bluetooth::PowerOff(void) {
    ScanStop();

    /* May not work on iOS*/
    localDevice.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

void Bluetooth::PowerToggle(void) {
    if (IsPowerOn()) {
        PowerOff();
    } else {
        PowerOn();
    }
}

bool Bluetooth::IsPowerOn(void) {
    return (localDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff) ? false : true;
}

void Bluetooth::ScanStart(void) {
    if (!IsPowerOn()) {
        return;
    }

    if (!IsScanning()) {
        emit ScanStarted();
        ///@todo Fails if flight mode is active
        deviceDiscoveryAgent.start();
    }
}

void Bluetooth::ScanStop(void) {
    if (IsScanning()) {
        deviceDiscoveryAgent.stop();
    }
}

void Bluetooth::ScanToggle(void) {
    if (IsScanning()) {
        ScanStop();
    } else {
        ScanStart();
    }
}

bool Bluetooth::IsScanning(void) {
    return deviceDiscoveryAgent.isActive();
}

/*
void Bluetooth::LocalDeviceConnected(const QBluetoothAddress &address) {

}

void Bluetooth::LocalDeviceDisconnected(const QBluetoothAddress &address) {

}
*/

void Bluetooth::LocalDeviceErrorOccurred(QBluetoothLocalDevice::Error error) {
    // TODO: Do error checking
}

void Bluetooth::LocalDeviceHostModeStateChanged(QBluetoothLocalDevice::HostMode state) {
    if ((stateBefore == QBluetoothLocalDevice::HostMode::HostPoweredOff) &&
        (state != QBluetoothLocalDevice::HostMode::HostPoweredOff)) {
        emit PowerTurnedOn();
    } else if (state == QBluetoothLocalDevice::HostMode::HostPoweredOff) {
        ScanStop();
        emit PowerTurnedOff();
    }

    stateBefore = state;
}

/*
void Bluetooth::LocalDevicePairingFinished(const QBluetoothAddress &address,
QBluetoothLocalDevice::Pairing pairing) {

}
*/

void Bluetooth::DeviceDiscoveryAgentCanceled() {
    emit ScanStopped();
}

void Bluetooth::DeviceDiscoveryAgentDeviceDiscovered(
    const QBluetoothDeviceInfo& bluetoothDeviceInfo) {
    if (bluetoothDeviceInfo.coreConfigurations() !=
            QBluetoothDeviceInfo::LowEnergyCoreConfiguration ||
        (bluetoothDeviceInfo.name() != "LineScale 3")) {
        return;
    }

    DeviceInfo deviceInfo;
    deviceInfo.type = ConnType::BLE;
    deviceInfo.ID = bluetoothDeviceInfo.name();
    deviceInfo.bluetooth = bluetoothDeviceInfo;
    emit ScanDeviceDiscovered(deviceInfo);
}

void Bluetooth::DeviceDiscoveryAgentDeviceUpdated(const QBluetoothDeviceInfo& bluetoothDeviceInfo,
                                                  QBluetoothDeviceInfo::Fields fields) {
    // TODO: Implement function
}

void Bluetooth::DeviceDiscoveryAgentErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error) {
    // TODO: Do error checking
}

void Bluetooth::DeviceDiscoveryAgentFinished() {
    emit ScanStopped();
}
}  // namespace comm
