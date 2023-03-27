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
 * @brief `ConnectionWidget` implementation
 *
 */

#include "connectionWidget.h"
#include <QPushButton>
#include "ui_connectionWidget.h"

ConnectionWidget::ConnectionWidget(QWidget* parent) : QWidget(parent), ui(new Ui::ConnectionWidget) {
    ui->setupUi(this);

    ui->boxFreq->addItem("10 Hz", static_cast<int>(10));
    ui->boxFreq->addItem("40 Hz", static_cast<int>(40));
    ui->boxFreq->addItem("640 Hz", static_cast<int>(640));
    ui->boxFreq->addItem("1280 Hz", static_cast<int>(1280));
    ui->boxFreq->setCurrentIndex(-1);  // clear box

    ui->boxUnit->addItem("kN", static_cast<int>(UnitValue::KN));
    ui->boxUnit->addItem("lbf", static_cast<int>(UnitValue::LBF));
    ui->boxUnit->addItem("kgf", static_cast<int>(UnitValue::KGF));
    ui->boxUnit->setCurrentIndex(-1);  // clear box

    connect(ui->btnRemove, &QPushButton::pressed, this, [=] { communication->removeConnection(); });
    connect(ui->boxFreq, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ConnectionWidget::requestNewFreq);
    connect(ui->boxUnit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ConnectionWidget::requestNewUnit);
}

ConnectionWidget::~ConnectionWidget() {
    delete ui;
}

void ConnectionWidget::setCommunicationMaster(comm::CommMaster* comm) {
    this->communication = comm;
}

void ConnectionWidget::requestNewFreq(int index) {
    if (index >= 0 && index < ui->boxFreq->count()) {
        int newFreq = ui->boxFreq->currentData().toInt();
        if (communication != nullptr) {
            communication->setNewFreq(newFreq);
        }
    }
}

void ConnectionWidget::requestNewUnit(int index) {
    if (index >= 0 && index < ui->boxUnit->count()) {
        UnitValue unit = UnitValue(ui->boxUnit->currentData().toInt());
        if (communication != nullptr) {
            communication->setNewUnit(unit);
        }
    }
}

void ConnectionWidget::updateWidget(Sample readings) {
    updateBatteryIcon(readings.batteryPercent);
    updateCurrentFrequency(readings.frequency);
    updateUnit(readings.unitValue);
}

void ConnectionWidget::updateBatteryIcon(int value) {
    if (value < 0) {
        value = 0;
    };
    if (value > 100) {
        value = 100;
    };
    ui->lblBattery->setText(QString("%1%").arg(value));
    QPixmap batteryLevel(":/linescaleGUI/icons/battery15.png");
    if (value >= 15 && value < 60) {
        batteryLevel = QPixmap(":/linescaleGUI/icons/battery50.png");
    }
    if (value >= 60 && value < 95) {
        batteryLevel = QPixmap(":/linescaleGUI/icons/battery75.png");
    }
    if (value >= 95) {
        batteryLevel = QPixmap(":/linescaleGUI/icons/battery100.png");
    }
    ui->iconBattery->setPixmap(batteryLevel);
    ui->lblBattery->setText(QString("%1").arg(value) + "%");
}

void ConnectionWidget::updateCurrentFrequency(int frequency) {
    int indexInComboBox = -1;
    switch (frequency) {
        case 10:
            indexInComboBox = 0;
            break;

        case 40:
            indexInComboBox = 1;
            break;

        case 640:
            indexInComboBox = 2;
            break;

        case 1280:
            indexInComboBox = 3;
            break;

        default:
            break;
    }

    if (indexInComboBox < ui->boxFreq->count() && indexInComboBox >= 0) {
        ui->boxFreq->setCurrentIndex(indexInComboBox);
    }
}

void ConnectionWidget::updateUnit(UnitValue unit) {
    int index = ui->boxUnit->findData(static_cast<int>(unit));
    ui->boxUnit->setCurrentIndex(index);
}
