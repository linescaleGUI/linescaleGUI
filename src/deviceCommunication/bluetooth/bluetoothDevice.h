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
 * @file BluetoothDevice.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Bluetooth device class declaration
 *
 */

#pragma once
#ifndef BLUETOOTHDEVICE_H_
#define BLUETOOTHDEVICE_H_

#include <QLowEnergyController>
#include <QObject>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <vector>
#include "../commDevice.h"
#include "BluetoothService.h"

namespace comm {
class BluetoothDevice : public CommDevice {
    Q_OBJECT

   public:
    BluetoothDevice(const QBluetoothDeviceInfo& deviceInfo);
    virtual ~BluetoothDevice();
    void connectDevice(void) override;
    void disconnectDevice(void) override;
    void readData(void) override;
    void sendData(const QByteArray& value) override;
    QBluetoothDeviceInfo& DeviceInfoGet(void);

   signals:
    void Connected(BluetoothDevice* device);
    void Disconnected(BluetoothDevice* device);
    void ServiceDiscoveryFinished(BluetoothDevice* device,
                                  std::vector<BluetoothService*>& services);
    void ServiceDiscoveryFailed(BluetoothDevice* device);

    void CharacteristicChanged(BluetoothDevice* device, const QByteArray& value);
    void CharacteristicRead(BluetoothDevice* device, const QByteArray& value);
    void CharacteristicWritten(BluetoothDevice* device, const QByteArray& value);

   private slots:
    void LowEnergyControllerConnected(void);
    void LowEnergyControllerConnectionUpdated(
        const QLowEnergyConnectionParameters& connectionParameters);
    void LowEnergyControllerDisconnected(void);
    void LowEnergyControllerDiscoveryFinished(void);
    void LowEnergyControllerErrorOccurred(QLowEnergyController::Error error);
    // void LowEnergyControllerMtuChanged(int mtu);
    void LowEnergyControllerServiceDiscovered(const QBluetoothUuid& uuid);
    void LowEnergyControllerStateChanged(QLowEnergyController::ControllerState state);

    void ServiceDetailsDiscovered(void);
    void ServiceCharacteristicChanged(const QLowEnergyCharacteristic& characteristic,
                                      const QByteArray& value);
    void ServiceCharacteristicRead(const QLowEnergyCharacteristic& characteristic,
                                   const QByteArray& value);
    void ServiceCharacteristicWritten(const QLowEnergyCharacteristic& characteristic,
                                      const QByteArray& value);

   private:
    static const QBluetoothUuid UUID_SERVICE;
    static const QBluetoothUuid UUID_CHARACTERISTIC_READ;
    static const QBluetoothUuid UUID_CHARACTERISTIC_WRITE;

    QBluetoothDeviceInfo deviceInfo;

    size_t serviceIndex;
    QLowEnergyController* lowEnergyController;
    std::vector<BluetoothService*> services;

    BluetoothService* communicationService;
    QLowEnergyCharacteristic communicationCharacteristicRead;
    QLowEnergyCharacteristic communicationCharacteristicWrite;
};
}  // namespace comm

#endif  // BLUETOOTHDEVICE_H_
