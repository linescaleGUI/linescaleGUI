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
 * @file Bluetooth.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Bluetooth class declaration
 *
 */

#pragma once
#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <QLowEnergyController>
#include <QObject>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <vector>
#include "BluetoothDevice.h"

class Bluetooth : public QObject {
    Q_OBJECT

   public:
    Bluetooth();
    ~Bluetooth(void);
    void PowerOn(void);
    void PowerOff(void);
    void PowerToggle(void);
    bool IsPowerOn(void);

    void ScanStart(void);
    void ScanStop(void);
    void ScanToggle(void);
    bool IsScanning(void);
    std::vector<BluetoothDevice*>& DevicesGet(void);

   signals:
    // void PowerChanged(void);
    void PowerTurnedOn(void);
    void PowerTurnedOff(void);

    void ScanStarted(void);
    void ScanStopped(std::vector<BluetoothDevice*>& devices);
    void ScanDeviceDiscovered(BluetoothDevice* device);

    // void Connected(BluetoothDevice* device);
    // void Disconnected(BluetoothDevice* device);

   private slots:
    // void LocalDeviceConnected(const QBluetoothAddress &address);
    // void LocalDeviceDisconnected(const QBluetoothAddress &address);
    void LocalDeviceErrorOccurred(QBluetoothLocalDevice::Error error);
    void LocalDeviceHostModeStateChanged(QBluetoothLocalDevice::HostMode state);
    // void LocalDevicePairingFinished(const QBluetoothAddress &address,
    // QBluetoothLocalDevice::Pairing pairing);
    void DeviceDiscoveryAgentCanceled(void);
    void DeviceDiscoveryAgentDeviceDiscovered(const QBluetoothDeviceInfo& deviceInfo);
    void DeviceDiscoveryAgentDeviceUpdated(const QBluetoothDeviceInfo& deviceInfo,
                                           QBluetoothDeviceInfo::Fields fields);
    void DeviceDiscoveryAgentErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);
    void DeviceDiscoveryAgentFinished(void);

   private:
    QBluetoothLocalDevice::HostMode stateBefore;
    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceDiscoveryAgent deviceDiscoveryAgent;
    std::vector<BluetoothDevice*> devices;
};

#endif  // BLUETOOTH_H_
