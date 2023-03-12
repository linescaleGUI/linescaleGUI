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
 * @file mainwindow.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `MainWindow` implementation
 *
 */

#include "mainwindow.h"
#include <QDesktopServices>
#include <QTimer>
#include "../deviceCommunication/command.h"
#include "../notification/notification.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    notification = new Notification(ui->textBrowserLog);
    comm = new comm::CommMaster();

    dAbout = new DialogAbout(this);
    dDebug = new DialogDebug(comm, this);
    dConnect = new DialogConnect(comm, this);
    ui->widgetConnection->setCommunicationMaster(comm);

    // menu actions
    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, dAbout, &DialogAbout::show);
    connect(ui->actionGitHub, &QAction::triggered, this, &MainWindow::openGitHubLink);
    connect(ui->actionDebug, &QAction::triggered, dDebug, &DialogDebug::show);
    connect(ui->actionShowLog, &QAction::triggered, this, &MainWindow::showLog);
    connect(ui->actionClearLog, &QAction::triggered, notification, &Notification::clear);
    connect(ui->actionSaveLog, &QAction::triggered, notification, &Notification::saveLog);
    connect(ui->actionAsPng, &QAction::triggered, this, [=] { ui->widgetChart->saveImage(notification); });

    // Tool bar actions
    connect(ui->actionConnect, &QAction::triggered, dConnect, &DialogConnect::show);
    connect(ui->actionDisconnect, &QAction::triggered, this, [=] { comm->removeConnection(); });
    connect(ui->actionStartStop, &QAction::triggered, this, &MainWindow::triggerReadings);

    // Buttons next to readings
    connect(ui->btnSetAbsoluteZero, &QPushButton::pressed, this, &MainWindow::sendSetAbsoluteZero);
    connect(ui->btnSetRelativeZero, &QPushButton::pressed, this, &MainWindow::sendSetRelativeZero);
    connect(ui->btnResetPeak, &QPushButton::pressed, this, &MainWindow::sendResetPeak);

    // updates from CommMaster
    connect(comm, &comm::CommMaster::newSampleMaster, this, &MainWindow::receiveNewSample);
    connect(comm, &comm::CommMaster::changedStateMaster, this, &MainWindow::toggleActions);

    // disable wait for close, automatic close after main window close
    dAbout->setAttribute(Qt::WA_QuitOnClose, false);
    dDebug->setAttribute(Qt::WA_QuitOnClose, false);
    dConnect->setAttribute(Qt::WA_QuitOnClose, false);

    // Set default log visibility to match the actionShowLog button
    showLog();

    // Init actions in the toolbar, deactivate actions that require a connected device
    toggleActions(false);
}

MainWindow::~MainWindow() {
    delete comm;
    delete ui;
    delete notification;
}

void MainWindow::openGitHubLink(void) {
    QDesktopServices::openUrl(QUrl("https://github.com/linescaleGUI/linescaleGUI"));
}

void MainWindow::showLog(void) {
    bool isChecked = ui->actionShowLog->isChecked();
    ui->textBrowserLog->setVisible(isChecked);
}

void MainWindow::sendResetPeak() {
    comm->sendData(command::RESETPEAK);
    maxValue = 0;
    ui->lblPeakForce->setText("-");
}

void MainWindow::sendSetAbsoluteZero() {
    comm->sendData(command::SETABSOLUTEMODE);
}

void MainWindow::sendSetRelativeZero() {
    comm->sendData(command::SETRELATIVEMODE);
    comm->sendData(command::SETZERO);
}

void MainWindow::triggerReadings() {
    if (!statusReading) {
        notification->push("Start reading");
        comm->sendData(command::REQUESTONLINE);
    } else {
        QTimer::singleShot(10, [=] { statusReading = false; });
        notification->push("Stop reading");
        comm->sendData(command::DISCONNECTONLINE);
    }
}

void MainWindow::receiveNewSample(Sample reading) {
    statusReading = true;
    if (currentUnit != reading.unitValue) {
        maxValue = 0;  // Trigger reset of peak value to update the unit
        currentUnit = reading.unitValue;
        switch (reading.unitValue) {
            case UnitValue::KN:
                unitString = " kN";
                break;
            case UnitValue::LBF:
                unitString = " lbf";
                break;

            case UnitValue::KGF:
                unitString = " kgf";
                break;
            default:
                break;
        }
    }

    if (reading.measuredValue >= maxValue) {
        maxValue = reading.measuredValue;
        ui->lblPeakForce->setText(QString("%1").arg(reading.measuredValue, 3, 'f', 2) + unitString);
    }
    ui->lblCurrentForce->setText(QString("%1").arg(reading.measuredValue, 3, 'f', 2) + unitString);
    ui->lblReferenceZero->setText(QString("%1").arg(reading.referenceZero, 3, 'f', 2) + unitString);
    ui->widgetConnection->updateWidget(reading);
    ui->widgetChart->addConsecutiveSample(reading);
}

void MainWindow::toggleActions(bool connected) {
    ui->actionDisconnect->setEnabled(connected);
    ui->actionStartStop->setEnabled(connected);
    ui->actionConnect->setEnabled(!connected);
    ui->widgetConnection->setEnabled(connected);
    ui->groupCurrent->setEnabled(connected);
    ui->groupPeak->setEnabled(connected);
}
