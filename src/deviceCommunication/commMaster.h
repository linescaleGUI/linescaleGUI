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
 * @brief This file contains the class to control all connected devices
 *
 * The CommMaster handles all communication with the connected devices.
 * It abstracts the different types of connections so from this stage upwards
 * the type of connection doesn't matter anymore.
 *
 * Each device gets a label which is used as an identifier inside the entire code:
 * USB: Port name
 * BLE: tbd
 *
 */

#pragma once
#ifndef COMMMASTER_H_
#define COMMMASTER_H_

#include <QHash>
#include <QObject>
#include <QSerialPortInfo>
#include "commDevice.h"
#include "commUSB.h"

class CommMaster : public QObject {
    Q_OBJECT

   public:

    /**
     * @brief Destroy the Comm Master object
     * 
     */
    ~CommMaster();

    /**
     * @brief Search all possible devices on either USB or BLE
     *
     * @return QList<QString>& Reference to a list with all devices
     */
    QList<deviceInfo>& pullAvailableDevices();

    /**
     * @brief Send data to the connected devices
     *
     * @param rawData QByteArray with the data to send (including CRC)
     */
    void sendData(QByteArray& rawData);

    /**
     * @brief Create connection
     * 
     * @param identifier Struct with the device info 
     * @return true Connection established
     * @return false Connection failed
     */
    bool addConnection(deviceInfo identifier);

    /**
     * @brief Terminate the connection and remove all references with the class
     * 
     */
    void removeConnection();

   private:
    QList<deviceInfo> availableDevice;
    CommDevice* singleDevice = nullptr;
};

#endif  // COMMMASTER_H_
