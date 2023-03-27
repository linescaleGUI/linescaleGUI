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
 * @file commMaster.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `comm::CommMaster` declaration
 *
 */

#pragma once
#ifndef COMMMASTER_H_
#define COMMMASTER_H_

#include <QObject>
#include "../notification/notification.h"
#include "bluetooth/bluetooth.h"
#include "commDevice.h"

namespace comm {

/**
 * @brief Class to handle all connections to the connected LineScales.
 *
 * The CommMaster handles all communication with the connected devices.
 * It abstracts the different types of connections so from this stage upwards
 * the type of connection doesn't matter anymore.
 *
 * Each device gets a label which is used as an identifier inside the entire code:
 * USB: Port name
 * BLE: tbd
 */
class CommMaster : public QObject {
    Q_OBJECT

   public:
    /**
     * @brief Constructor of the class
     *
     * @param notification Pointer to a notification instance to push messages
     * @param bluetooth Pointer to a bluetooth instance
     */
    CommMaster(Notification* notification, Bluetooth* bluetooth);

    /**
     * @brief Destroy the Comm Master object
     *
     */
    ~CommMaster();

    /**
     * @brief Search all possible devices on either USB or BLE
     *
     * `discoveredDeviceMaster` is emitted after a device was discovered. If all
     * devices have been discovered `discoverDevicesFinishedMaster` is emitted
     */
    void discoverDevices(void);

    /**
     * @brief Send data to the connected devices
     *
     * @param rawData QByteArray with the data to send (including CRC)
     */
    void sendData(const QByteArray& rawData);

    /**
     * @brief Send data to the connected devices
     *
     * @param rawData QString with the msg as HEX characters
     */
    void sendData(const QString& rawData);

    /**
     * @brief Create connection
     *
     * @param identifier Struct with the device info
     */
    void addConnection(const DeviceInfo identifier);

    /**
     * @brief Terminate the connection and remove all references with the class
     *
     */
    void removeConnection();

    /**
     * @brief Set a new frequency on the device
     *
     * @param newFreq Frequency in Hz (10, 40, 640, 1280)
     */
    void setNewFreq(int newFreq);

    /**
     * @brief Set a new unit on the device
     * 
     * @param unit `UnitValue` to switch to
     */
    void setNewUnit(UnitValue unit);

   signals:
    /**
     * @brief Emit after new sample was sent by a deviceClass
     *
     * @param reading Sample from device
     */
    void newSampleMaster(Sample reading);

    /**
     * @brief Emit after status change
     *
     * @param connected true if connected
     */
    void changedStateMaster(bool connected);

    /**
     * @brief Emit after a device was discovered
     *
     * @param deviceInfo Information of the discovered device
     */
    void discoveredDeviceMaster(DeviceInfo& deviceInfo);

    /**
     * @brief Emit after all devices have been discovered
     *
     */
    void discoverDevicesFinishedMaster(void);

   private slots:
    /**
     * @brief Slot to receive the emitted signal from a deviceClass
     *
     * @param reading Sample from device
     */
    void receiveSampleMaster(Sample reading);

    /**
     * @brief Slot to receive the updated state from a deviceClass
     *
     * @param connected true if connected
     */
    void getChangedState(bool connected);

    /**
     * @brief Slot to receive the emitted signal after a device was discovered
     *
     * @param deviceInfo Information of the discovered device
     */
    void discoveredDeviceBluetooth(DeviceInfo& deviceInfo);

    /**
     * @brief Slot to receive the emitted signal after the bluetooth device
     * discovery has started
     *
     */
    void discoverDevicesStartedBluetooth(void);

    /**
     * @brief Slot to receive the emitted signal after all bluetooth devices
     * have been discovered
     *
     */
    void discoverDevicesStoppedBluetooth(void);

   private:
    QList<DeviceInfo> availableDevices;
    CommDevice* singleDevice = nullptr;
    Notification* notification = nullptr;
    Bluetooth* bluetooth = nullptr;
};

}  // namespace comm

#endif  // COMMMASTER_H_
