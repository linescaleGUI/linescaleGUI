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
 * @file commMaster.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "commMaster.h"

CommMaster::~CommMaster() {
    delete singleDevice;
}

bool CommMaster::addConnection(deviceInfo identifier) {
    if (singleDevice == nullptr) {
        removeConnection();
    }
    switch (identifier.type) {
        case connType::USB:
            singleDevice = new CommUSB(identifier);
            break;

        case connType::BLE:
            /// @todo add BLE ctor
            break;

        default:
            break;
    }

    if(singleDevice != nullptr) {
        return singleDevice->connectDevice();
    }
    else {
        return false;
    }
}

void CommMaster::removeConnection() {
    if (singleDevice != nullptr) {
        singleDevice->disconnect();
    }
    delete singleDevice;
    singleDevice = nullptr;
}

QList<deviceInfo>& CommMaster::pullAvailableDevices() {
    availableDevice.clear();

    QList<QSerialPortInfo> listOfCOMPorts = QSerialPortInfo::availablePorts();
    for (int i = 0; i < listOfCOMPorts.length(); ++i) {
        /// @todo check Manufacturer ID and Vendor ID -> return only LineScales
        deviceInfo tmp;
        tmp.ID = listOfCOMPorts[i].portName();
        tmp.type = connType::USB;
        availableDevice.append(tmp);
    }

    /// @todo Add code for BLE pull

    return availableDevice;
}

void CommMaster::sendData(QByteArray& rawData) {
    if (singleDevice != nullptr && rawData.length() > 0) {
        singleDevice->sendData(rawData);
    }
}
