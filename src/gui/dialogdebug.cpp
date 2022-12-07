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
 * @file dialogdebug.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "dialogdebug.h"
#include <QTime>

DialogDebug::DialogDebug(CommMaster* comm, QWidget* parent) : QDialog(parent), ui(new Ui::DialogDebug) {
    ui->setupUi(this);

    assert(comm != nullptr);
    this->comm = comm;

    // treeCmd, double click
    connect(ui->treeCmd, &QTreeView::doubleClicked, this, &DialogDebug::clickedTreeCmd);

    // buttons below treeCmd
    connect(ui->btnInsertToLine, &QPushButton::clicked, this, &DialogDebug::insertTreeCmd);
    connect(ui->btnSendCmd, &QPushButton::clicked, this, &DialogDebug::sendMsg);

    // buttons / input fields below textLogging
    connect(ui->btnSendMsg, &QPushButton::clicked, this, &DialogDebug::sendMsg);
    connect(ui->inputPayload, &QLineEdit::returnPressed, this, &DialogDebug::sendMsg);
    connect(ui->btnClearLog, &QPushButton::clicked, this, &DialogDebug::clearLog);
}

DialogDebug::~DialogDebug() {
    delete ui;
}

void DialogDebug::insertTreeCmd() {
    QTreeWidgetItem* cItem = ui->treeCmd->currentItem();
    if (cItem && cItem->childCount() == 0) {
        QString payload = cItem->data(1, 0).toString();
        qDebug() << payload;
        ui->inputPayload->setText(payload);
    } else {
        // ignore group items with no child object
        appendText("Nothing to insert", Qt::red);
    }
}

void DialogDebug::clickedTreeCmd() {
    insertTreeCmd();
    sendMsg();
}

void DialogDebug::appendText(const QString& text, const QColor& color) {
    QString printingText = QTime::currentTime().toString() + " " + text;
    ui->textLogging->setTextColor(color);
    ui->textLogging->append(printingText);
}

void DialogDebug::sendMsg() {
    QString payload = ui->inputPayload->text();
    appendText(payload, Qt::blue);
    comm->sendData(payload);
}

void DialogDebug::clearLog() {
    ui->textLogging->setText("");
    ui->inputPayload->setText("");
}
