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
 * @file connectionWidget.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "connectionWidget.h"
#include <QPushButton>
#include "ui_connectionWidget.h"

ConnectionWidget::ConnectionWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::ConnectionWidget) {
    ui->setupUi(this);

    connect(ui->btnRemove, &QPushButton::pressed, this, [=] { communication->removeConnection(); });
    connect(ui->boxFreq, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &ConnectionWidget::requestNewFreq);
    ui->boxFreq->addItem("10 Hz", int(10));
    ui->boxFreq->addItem("40 Hz", int(40));
    ui->boxFreq->addItem("640 Hz", int(640));
    ui->boxFreq->addItem("1280 Hz", int(1280));
}

ConnectionWidget::~ConnectionWidget() {
    delete ui;
}

void ConnectionWidget::setCommunicationMaster(comm::CommMaster* comm) {
    this->communication = comm;
}

void ConnectionWidget::requestNewFreq(int index) {
    if(index >= 0 &&  index < ui->boxFreq->count()) {
        int newFreq = ui->boxFreq->currentData().toInt();
        if(communication != nullptr) {
            communication->setNewFreq(newFreq);
        }
    }
}
