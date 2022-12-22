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
    // bool UuidSet(const QBluetoothUuid &uuid);
    bool DiscoverDetails(void);
    QLowEnergyService* ServiceGet(void);
    void Read(QLowEnergyCharacteristic& characteristic);
    void Write(QLowEnergyCharacteristic& characteristic, const QByteArray& value);
    void ReadDescriptor(QLowEnergyDescriptor& descriptor);
    void WriteDescriptor(QLowEnergyDescriptor& descriptor, const QByteArray& value);

   signals:
    void CharacteristicChanged(const QLowEnergyCharacteristic& characteristic,
                               const QByteArray& value);
    void CharacteristicRead(const QLowEnergyCharacteristic& characteristic,
                            const QByteArray& value);
    void CharacteristicWritten(const QLowEnergyCharacteristic& characteristic,
                               const QByteArray& value);
    void DescriptorRead(const QLowEnergyDescriptor& descriptor, const QByteArray& value);
    void DescriptorWritten(const QLowEnergyDescriptor& descriptor, const QByteArray& value);
    void ErrorOccurred(void);
    void DetailsDiscovered(void);

   private slots:
    void LowEnergyServiceCharacteristicChanged(const QLowEnergyCharacteristic& characteristic,
                                               const QByteArray& value);
    void LowEnergyServiceCharacteristicRead(const QLowEnergyCharacteristic& characteristic,
                                            const QByteArray& value);
    void LowEnergyServiceCharacteristicWritten(const QLowEnergyCharacteristic& characteristic,
                                               const QByteArray& value);
    void LowEnergyServiceDescriptorRead(const QLowEnergyDescriptor& descriptor,
                                        const QByteArray& value);
    void LowEnergyServiceDescriptorWritten(const QLowEnergyDescriptor& descriptor,
                                           const QByteArray& value);
    void LowEnergyServiceErrorOccurred(QLowEnergyService::ServiceError error);
    void LowEnergyServiceStateChanged(QLowEnergyService::ServiceState state);

   private:
    QLowEnergyService* service;
};
}  // namespace comm

#endif  // BLUETOOTHSERVICE_H_
