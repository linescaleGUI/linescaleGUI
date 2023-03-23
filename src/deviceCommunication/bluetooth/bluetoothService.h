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
 * @file bluetoothService.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Bluetooth service class declaration
 *
 */

#pragma once
#ifndef BLUETOOTHSERVICE_H_
#define BLUETOOTHSERVICE_H_

#include <QLowEnergyController>
#include <QObject>
#include <vector>

namespace comm {
class BluetoothService : public QObject {
    Q_OBJECT

   public:
    BluetoothService(QLowEnergyService* service);
    ~BluetoothService();
    bool discoverDetails(void);
    QLowEnergyService* getService(void);
    void read(QLowEnergyCharacteristic& characteristic);
    void write(QLowEnergyCharacteristic& characteristic, const QByteArray& value);
    void readDescriptor(QLowEnergyDescriptor& descriptor);
    void writeDescriptor(QLowEnergyDescriptor& descriptor, const QByteArray& value);

   signals:
    void characteristicChanged(const QLowEnergyCharacteristic& characteristic,
                               const QByteArray& value);
    void characteristicRead(const QLowEnergyCharacteristic& characteristic,
                            const QByteArray& value);
    void characteristicWritten(const QLowEnergyCharacteristic& characteristic,
                               const QByteArray& value);
    void descriptorRead(const QLowEnergyDescriptor& descriptor, const QByteArray& value);
    void descriptorWritten(const QLowEnergyDescriptor& descriptor, const QByteArray& value);
    void errorOccurred(void);
    void detailsDiscovered(void);

   private slots:
    void lowEnergyServiceCharacteristicChanged(const QLowEnergyCharacteristic& characteristic,
                                               const QByteArray& value);
    void lowEnergyServiceCharacteristicRead(const QLowEnergyCharacteristic& characteristic,
                                            const QByteArray& value);
    void lowEnergyServiceCharacteristicWritten(const QLowEnergyCharacteristic& characteristic,
                                               const QByteArray& value);
    void lowEnergyServiceDescriptorRead(const QLowEnergyDescriptor& descriptor,
                                        const QByteArray& value);
    void lowEnergyServiceDescriptorWritten(const QLowEnergyDescriptor& descriptor,
                                           const QByteArray& value);
    void lowEnergyServiceErrorOccurred(QLowEnergyService::ServiceError error);
    void lowEnergyServiceStateChanged(QLowEnergyService::ServiceState state);

   private:
    QLowEnergyService* service;
};
}  // namespace comm

#endif  // BLUETOOTHSERVICE_H_
