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

    plotMerged = new Plot(this);
    currentPlotSelected = new Plot(this);  // init with empty plot

    Logfile* file0 = new Logfile();
    Logfile* file1 = new Logfile();

    listOfFiles.append(file0);
    listOfFiles.append(file1);

    file0->setPath("../../tests/inputFiles/logfile_long0.csv");
    file0->load();
    plotMerged->addLogfile(file0);
    updateMetadata(file0);

    file1->setPath("../../tests/inputFiles/logfile_long1.csv");
    file1->load();
    plotMerged->addLogfile(file1);

    // Hide / show different parts of the editor
    connect(ui->actionFileBrowser, &QAction::triggered, this, &LogfileEditor::showFileBrowser);
    connect(ui->actionSelectedLogfile, &QAction::triggered, this, &LogfileEditor::showSelectedLogfile);
    connect(ui->btnLoadFile, &QPushButton::pressed, this, &LogfileEditor::openFile);

    ui->layoutPlotSelected->addWidget(currentPlotSelected);
    ui->layoutPlotMerged->addWidget(plotMerged);

    ui->layoutPlotMerged->setStretchFactor(plotMerged, 2);
    ui->layoutPlotSelected->setStretchFactor(currentPlotSelected, 2);
}

LogfileEditor::~LogfileEditor() {
    delete ui;
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
        QFileDialog::getOpenFileName(this, tr("Open existing logfile"), "/home", tr("CSV Files (*.csv)"));
    Logfile* logfile = new Logfile();
    logfile->setPath(path);
    if(logfile->load()){
        listOfFiles.append(logfile);
        currentPlotSelected->addLogfile(logfile);
        qDebug() << "success";
    }
    else{
        qDebug() << "unable to load file";
    }
    delete logfile;
    qDebug() << path;
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
