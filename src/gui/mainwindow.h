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
 * @brief Mainwindow for the project linescaleGUI
 *
 */

#pragma once
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include "../deviceCommunication/commMaster.h"
#include "dialogabout.h"
#include "dialogconfigure.h"
#include "dialogdebug.h"
#include "../notification/notification.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

   private:
    /** @brief Open project in github with default browser */
    void openGitHubLink(void);
    void showLog(void);
    void sendResetPeak();
    void initDeviceWidget();

   private slots:
    void getNewForce(float value);
    void getChangedState(bool connected);
    void triggerReadings();

   private:
    Ui::MainWindow* ui;
    comm::CommMaster* comm;
    DialogAbout* dAbout;
    DialogDebug* dDebug;
    DialogConfigure* dConfig;
    Notification* notification;
    ConnectionWidget* connectionWidget;
    float maxValue = 0;
    bool reading;
};

#endif  // MAINWINDOW_H_
