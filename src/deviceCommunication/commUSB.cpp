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
 * @file commUSB.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 *
 */

#include "commUSB.h"

namespace comm {

CommUSB::CommUSB(DeviceInfo identifier) {
    this->identifier = identifier;

    connect(&serialPort, &QSerialPort::readyRead, this, &CommUSB::readData);
    connect(&serialPort, &QSerialPort::errorOccurred, this, &CommUSB::handleError);
};

CommUSB::~CommUSB() {
    CommUSB::disconnectDevice();
}

void CommUSB::disconnectDevice() {
    if (connected) {
        serialPort.close();
        connected = false;
        emit changedStateDevice(connected);
    }
};

void CommUSB::sendData(const QByteArray& rawData) {
    serialPort.write(rawData);
    serialPort.flush();
};

void CommUSB::readData() {
    COMbuffer += serialPort.readAll();
    int msgLen = Parser::PACKET_EXPECTED_LEN;
    while (COMbuffer.length() >= msgLen) {
        if (COMbuffer[msgLen - 1] == '\r') {
            extractedMessage = COMbuffer.mid(0, msgLen);
            bool success = parser.parsePackage(extractedMessage, receivedData);
            if (success) {
                emit newSampleDevice(receivedData);
            }
            COMbuffer.remove(0, msgLen); // remove parsed package
        } else {
            COMbuffer.remove(0, 1);  // remove first byte and try again
        }
    }
}

void CommUSB::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError) {
        qDebug() << serialPort.errorString();
    }
}

bool CommUSB::connectDevice() {
    if (connected) {
        disconnectDevice();
    }

    serialPort.setBaudRate(identifier.baudRate);
    serialPort.setPortName(identifier.ID);
    connected = serialPort.open(QIODevice::ReadWrite);
    emit changedStateDevice(connected);
    return connected;
}
}  // namespace comm
