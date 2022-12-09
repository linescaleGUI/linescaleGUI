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

CommUSB::CommUSB(deviceInfo identifier) {
    this->identifier = identifier;
};

CommUSB::~CommUSB() {
    CommUSB::disconnectDevice();
    delete serialPort;
}

void CommUSB::disconnectDevice() {
    if(serialPort != nullptr)
    {
        serialPort->close();
        connected = false;
        emit changedStateDevice(connected);
        delete serialPort;
        serialPort = nullptr;
    }
};

void CommUSB::sendData(QByteArray rawData) {
    serialPort->write(rawData);
    serialPort->flush();
};

void CommUSB::readData() {
    /// @todo add proper parser
    QByteArray data = serialPort->readAll();
    COMbuffer += data;
    QStringList COMBufferList = COMbuffer.split('\n');
    for(int i = 0; i < COMBufferList.length(); ++i)
    {
        QString currentMsg = COMBufferList[i];
        if(currentMsg.endsWith('\r'))
        {
            float force = currentMsg.mid(1, 6).remove('-').toFloat();

            COMbuffer.remove(currentMsg);
            emit newForceDevice(force);
        }
    }
};

void CommUSB::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError)
    {
        qDebug() << serialPort->errorString();
    }
}

bool CommUSB::connectDevice() {
    if(serialPort != nullptr){disconnectDevice();}
    serialPort = new QSerialPort();

    connect(serialPort, &QSerialPort::readyRead, this, &CommUSB::readData);
    connect(serialPort, &QSerialPort::errorOccurred, this, &CommUSB::handleError);

    serialPort->setBaudRate(baudRate);
    serialPort->setPortName(identifier.ID);
    connected = serialPort->open(QIODevice::ReadWrite);
    emit changedStateDevice(connected);
    return connected;
} 
