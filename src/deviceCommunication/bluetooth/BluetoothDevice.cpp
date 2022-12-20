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
 * @file BluetoothDevice.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "BluetoothDevice.h"

const QBluetoothUuid BluetoothDevice::UUID_SERVICE("00001000-0000-1000-8000-00805f9b34fb");
const QBluetoothUuid BluetoothDevice::UUID_CHARACTERISTIC_READ(
    "00001002-0000-1000-8000-00805f9b34fb");
const QBluetoothUuid BluetoothDevice::UUID_CHARACTERISTIC_WRITE(
    "00001001-0000-1000-8000-00805f9b34fb");

BluetoothDevice::BluetoothDevice(const QBluetoothDeviceInfo& deviceInfo) : deviceInfo(deviceInfo) {
    serviceIndex = 0;
    lowEnergyController = nullptr;
    services.clear();
    communicationService = nullptr;
}

BluetoothDevice::~BluetoothDevice() {
    Disconnect();

    if (lowEnergyController != nullptr) {
        delete lowEnergyController;
    }

    services.clear();
}

void BluetoothDevice::Connect(void) {
    // If a connection is already established don't connect to the device again
    if (lowEnergyController == nullptr) {
        lowEnergyController = QLowEnergyController::createCentral(deviceInfo);
        connect(lowEnergyController, &QLowEnergyController::connected, this,
                &BluetoothDevice::LowEnergyControllerConnected);
        connect(lowEnergyController, &QLowEnergyController::connectionUpdated, this,
                &BluetoothDevice::LowEnergyControllerConnectionUpdated);
        connect(lowEnergyController, &QLowEnergyController::disconnected, this,
                &BluetoothDevice::LowEnergyControllerDisconnected);
        connect(lowEnergyController, &QLowEnergyController::discoveryFinished, this,
                &BluetoothDevice::LowEnergyControllerDiscoveryFinished);
        connect(lowEnergyController, &QLowEnergyController::errorOccurred, this,
                &BluetoothDevice::LowEnergyControllerErrorOccurred);
        connect(lowEnergyController, &QLowEnergyController::serviceDiscovered, this,
                &BluetoothDevice::LowEnergyControllerServiceDiscovered);
        connect(lowEnergyController, &QLowEnergyController::stateChanged, this,
                &BluetoothDevice::LowEnergyControllerStateChanged);
    } else {
        if (lowEnergyController->state() != QLowEnergyController::UnconnectedState) {
            return;
        }
    }

    lowEnergyController->connectToDevice();
}

void BluetoothDevice::Disconnect(void) {
    if (lowEnergyController == nullptr) {
        return;
    }

    lowEnergyController->disconnectFromDevice();
}

void BluetoothDevice::Read(void) {
    if (communicationService == nullptr) {
        return;
    }

    communicationService->Read(communicationCharacteristicRead);
}

void BluetoothDevice::Write(QByteArray& value) {
    if (communicationService == nullptr) {
        return;
    }

    communicationService->Write(communicationCharacteristicWrite, value);
}

QBluetoothDeviceInfo& BluetoothDevice::DeviceInfoGet(void) {
    return deviceInfo;
}

void BluetoothDevice::LowEnergyControllerConnected(void) {
    emit Connected(this);
    lowEnergyController->discoverServices();
}

void BluetoothDevice::LowEnergyControllerConnectionUpdated(
    const QLowEnergyConnectionParameters& connectionParameters) {
    // TODO: Implement function
}

void BluetoothDevice::LowEnergyControllerDisconnected(void) {
    if (communicationService != nullptr) {
        disconnect(communicationService, &BluetoothService::CharacteristicChanged, this,
                   &BluetoothDevice::ServiceCharacteristicChanged);
        disconnect(communicationService, &BluetoothService::CharacteristicRead, this,
                   &BluetoothDevice::ServiceCharacteristicRead);
        disconnect(communicationService, &BluetoothService::CharacteristicWritten, this,
                   &BluetoothDevice::ServiceCharacteristicWritten);
    }

    communicationService = nullptr;
    services.clear();
    emit Disconnected(this);
}

void BluetoothDevice::LowEnergyControllerDiscoveryFinished(void) {
    bool serviceFound = false;

    // Check if the service is available on this device before the details are discovered. Safes
    // time
    for (BluetoothService* service : services) {
        if (service->ServiceGet()->serviceUuid() == UUID_SERVICE) {
            serviceFound = true;
        }
    }

    if (!serviceFound) {
        emit ServiceDiscoveryFailed(this);
        Disconnect();
        return;
    }

    // Iterate through the service list and start the discovery for the first service which requires
    // it
    for (serviceIndex = 0; serviceIndex < services.capacity(); ++serviceIndex) {
        if (services.at(serviceIndex)->DiscoverDetails()) {
            break;
        }
    }

    if (serviceIndex >= services.capacity()) {
        ServiceDetailsDiscovered();
    }
}

void BluetoothDevice::LowEnergyControllerErrorOccurred(QLowEnergyController::Error error) {}

/*
void BluetoothDevice::LowEnergyControllerMtuChanged(int mtu) {

}
*/

void BluetoothDevice::LowEnergyControllerServiceDiscovered(const QBluetoothUuid& uuid) {
    BluetoothService* service =
        new BluetoothService(lowEnergyController->createServiceObject(uuid));
    service->connect(service, &BluetoothService::DetailsDiscovered, this,
                     &BluetoothDevice::ServiceDetailsDiscovered);
    services.push_back(service);
}

void BluetoothDevice::LowEnergyControllerStateChanged(QLowEnergyController::ControllerState state) {
    // TODO: Implement function
}

void BluetoothDevice::ServiceDetailsDiscovered(void) {
    // Iterate through the rest of the service list and start the discovery for every service which
    // requires it
    for (; serviceIndex < services.capacity(); ++serviceIndex) {
        if (services.at(serviceIndex)->DiscoverDetails()) {
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

        if (service->ServiceGet()->serviceUuid() == UUID_SERVICE) {
            QList<QLowEnergyCharacteristic> characteristics =
                service->ServiceGet()->characteristics();

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
        emit ServiceDiscoveryFailed(this);
        Disconnect();
    }

    QLowEnergyDescriptor notification = communicationCharacteristicRead.descriptor(
        QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

    if (!notification.isValid()) {
        emit ServiceDiscoveryFailed(this);
        Disconnect();
    }

    communicationService->WriteDescriptor(notification, QByteArray::fromHex("0100"));

    connect(communicationService, &BluetoothService::CharacteristicChanged, this,
            &BluetoothDevice::ServiceCharacteristicChanged);
    connect(communicationService, &BluetoothService::CharacteristicRead, this,
            &BluetoothDevice::ServiceCharacteristicRead);
    connect(communicationService, &BluetoothService::CharacteristicWritten, this,
            &BluetoothDevice::ServiceCharacteristicWritten);
    emit ServiceDiscoveryFinished(this, services);
}

void BluetoothDevice::ServiceCharacteristicChanged(const QLowEnergyCharacteristic& characteristic,
                                                   const QByteArray& value) {
    emit CharacteristicChanged(this, value);
}

void BluetoothDevice::ServiceCharacteristicRead(const QLowEnergyCharacteristic& characteristic,
                                                const QByteArray& value) {
    emit CharacteristicRead(this, value);
}

void BluetoothDevice::ServiceCharacteristicWritten(const QLowEnergyCharacteristic& characteristic,
                                                   const QByteArray& value) {
    emit CharacteristicWritten(this, value);
}
