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
#include <cassert>

namespace comm {
const QString Bluetooth::FILTER_NAME = "LineScale 3";

Bluetooth::Bluetooth(Notification* notification) : notification(notification) {
    assert(notification != nullptr);

    if (!localDevice.isValid()) {
        return;
    }

    stateBefore = localDevice.hostMode();
    deviceDiscoveryAgent.setLowEnergyDiscoveryTimeout(5000);
    scanStop();

    connect(&localDevice, &QBluetoothLocalDevice::error, this,
            &Bluetooth::localDeviceErrorOccurred);
    connect(&localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this,
            &Bluetooth::localDeviceHostModeStateChanged);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this,
            &Bluetooth::deviceDiscoveryAgentCanceled);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this,
            &Bluetooth::deviceDiscoveryAgentDeviceDiscovered);
    connect(
        &deviceDiscoveryAgent,
        qOverload<QBluetoothDeviceDiscoveryAgent::Error>(&QBluetoothDeviceDiscoveryAgent::error),
        this, &Bluetooth::deviceDiscoveryAgentErrorOccurred);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this,
            &Bluetooth::deviceDiscoveryAgentFinished);
}

Bluetooth::~Bluetooth() {}

bool Bluetooth::isPowerOn(void) {
    return (localDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff) ? false : true;
}

void Bluetooth::scanStart(void) {
    if (!isPowerOn()) {
        return;
    }

    if (!isScanning()) {
        emit scanStarted();
        deviceDiscoveryAgent.start();
    }
}

void Bluetooth::scanStop(void) {
    if (isScanning()) {
        deviceDiscoveryAgent.stop();
    }
}

void Bluetooth::scanToggle(void) {
    if (isScanning()) {
        scanStop();
    } else {
        scanStart();
    }
}

bool Bluetooth::isScanning(void) {
    return deviceDiscoveryAgent.isActive();
}

void Bluetooth::localDeviceErrorOccurred(QBluetoothLocalDevice::Error error) {
    notification->push("Local device error occurred" + error, Notification::SEVERITY_CRITICAL);
}

void Bluetooth::localDeviceHostModeStateChanged(QBluetoothLocalDevice::HostMode state) {
    if ((stateBefore == QBluetoothLocalDevice::HostMode::HostPoweredOff) &&
        (state != QBluetoothLocalDevice::HostMode::HostPoweredOff)) {
        emit powerTurnedOn();
    } else if (state == QBluetoothLocalDevice::HostMode::HostPoweredOff) {
        scanStop();
        emit powerTurnedOff();
    }

    stateBefore = state;
}

void Bluetooth::deviceDiscoveryAgentCanceled() {
    emit scanStopped();
}

void Bluetooth::deviceDiscoveryAgentDeviceDiscovered(
    const QBluetoothDeviceInfo& bluetoothDeviceInfo) {
    if (bluetoothDeviceInfo.coreConfigurations() !=
            QBluetoothDeviceInfo::LowEnergyCoreConfiguration ||
        (bluetoothDeviceInfo.name() != FILTER_NAME)) {
        return;
    }

    DeviceInfo deviceInfo;
    deviceInfo.type = ConnType::BLE;
    deviceInfo.ID = bluetoothDeviceInfo.name();
    deviceInfo.bluetooth = bluetoothDeviceInfo;
    emit deviceDiscovered(deviceInfo);
}

void Bluetooth::deviceDiscoveryAgentErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error) {
    notification->push("Error occurred during device discovery: " + error,
                       Notification::SEVERITY_CRITICAL);
}

void Bluetooth::deviceDiscoveryAgentFinished() {
    emit scanStopped();
}
}  // namespace comm
