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
            &BluetoothService::lowEnergyServiceCharacteristicChanged);
    connect(service, &QLowEnergyService::characteristicRead, this,
            &BluetoothService::lowEnergyServiceCharacteristicRead);
    connect(service, &QLowEnergyService::characteristicWritten, this,
            &BluetoothService::lowEnergyServiceCharacteristicWritten);
    connect(service, &QLowEnergyService::descriptorRead, this,
            &BluetoothService::lowEnergyServiceDescriptorRead);
    connect(service, &QLowEnergyService::descriptorWritten, this,
            &BluetoothService::lowEnergyServiceDescriptorWritten);
    connect(service, qOverload<QLowEnergyService::ServiceError>(&QLowEnergyService::error), this,
            &BluetoothService::lowEnergyServiceErrorOccurred);
    connect(service, &QLowEnergyService::stateChanged, this,
            &BluetoothService::lowEnergyServiceStateChanged);
}

BluetoothService::~BluetoothService() {
    if (service == nullptr) {
        return;
    }

    disconnect(service, &QLowEnergyService::characteristicChanged, this,
               &BluetoothService::lowEnergyServiceCharacteristicChanged);
    disconnect(service, &QLowEnergyService::characteristicRead, this,
               &BluetoothService::lowEnergyServiceCharacteristicRead);
    disconnect(service, &QLowEnergyService::characteristicWritten, this,
               &BluetoothService::lowEnergyServiceCharacteristicWritten);
    disconnect(service, &QLowEnergyService::descriptorRead, this,
               &BluetoothService::lowEnergyServiceDescriptorRead);
    disconnect(service, &QLowEnergyService::descriptorWritten, this,
               &BluetoothService::lowEnergyServiceDescriptorWritten);
    disconnect(service, qOverload<QLowEnergyService::ServiceError>(&QLowEnergyService::error), this,
               &BluetoothService::lowEnergyServiceErrorOccurred);
    disconnect(service, &QLowEnergyService::stateChanged, this,
               &BluetoothService::lowEnergyServiceStateChanged);
}

bool BluetoothService::discoverDetails(void) {
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

QLowEnergyService* BluetoothService::getService(void) {
    return service;
}

void BluetoothService::read(QLowEnergyCharacteristic& characteristic) {
    if (service == nullptr) {
        return;
    }

    service->readCharacteristic(characteristic);
}

void BluetoothService::write(QLowEnergyCharacteristic& characteristic, const QByteArray& value) {
    if (service == nullptr) {
        return;
    }

    service->writeCharacteristic(characteristic, value);
}

void BluetoothService::readDescriptor(QLowEnergyDescriptor& descriptor) {
    if (service == nullptr) {
        return;
    }

    service->readDescriptor(descriptor);
}

void BluetoothService::writeDescriptor(QLowEnergyDescriptor& descriptor, const QByteArray& value) {
    if (service == nullptr) {
        return;
    }

    service->writeDescriptor(descriptor, value);
}

void BluetoothService::lowEnergyServiceCharacteristicChanged(
    const QLowEnergyCharacteristic& characteristic,
    const QByteArray& value) {
    emit characteristicChanged(characteristic, value);
}

void BluetoothService::lowEnergyServiceCharacteristicRead(
    const QLowEnergyCharacteristic& characteristic,
    const QByteArray& value) {
    emit characteristicRead(characteristic, value);
}

void BluetoothService::lowEnergyServiceCharacteristicWritten(
    const QLowEnergyCharacteristic& characteristic,
    const QByteArray& value) {
    emit characteristicWritten(characteristic, value);
}

void BluetoothService::lowEnergyServiceDescriptorRead(const QLowEnergyDescriptor& descriptor,
                                                      const QByteArray& value) {
    emit descriptorRead(descriptor, value);
}

void BluetoothService::lowEnergyServiceDescriptorWritten(const QLowEnergyDescriptor& descriptor,
                                                         const QByteArray& value) {
    emit descriptorWritten(descriptor, value);
}

void BluetoothService::lowEnergyServiceErrorOccurred(QLowEnergyService::ServiceError error) {
    // TODO: Implement function
}

void BluetoothService::lowEnergyServiceStateChanged(QLowEnergyService::ServiceState state) {
    if (state == QLowEnergyService::ServiceDiscovered) {
        emit detailsDiscovered();
    }
}
}
