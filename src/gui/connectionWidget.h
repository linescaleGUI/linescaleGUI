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
 * @file connectionWidget.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `ConnectionWidget` declaration
 *
 */

#pragma once
#ifndef CONNECTIONWIDGET_H_
#define CONNECTIONWIDGET_H_

#include <QWidget>
#include "../deviceCommunication/commMaster.h"

namespace Ui {
class ConnectionWidget;
}

/**
 * @brief Widget to display informations about a single connection
 *
 * This class can display information extracted from the data stream to the user.
 * It also enables interaction with a device; e.g. change the frequency or
 * disconnect from a device.
 *
 * @todo Add method to update the remaining data (battery, frequency...)
 * @todo Differentiate between USB and BLE regarding supported frequencies
 *
 */
class ConnectionWidget : public QWidget {
    Q_OBJECT

   public:
    /**
     * @brief Constructor of the class
     *
     * The constructor initialize the frequency selector with the available
     * frequency values.
     *
     * @note Call the method `ConnectionWidget::setCommunicationMaster` directly
     * after construction to assign the `comm::CommMaster` instance.
     *
     * @param parent Pointer to parent widget, used for parent/child relation of qt
     */
    ConnectionWidget(QWidget* parent = nullptr);
    ~ConnectionWidget();

    /**
     * @brief Set the `comm::CommMaster` instance used by this `ConnectionWidget`.
     *
     * It is used to connect new devices and get all available devices.
     * This setter exists because this widget is set up automatically through
     * auto-generated code (as defined in `ui_dialogconnect.ui`) which requires
     * a constructor with a single `QWidget*` argument.
     *
     * @note Call this method directly after construction to assign
     * the `comm::CommMaster` instance.
     *
     * @param comm Pointer to the communication master used in mainwindow.h
     */
    void setCommunicationMaster(comm::CommMaster* comm);

    /**
     * @brief Updates the data inside the device widget
     *
     * @param readings `Sample` with the current data
     */
    void updateWidget(Sample readings);

   private slots:

    /**
     * @brief Request new frequency from the connected device
     *
     * Called upon a change on the frequency selector. Takes the userdata from
     * the selector and calls the `comm::CommMaster` instance to request a new frequency.
     *
     * @note The index sent by QComboBox::currentIndexChanged(int index) will be -1
     * if the box is empty, which would result in an array out-of-bounds access.
     * Thus the method does nothing if the index is -1.
     *
     * @param index Index of the current item in the frequency selector
     */
    void requestNewFreq(int index);

   private:
    /**
     * @brief Helper function for `updateWidget` to update the battery icon
     *
     * Replace the battery icon with the correct image for the current battery
     * level.
     *
     *
     * @param value Battery level from 0 to 100 in percent
     */
    void updateBatteryIcon(int value);

    /**
     * @brief Helper function for updateWidget to display the current frequency.
     * 
     * Updates the comboBox with the frequencies. The frequency is converted to 
     * the index of the comboBox, starting with index 0 for 10Hz.
     * 
     * @param frequency Frequency in Hz (10, 40, 640, 1280)
     */
    void updateCurrentFrequency(int frequency);
    
    Ui::ConnectionWidget* ui;
    comm::CommMaster* communication = nullptr;
};

#endif  // CONNECTIONWIDGET_H_
