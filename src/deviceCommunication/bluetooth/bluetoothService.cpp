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
 * @file bluetoothService.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "bluetoothService.h"

namespace comm {
BluetoothService::BluetoothService(QLowEnergyService* service) : service(service) {
    if (service == nullptr) {
        return;
    }

    connect(service, &QLowEnergyService::characteristicChanged, this,
            &BluetoothService::LowEnergyServiceCharacteristicChanged);
    connect(service, &QLowEnergyService::characteristicRead, this,
            &BluetoothService::LowEnergyServiceCharacteristicRead);
    connect(service, &QLowEnergyService::characteristicWritten, this,
            &BluetoothService::LowEnergyServiceCharacteristicWritten);
    connect(service, &QLowEnergyService::descriptorRead, this,
            &BluetoothService::LowEnergyServiceDescriptorRead);
    connect(service, &QLowEnergyService::descriptorWritten, this,
            &BluetoothService::LowEnergyServiceDescriptorWritten);
    connect(service, qOverload<QLowEnergyService::ServiceError>(&QLowEnergyService::error), this,
            &BluetoothService::LowEnergyServiceErrorOccurred);
    connect(service, &QLowEnergyService::stateChanged, this,
            &BluetoothService::LowEnergyServiceStateChanged);
}

BluetoothService::~BluetoothService() {
    if (service == nullptr) {
        return;
    }

    disconnect(service, &QLowEnergyService::characteristicChanged, this,
               &BluetoothService::LowEnergyServiceCharacteristicChanged);
    disconnect(service, &QLowEnergyService::characteristicRead, this,
               &BluetoothService::LowEnergyServiceCharacteristicRead);
    disconnect(service, &QLowEnergyService::characteristicWritten, this,
               &BluetoothService::LowEnergyServiceCharacteristicWritten);
    disconnect(service, &QLowEnergyService::descriptorRead, this,
               &BluetoothService::LowEnergyServiceDescriptorRead);
    disconnect(service, &QLowEnergyService::descriptorWritten, this,
               &BluetoothService::LowEnergyServiceDescriptorWritten);
    disconnect(service, qOverload<QLowEnergyService::ServiceError>(&QLowEnergyService::error), this,
               &BluetoothService::LowEnergyServiceErrorOccurred);
    disconnect(service, &QLowEnergyService::stateChanged, this,
               &BluetoothService::LowEnergyServiceStateChanged);

    // TODO: Is this redundant?
    delete service;
}

bool BluetoothService::DiscoverDetails(void) {
    switch (service->state()) {
        case QLowEnergyService::DiscoveryRequired:
            service->discoverDetails();
            return true;
        case QLowEnergyService::InvalidService:
        case QLowEnergyService::DiscoveringServices:
        case QLowEnergyService::ServiceDiscovered:
        case QLowEnergyService::LocalService:
        default:
            return false;
    }
}

QLowEnergyService* BluetoothService::ServiceGet(void) {
    return service;
}

void BluetoothService::Read(QLowEnergyCharacteristic& characteristic) {
    if (service == nullptr) {
        return;
    }

    service->readCharacteristic(characteristic);
}

void BluetoothService::Write(QLowEnergyCharacteristic& characteristic, const QByteArray& value) {
    if (service == nullptr) {
        return;
    }

    service->writeCharacteristic(characteristic, value);
}

void BluetoothService::ReadDescriptor(QLowEnergyDescriptor& descriptor) {
    if (service == nullptr) {
        return;
    }

    service->readDescriptor(descriptor);
}

void BluetoothService::WriteDescriptor(QLowEnergyDescriptor& descriptor, const QByteArray& value) {
    if (service == nullptr) {
        return;
    }

    service->writeDescriptor(descriptor, value);
}

/*
bool BluetoothService::UuidSet(const QBluetoothUuid &uuid) {

}
*/

void BluetoothService::LowEnergyServiceCharacteristicChanged(
    const QLowEnergyCharacteristic& characteristic,
    const QByteArray& value) {
    emit CharacteristicChanged(characteristic, value);
}

void BluetoothService::LowEnergyServiceCharacteristicRead(
    const QLowEnergyCharacteristic& characteristic,
    const QByteArray& value) {
    emit CharacteristicRead(characteristic, value);
}

void BluetoothService::LowEnergyServiceCharacteristicWritten(
    const QLowEnergyCharacteristic& characteristic,
    const QByteArray& value) {
    emit CharacteristicWritten(characteristic, value);
}

void BluetoothService::LowEnergyServiceDescriptorRead(const QLowEnergyDescriptor& descriptor,
                                                      const QByteArray& value) {
    emit DescriptorRead(descriptor, value);
}

void BluetoothService::LowEnergyServiceDescriptorWritten(const QLowEnergyDescriptor& descriptor,
                                                         const QByteArray& value) {
    emit DescriptorWritten(descriptor, value);
}

void BluetoothService::LowEnergyServiceErrorOccurred(QLowEnergyService::ServiceError error) {
    // TODO: Implement function
}

void BluetoothService::LowEnergyServiceStateChanged(QLowEnergyService::ServiceState state) {
    if (state == QLowEnergyService::ServiceDiscovered) {
        emit DetailsDiscovered();
    }
}
}
