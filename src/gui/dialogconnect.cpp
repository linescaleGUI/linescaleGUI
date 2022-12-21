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
 * @file dialogconnect.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 * 
 * @brief `DialogConnect` implementation
 *
 */

#include "dialogconnect.h"
#include <QPushButton>
#include "ui_dialogconnect.h"

DialogConnect::DialogConnect(comm::CommMaster* comm, QWidget* parent)
    : QDialog(parent), ui(new Ui::DialogConnect) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->comm = comm;

    // Button action
    connect(ui->btnConnect, &QPushButton::pressed, this, &DialogConnect::requestConnection);
    connect(ui->btnReload, &QPushButton::pressed, this, &DialogConnect::reloadConnections);
    connect(ui->boxConnections, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &DialogConnect::updateFrequencySelector);
    connect(comm, &comm::CommMaster::changedStateMaster, this,
            &DialogConnect::requestConnectionAccepted);
    connect(comm, &comm::CommMaster::discoveredDeviceMaster, this, &DialogConnect::discoveredDevice);
    connect(comm, &comm::CommMaster::discoverDevicesFinishedMaster, this, &DialogConnect::discoverDevicesFinished);
}

DialogConnect::~DialogConnect() {
    delete ui;
}

void DialogConnect::showEvent(QShowEvent* event) {
    reloadConnections();
    QWidget::showEvent(event);
}

void DialogConnect::reloadConnections() {
    ui->boxConnections->clear();
    devices.clear();

    // The connect button is only reenabled after the discovery has finished
    ui->boxConnections->addItem("No device");
    ui->btnConnect->setEnabled(false);
    ui->btnReload->setEnabled(false);
    ui->boxConnections->setEnabled(false);
    ui->boxFreq->setEnabled(false);

    comm->discoverDevices();
}

void DialogConnect::requestConnection() {
    if (devices.length() == 0) {
        // No devices available
        return;
    }

    int index = ui->boxConnections->currentIndex();
    if (index < 0 || index >= devices.length()) {
        // Index out of range
        return;
    }

    comm->addConnection(devices[index]);
}

void DialogConnect::updateFrequencySelector(int index) {
    if (devices.length() <= index || index < 0) {
        return;
    }

    ui->boxFreq->clear();
    ui->boxFreq->addItem("10 Hz", int(10));
    ui->boxFreq->addItem("40 Hz", int(40));

    if (devices[index].type == comm::ConnType::USB) {
        ui->boxFreq->addItem("640 Hz", int(640));
        ui->boxFreq->addItem("1280 Hz", int(1280));
    }
}

void DialogConnect::requestConnectionAccepted(bool connected) {
    if (!connected) {
        return;
    }

    comm->setNewFreq(ui->boxFreq->currentData().toInt());
    close();
}

void DialogConnect::discoveredDevice(comm::DeviceInfo& deviceInfo) {
    // If a device was found enable the ui elements again except the reload button
    if (devices.length() == 0) {
        ui->boxConnections->clear();
        ui->btnConnect->setEnabled(true);
        ui->boxConnections->setEnabled(true);
        ui->boxFreq->setEnabled(true);
    }

    ui->boxConnections->addItem(deviceInfo.ID);
    devices.append(deviceInfo);
}

void DialogConnect::discoverDevicesFinished(void) {
    // Enable reload button after the scan has finished
    ui->btnReload->setEnabled(true);
}
