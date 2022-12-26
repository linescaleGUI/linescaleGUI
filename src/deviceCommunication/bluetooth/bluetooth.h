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
 * @file bluetooth.h
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
#include "../commDevice.h"
#include "bluetoothDevice.h"

namespace comm {
class Bluetooth : public QObject {
    Q_OBJECT

   public:
    Bluetooth();
    ~Bluetooth();
    void powerTurnOn(void);
    void powerTurnOff(void);
    void powerSet(bool on);
    void powerToggle(void);
    bool isPowerOn(void);

    void scanStart(void);
    void scanStop(void);
    void scanToggle(void);
    bool isScanning(void);

   signals:
    void powerTurnedOn(void);
    void powerTurnedOff(void);

    void scanStarted(void);
    void scanStopped(void);
    void deviceDiscovered(DeviceInfo& deviceInfo);

   private slots:
    void localDeviceErrorOccurred(QBluetoothLocalDevice::Error error);
    void localDeviceHostModeStateChanged(QBluetoothLocalDevice::HostMode state);
    void deviceDiscoveryAgentCanceled(void);
    void deviceDiscoveryAgentDeviceDiscovered(const QBluetoothDeviceInfo& bluetoothDeviceInfo);
    void deviceDiscoveryAgentDeviceUpdated(const QBluetoothDeviceInfo& bluetoothDeviceInfo,
                                           QBluetoothDeviceInfo::Fields fields);
    void deviceDiscoveryAgentErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);
    void deviceDiscoveryAgentFinished(void);

   private:
    static const QString FILTER_NAME;

    QBluetoothLocalDevice::HostMode stateBefore;
    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceDiscoveryAgent deviceDiscoveryAgent;
};
}  // namespace comm

#endif  // BLUETOOTH_H_
