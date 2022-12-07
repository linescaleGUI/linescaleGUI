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
    delete serialPort;
}

void CommUSB::disconnect() {
    if(serialPort != nullptr)
    {
        serialPort->close();
        delete serialPort;
        serialPort = nullptr;
    }
};

void CommUSB::sendData(QByteArray rawData) {
    qDebug() << "From USB" << rawData; 
    serialPort->write(rawData);
    serialPort->flush();
};

void CommUSB::readData() {
    qDebug() << serialPort->readAll();
};

void CommUSB::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError)
    {
        qDebug() << serialPort->errorString();
    }
}

bool CommUSB::connectDevice() {
    if(serialPort != nullptr){disconnect();}
    serialPort = new QSerialPort();

    connect(serialPort, &QSerialPort::readyRead, this, &CommUSB::readData);
    connect(serialPort, &QSerialPort::errorOccurred, this, &CommUSB::handleError);

    serialPort->setBaudRate(baudRate);
    serialPort->setPortName(identifier.ID);
    // if(serialPort->open(QIODeviceBase::ReadWrite))
    // {
    //     connState = true;
    //     serialPort->setDataTerminalReady(false); //disable reset on open
    //     serialPort->write("");
    // }
    // else
    // {
    //     qDebug() << "No connection";
    // }
    return connState;
} 
