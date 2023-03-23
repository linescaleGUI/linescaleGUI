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
 * @file bluetoothDevice.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "bluetoothDevice.h"

namespace comm {
const QBluetoothUuid BluetoothDevice::UUID_SERVICE(
    QString::fromStdString("00001000-0000-1000-8000-00805f9b34fb"));
const QBluetoothUuid BluetoothDevice::UUID_CHARACTERISTIC_READ(
    QString::fromStdString("00001002-0000-1000-8000-00805f9b34fb"));
const QBluetoothUuid BluetoothDevice::UUID_CHARACTERISTIC_WRITE(
    QString::fromStdString("00001001-0000-1000-8000-00805f9b34fb"));

BluetoothDevice::BluetoothDevice(const DeviceInfo& deviceInfo) : deviceInfo(deviceInfo) {
    type = ConnType::BLE;
    serviceIndex = 0;
    lowEnergyController = nullptr;
    services.clear();
    communicationService = nullptr;
}

BluetoothDevice::~BluetoothDevice() {
    disconnectDevice();

    if (lowEnergyController != nullptr) {
        delete lowEnergyController;
    }
}

void BluetoothDevice::connectDevice(void) {
    // If a connection is already established don't connect to the device again
    if (lowEnergyController == nullptr) {
        lowEnergyController = QLowEnergyController::createCentral(deviceInfo.bluetooth);
        connect(lowEnergyController, &QLowEnergyController::connected, this,
                &BluetoothDevice::lowEnergyControllerConnected);
        connect(lowEnergyController, &QLowEnergyController::disconnected, this,
                &BluetoothDevice::lowEnergyControllerDisconnected);
        connect(lowEnergyController, &QLowEnergyController::discoveryFinished, this,
                &BluetoothDevice::lowEnergyControllerDiscoveryFinished);
        connect(lowEnergyController,
                qOverload<QLowEnergyController::Error>(&QLowEnergyController::error), this,
                &BluetoothDevice::lowEnergyControllerErrorOccurred);
        connect(lowEnergyController, &QLowEnergyController::serviceDiscovered, this,
                &BluetoothDevice::lowEnergyControllerServiceDiscovered);
    } else {
        if (lowEnergyController->state() != QLowEnergyController::UnconnectedState) {
            return;
        }
    }

    lowEnergyController->connectToDevice();
}

void BluetoothDevice::disconnectDevice(void) {
    if (lowEnergyController == nullptr) {
        return;
    }

    lowEnergyController->disconnectFromDevice();
}

void BluetoothDevice::readData(void) {
    if (communicationService == nullptr) {
        return;
    }

    communicationService->read(communicationCharacteristicRead);
}

void BluetoothDevice::sendData(const QByteArray& value) {
    if (communicationService == nullptr) {
        return;
    }

    communicationService->write(communicationCharacteristicWrite, value);
}

DeviceInfo& BluetoothDevice::getDeviceInfo(void) {
    return deviceInfo;
}

void BluetoothDevice::lowEnergyControllerConnected(void) {
    emit connected(this);
    /// @todo Add multiple device support
    emit changedStateDevice(true);
    lowEnergyController->discoverServices();
}

void BluetoothDevice::lowEnergyControllerDisconnected(void) {
    if (communicationService != nullptr) {
        disconnect(communicationService, &BluetoothService::characteristicChanged, this,
                   &BluetoothDevice::serviceCharacteristicChanged);
        disconnect(communicationService, &BluetoothService::characteristicRead, this,
                   &BluetoothDevice::serviceCharacteristicRead);
        disconnect(communicationService, &BluetoothService::characteristicWritten, this,
                   &BluetoothDevice::serviceCharacteristicWritten);
    }

    for (BluetoothService* service : services) {
        if (service != nullptr) {
            delete service;
        }
    }

    services.clear();
    communicationService = nullptr;
    emit disconnected(this);
    /// @todo Add multiple device support
    emit changedStateDevice(false);
}

void BluetoothDevice::lowEnergyControllerDiscoveryFinished(void) {
    bool serviceFound = false;

    // Check if the service is available on this device before the details are discovered. Safes
    // time
    for (BluetoothService* service : services) {
        if (service->getService()->serviceUuid() == UUID_SERVICE) {
            serviceFound = true;
        }
    }

    if (!serviceFound) {
        emit serviceDiscoveryFailed(this);
        disconnectDevice();
        return;
    }

    // Iterate through the service list and start the discovery for the first service which requires
    // it
    for (serviceIndex = 0; serviceIndex < services.capacity(); ++serviceIndex) {
        if (services.at(serviceIndex)->discoverDetails()) {
            break;
        }
    }

    if (serviceIndex >= services.capacity()) {
        serviceDetailsDiscovered();
    }
}

void BluetoothDevice::lowEnergyControllerErrorOccurred(QLowEnergyController::Error error) {
    /// @todo Implement function
}

void BluetoothDevice::lowEnergyControllerServiceDiscovered(const QBluetoothUuid& uuid) {
    BluetoothService* service =
        new BluetoothService(lowEnergyController->createServiceObject(uuid));
    service->connect(service, &BluetoothService::detailsDiscovered, this,
                     &BluetoothDevice::serviceDetailsDiscovered);
    services.push_back(service);
}

void BluetoothDevice::serviceDetailsDiscovered(void) {
    // Iterate through the rest of the service list and start the discovery for every service which
    // requires it
    for (; serviceIndex < services.capacity(); ++serviceIndex) {
        if (services.at(serviceIndex)->discoverDetails()) {
            break;
        }
    }

    // Return if not all details have been discovered yet
    if (serviceIndex < services.capacity()) {
        return;
    }

    communicationService = nullptr;

    // Check if the characteristics can be found
    for (BluetoothService* service : services) {
        bool characteristicReadFound = false;
        bool characteristicWriteFound = false;

        if (service->getService()->serviceUuid() == UUID_SERVICE) {
            QList<QLowEnergyCharacteristic> characteristics =
                service->getService()->characteristics();

            for (QLowEnergyCharacteristic& characteristic : characteristics) {
                if (characteristic.uuid() == UUID_CHARACTERISTIC_READ) {
                    characteristicReadFound = true;
                    communicationCharacteristicRead = characteristic;
                } else if (characteristic.uuid() == UUID_CHARACTERISTIC_WRITE) {
                    characteristicWriteFound = true;
                    communicationCharacteristicWrite = characteristic;
                }
            }

            // All characteristics have been found within a certain service
            if (characteristicReadFound && characteristicWriteFound) {
                communicationService = service;
                break;
            }
        }
    }

    if ((communicationService == nullptr) || !communicationCharacteristicRead.isValid() ||
        !communicationCharacteristicWrite.isValid()) {
        emit serviceDiscoveryFailed(this);
        disconnectDevice();
    }

    QLowEnergyDescriptor notification = communicationCharacteristicRead.descriptor(
        QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

    if (!notification.isValid()) {
        emit serviceDiscoveryFailed(this);
        disconnectDevice();
    }

    communicationService->writeDescriptor(notification, QByteArray::fromHex("0100"));

    connect(communicationService, &BluetoothService::characteristicChanged, this,
            &BluetoothDevice::serviceCharacteristicChanged);
    connect(communicationService, &BluetoothService::characteristicRead, this,
            &BluetoothDevice::serviceCharacteristicRead);
    connect(communicationService, &BluetoothService::characteristicWritten, this,
            &BluetoothDevice::serviceCharacteristicWritten);
    emit serviceDiscoveryFinished(this, services);
}

void BluetoothDevice::serviceCharacteristicChanged(const QLowEnergyCharacteristic& characteristic,
                                                   const QByteArray& value) {
    emit characteristicChanged(this, value);

    bool success;
    success = parser.parsePackage((QByteArray&)value, receivedData);

    if (success) {
        emit newSampleDevice(receivedData);
    }
}

void BluetoothDevice::serviceCharacteristicRead(const QLowEnergyCharacteristic& characteristic,
                                                const QByteArray& value) {
    emit characteristicRead(this, value);

    bool success;
    success = parser.parsePackage((QByteArray&)value, receivedData);

    if (success) {
        emit newSampleDevice(receivedData);
    }
}

void BluetoothDevice::serviceCharacteristicWritten(const QLowEnergyCharacteristic& characteristic,
                                                   const QByteArray& value) {
    emit characteristicWritten(this, value);
}
}  // namespace comm
