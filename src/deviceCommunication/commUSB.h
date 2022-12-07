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
 * @file commUSB.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Child class for USB connection
 *
 *
 */

#pragma once
#ifndef COMMUSB_H_
#define COMMUSB_H_

#include <QDebug>
#include <QHash>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "commDevice.h"

class CommUSB : public CommDevice {
    Q_OBJECT

   public:
    CommUSB(deviceInfo identifier);
    virtual ~CommUSB();

    bool connectDevice() override;
    void disconnect() override;
    void sendData(QByteArray rawData) override;
    void readData() override;

   private:
    void handleError(QSerialPort::SerialPortError error);

    QSerialPort* serialPort = nullptr;
    int baudRate = 230400;
    deviceInfo identifier;
    bool connState;
    QString COMbuffer;
};

#endif  // COMMUSB_H_
