/******************************************************************************
 * Copyright (C) 2023 by Gschwind, Weber, Schoch, Niederberger                *
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
 * @file logfileEditor.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `LogfileEditor` implementation
 *
 */

#include "logfileEditor.h"
#include "ui_logfileEditor.h"

LogfileEditor::LogfileEditor(QWidget* parent) : QMainWindow(parent), ui(new Ui::LogfileEditor) {
    ui->setupUi(this);

    dAbout = new DialogAbout(this);
    plotMerged = new Plot(this);
    currentPlotSelected = new Plot(this);

    startingFolder = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    // Hide / show different parts of the editor
    connect(ui->actionFileBrowser, &QAction::triggered, this, &LogfileEditor::showFileBrowser);
    connect(ui->actionSelectedLogfile, &QAction::triggered, this, &LogfileEditor::showSelectedLogfile);

    // Menu action
    connect(ui->actionExit_file_editor, &QAction::triggered, this, &LogfileEditor::hide);
    connect(ui->actionAdd_logfile_from_disk, &QAction::triggered, this, &LogfileEditor::openFile);
    connect(ui->actionAbout, &QAction::triggered, dAbout, &DialogAbout::show);
    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionRemove_all_files, &QAction::triggered, this, &LogfileEditor::removeAllLogfiles);

    // TreeWidget
    connect(ui->btnLoadFile, &QPushButton::pressed, this, &LogfileEditor::openFile);
    connect(ui->btnRemoveFile, &QPushButton::pressed, this, &LogfileEditor::removeCurrentSelectedLogfile);
    connect(ui->treeLogfiles, &QTreeWidget::currentItemChanged, this, &LogfileEditor::displayNewLogfile);

    ui->layoutPlotSelected->addWidget(currentPlotSelected);
    ui->layoutPlotMerged->addWidget(plotMerged);

    ui->verticalLayout->setStretchFactor(plotMerged, 2);
    ui->layoutPlotSelected->setStretchFactor(currentPlotSelected, 2);
}

LogfileEditor::~LogfileEditor() {
    delete ui;
    foreach (Logfile* logfile, allLogfiles) {
        delete logfile;
    }
}

void LogfileEditor::showFileBrowser() {
    bool checked = ui->actionFileBrowser->isChecked();
    ui->widgetFileBrowser->setVisible(checked);
}

void LogfileEditor::showSelectedLogfile() {
    bool checked = ui->actionSelectedLogfile->isChecked();
    ui->groupSelLogfile->setVisible(checked);
}

void LogfileEditor::openFile() {
    QString path =
        QFileDialog::getOpenFileName(this, tr("Open existing logfile"), startingFolder, tr("Logfiles (*.csv)"));
    Logfile* logfile = new Logfile();
    logfile->setPath(path);
    int errorCode = logfile->load();

    if (errorCode == -1) {
        ui->outInfo->setText(QString("Unable to load file from %1").arg(logfile->getPath()));
        return;
    }

    if (errorCode != 0) {
        ui->outInfo->setText(QString("Unable to parse file; error on line %1").arg(errorCode));
        return;
    }

    if (allLogfiles.contains(logfile->getPath())) {
        ui->outInfo->setText(QString("Duplicated file, unable to load from %1").arg(logfile->getPath()));
        return;
    }

    allLogfiles.insert(logfile->getPath(), logfile);
    insertNewLogfile(logfile);
    currentPlotSelected->addLogfile(logfile, true);
    plotMerged->addLogfile(logfile);
    updateMetadata(logfile);
    ui->outInfo->setText(QString("Successfully loaded file from %1").arg(logfile->getPath()));
    QTimer::singleShot(2000, this, [=] { ui->outInfo->clear(); });
    startingFolder = logfile->getPath();  // open next logfile from same folder
}

void LogfileEditor::insertNewLogfile(Logfile* logfile) {
    QTreeWidgetItem* treeItem = new QTreeWidgetItem(ui->treeLogfiles);
    Metadata metadata = logfile->getMetadata();
    treeItem->setText(0, logfile->getFileName());
    treeItem->setText(1, metadata.deviceID);
    treeItem->setText(2, metadata.date);
    treeItem->setText(3, metadata.time);
    treeItem->setText(4, QString::number(metadata.logNr).rightJustified(3, '0'));
    treeItem->setData(0, Qt::UserRole, QVariant(logfile->getPath()));
}

void LogfileEditor::updateMetadata(Logfile* logfile) {
    Metadata metadata = logfile->getMetadata();
    ui->outDeviceID->setText(metadata.deviceID);
    ui->outDate->setText(metadata.date);
    ui->outTime->setText(metadata.time);
    ui->outLogNr->setNum(metadata.logNr);
    ui->outRelZero->setNum(metadata.relZero);
    ui->outSpeed->setNum(metadata.speed);
    ui->outTriggerForce->setNum(metadata.triggerForce);
    ui->outStopForce->setNum(metadata.stopForce);
    ui->outPreCatch->setNum(metadata.preCatch);
    ui->outCatchTime->setNum(metadata.catchTime);
    ui->outTotalTime->setNum(metadata.totalTime);

    ui->outMinForce->setNum(logfile->getMinForce());
    ui->outMaxForce->setNum(logfile->getMaxForce());

    QString minTime = QString::number(logfile->getMinForceIndex() / metadata.speed, 'f', 2);
    QString maxTime = QString::number(logfile->getMaxForceIndex() / metadata.speed, 'f', 2);

    ui->outMinTime->setText(minTime + " s");
    ui->outMaxTime->setText(maxTime + " s");

    QString temp;
    switch (metadata.unit) {
        case UnitValue::KGF:
            temp = "kgf";
            break;
        case UnitValue::KN:
            temp = "kN";
            break;
        case UnitValue::LBF:
            temp = "lbf";
        default:
            temp = "";
    }
    ui->outUnit->setText(temp);
}

void LogfileEditor::displayNewLogfile(QTreeWidgetItem* current) {
    if (current != nullptr) {
        QString path = current->data(0, Qt::UserRole).value<QString>();
        Logfile* logfile = allLogfiles.value(path);
        updateMetadata(logfile);
        currentPlotSelected->addLogfile(logfile, true);
    }
}

void LogfileEditor::removeCurrentSelectedLogfile() {
    QTreeWidgetItem* current = ui->treeLogfiles->currentItem();
    qDebug() << current;
    if (current != nullptr) {
        QString path = current->data(0, Qt::UserRole).value<QString>();
        qDebug() << path;
        if (allLogfiles.contains(path)) {
            allLogfiles.remove(path);
            delete current;
        }
    } else {
        ui->outInfo->setText("No file to remove");
    }
}

void LogfileEditor::removeAllLogfiles() {
    ui->treeLogfiles->clear();
    foreach (Logfile* logfile, allLogfiles) {
        delete logfile;
    }
    allLogfiles.clear();
}
