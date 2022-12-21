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
 * @file dialogconnect.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `DialogConnect` declaration
 *
 */

#pragma once
#ifndef DIALOGCONNECT_H_
#define DIALOGCONNECT_H_

#include <QDialog>
#include "../deviceCommunication/commMaster.h"
#include "connectionWidget.h"

namespace Ui {
class DialogConnect;
}

/**
 * @brief Dialog to handle the connection with a device.
 *
 * This dialog displays all available connections and a frequency selector.
 * The available devices are already filtered to only include compatible devices.
 * If the connection was successfully established, this dialog will close.
 *
 */
class DialogConnect : public QDialog {
    Q_OBJECT

   public:
    /**
     * @brief Constructor of the class
     *
     * @param comm Pointer to the communication master
     * @param parent Pointer to parent widget, used for parent/child relation of qt
     */
    DialogConnect(comm::CommMaster* comm, QWidget* parent = nullptr);
    ~DialogConnect();

   private slots:
    /**
     * @brief Reload comboBox with all available connections
     *
     * This method calls the commMaster instance to get an array of deviceInfos.
     * This data is then added to the device selector. This array is later used
     * when establishing a connection as reference.
     *
     * @note If no devices are available, the UI elements for establishing a
     * connection will be disabled. This prevents a call from
     * `DialogConnect::requestConnection` with an empty `DialogConnect::devices`
     * and guides the user.
     */
    void reloadConnections();

    /**
     * @brief Request a new connection from the commMaster
     *
     * After the user pressed the connectButton, this method calls the commMaster
     * and requests a new connection. If the connection was successfully
     * established the requested frequency from the comboBox will be sent to the
     * device.
     *
     * @note If the list `DialogConnect::devices` is empty or the index from the
     * combo box is out-of-bound, the method does nothing.
     */
    void requestConnection();

    /**
     * @brief Fill the frequency selector with the compatible frequencies.
     *
     * If the current index of the device selector box changed, this slot
     * fills the compatible frequencies into the frequency selector.
     * Here we differentiate between BLE and USB because the higher frequencies
     * are only supported by USB.
     *
     * @note The index sent by QComboBox::currentIndexChanged(int index) will be -1
     * if the box is empty, which would result in an array out-of-bounds access.
     * Thus the method does nothing if the index is -1.
     *
     * @param index Current selected index of the device selector.
     */
    void updateFrequencySelector(int index);

   private slots:
    /**
     * @brief Is called after a master communication has changed its connection
     * state
     */
    void requestConnectionAccepted(bool connected);

   private:
    /**
     * @brief Expands the `QWidget::showEvent` to update the available devices
     *
     * @param event QEvent from the QWidget class
     */
    void showEvent(QShowEvent* event) override;

    Ui::DialogConnect* ui;            ///< Default ui pointer from qt
    comm::CommMaster* comm;           ///< Pointer to the communication instance
    QList<comm::DeviceInfo> devices;  ///< List of available devices
};

#endif  // DIALOGCONNECT_H_
