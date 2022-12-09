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
 * @file dialogconfigure.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "dialogconfigure.h"
#include <QPushButton>
#include "ui_dialogconfigure.h"

DialogConfigure::DialogConfigure(CommMaster* comm, QWidget* parent)
    : QDialog(parent), ui(new Ui::DialogConfigure) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->comm = comm;

    // Button action
    connect(ui->btnConnect, &QPushButton::pressed, this, &DialogConfigure::requestConnection);
    connect(ui->boxConnections, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &DialogConfigure::updateFreq);

    // Updates from commMaster
    connect(comm, &CommMaster::changedStateMaster, this, &DialogConfigure::toggleConnectionGroup);

    initWidget();
    reloadConnections();
}

DialogConfigure::~DialogConfigure() {
    delete wConn;
    delete ui;
}

void DialogConfigure::reloadConnections() {
    ui->boxConnections->clear();
    devices.clear();
    devices = comm->pullAvailableDevices();
    for (int i = 0; i < devices.length(); ++i) {
        ui->boxConnections->addItem(devices[i].ID);
    }
}

void DialogConfigure::requestConnection() {
    int index = ui->boxConnections->currentIndex();
    qDebug() << devices[index].ID;
    
    // disable group on success
    toggleConnectionGroup((comm->addConnection(devices[index])));
}

void DialogConfigure::initWidget() {
    wConn = new ConnectionWidget();
    ui->frameLayout->addWidget(wConn);
}

void DialogConfigure::updateFreq(int index) {
    ui->boxFreq->clear();
    ui->boxFreq->addItem("10 Hz");
    ui->boxFreq->addItem("40 Hz");
    if (devices[index].type == connType::USB) {
        ui->boxFreq->addItem("640 Hz");
        ui->boxFreq->addItem("1280 Hz");
    }
}

void DialogConfigure::toggleConnectionGroup(bool connected) {
    ui->groupConnection->setEnabled(!connected);
}
