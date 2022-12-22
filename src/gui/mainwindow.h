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
 * @file mainwindow.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `MainWindow` declaration
 *
 */

#pragma once
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include "../deviceCommunication/commMaster.h"
#include "../notification/notification.h"
#include "../parser/parser.h"
#include "dialogabout.h"
#include "dialogconnect.h"
#include "dialogdebug.h"
#include "plotWidget.h"

QT_BEGIN_NAMESPACE
/**
 * @brief Namespace for all GUI elements
 * 
 */
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief Main window for the application linescaleGUI
 * 
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

   private:
    /**
     * @brief Open the github project page
     *
     */
    void openGitHubLink(void);

    /**
     * @brief Show or hide the logWindow
     *
     * Trigger the logWindow based on the action in the menubar.
     */
    void showLog(void);

    /**
     * @brief Slot triggered by the button "resetPeak"
     *
     * This slot sets the value on the right sidebar to zero and sends
     * the command to the connected device.
     */
    void sendResetPeak();

    /**
     * @brief Slot triggered by the button "setAbsoluteZero"
     *
     * This slot sets the device into absolute mode and resets the zero point
     *
     */
    void sendSetAbsoluteZero();

    /**
     * @brief Slot triggered by the button "setReferenceZero"
     *
     * This slots sets the device into relative mode and sets the current value
     * as the new zero.
     *
     */
    void sendSetRelativeZero();

   private slots:
    /**
     * @brief Receive new sample from CommMaster
     *
     * This slot updates the peak and current value of the right sidebar.
     * The correct unit is extracted from the `Sample` and set accordingly.
     * When a change in unit is detected, the peak value is reset.
     *
     * It also updates the bool `MainWindow::statusReading` keeping track of
     * the status of the connection.
     *
     * @param reading Current sample
     */
    void receiveNewSample(Sample reading);

    /**
     * @brief Toggle the GUI elements on connection
     *
     * Some GUI elements are only relevant / usable if a device is connected.
     * This slot toggles this elements if a change in the connection was
     * signaled by the CommMaster.
     *
     * @param connected Current connection state
     */
    void toggleActions(bool connected);

    /**
     * @brief Start or stop the readings
     *
     * Send the command to the connected device. If the host receives a new
     * statusReading, the bool `MainWindow::statusReading` will be enabled by
     * `MainWindow::receiveNewSample`.
     * If the host terminates the stream, the bool will be set to false after
     * a delay. This is to prevent buffered data from setting the bool to true.
     */
    void triggerReadings();

   private:
    void updateImportantValues(float time, float value);

   private:
    Ui::MainWindow* ui;
    comm::CommMaster* comm;
    DialogAbout* dAbout;
    DialogDebug* dDebug;
    DialogConnect* dConnect;
    Notification* notification;
    Plot* plot;
    float maxValue = 0;
    bool statusReading = false;  ///< Tracks whether the host reads data or not
    UnitValue currentUnit;       ///< Current unit value, used to detect a change
    QString unitString = "";     ///< Cache the current unitString
};

#endif  // MAINWINDOW_H_
