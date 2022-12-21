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
 * @file commDevice.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `comm::CommDevice` declaration
 *
 */

#pragma once
#ifndef COMMDEVICE_H_
#define COMMDEVICE_H_

#include <QDebug>
#include <QObject>
#include "../parser/parser.h"

/**
 * @brief Namespace for everything related to the communication.
 *
 */
namespace comm {

/**
 * @brief Enum to describe the type used for a specific connection
 *
 */
enum class ConnType {
    BLE,  ///< Bluetooth low energy
    USB,  ///< Serial port via USB-mini
};

/**
 * @brief Struct to identify a device
 * Used to connect to one device.
 *
 */
struct DeviceInfo {
    ConnType type;  ///< Type of connection
    QString ID;     ///< Identifier of a given connection; e.g. COM101
    int baudRate;   ///< Baudrate, used by USB connection
};

/**
 * @brief Base class for every connection
 *
 */
class CommDevice : public QObject {
    Q_OBJECT

   public:
    /**
     * @brief Connect device to host
     * Implemented in USB or BLE class
     *
     * @return true Connection established successfully
     * @return false Connection failed
     */
    virtual bool connectDevice() { return false; };

    /**
     * @brief Disconnect device from host
     * Implemented in USB or BLE class
     *
     */
    virtual void disconnectDevice(){};

    /**
     * @brief Send date to the connected device
     *
     * @param data HEX values as a QByteArray to send
     */
    virtual void sendData(const QByteArray& data) { qDebug() << data; };

    /**
     * @brief Method to read data from the connected device.
     *
     * Called by e.g. SerialPort::readReady
     *
     */
    virtual void readData(){};

    /**
     * @brief Set a new frequency
     *
     * @param newFreq Frequency in Hz (10, 40, 640, 1280)
     */
    void setFreq(int newFreq) { freq = newFreq; };

    /**
     * @brief Get the Freq object
     *
     * @return int Current frequency in Hertz
     */
    int getFreq() const { return freq; };

    /**
     * @brief Get the Identifier of the connection
     *
     * @return QString Unique identifier for this connection
     */
    QString getIdentifier() const { return identifier; };

    /**
     * @brief Get the connection type of the connection
     *
     * @return ConnType enum with the type
     */
    ConnType getConnType() const { return type; };

    /**
     * @brief Get the connection status
     *
     * @return true if connected
     */
    bool getStatus() const { return connected; };

   signals:
    /**
     * @brief Emit after a package was received and parsed
     *
     * @param reading A new `Parser::Sample` from the device
     */
    void newSampleDevice(Sample reading);

    /**
     * @brief Emit after connection / disconnection to trigger UI changes
     *
     * @param connected
     */
    void changedStateDevice(bool connected);

   protected:
    int freq = 10;                  ///< Sample frequency of the connection
    QString identifier;             ///< Unique identifier
    ConnType type = ConnType::USB;  ///< USB or BLE
    bool connected;
    Parser parser;
    Sample receivedData;
};

}  // namespace comm

#endif  // COMMDEVICE_H_
