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
    show();

    plotMerged = new Plot(this);

    listOfPlots.insert("plot0", new Plot(this));
    listOfPlots.insert("plot1", new Plot(this));
    listOfPlots.insert("plot2", new Plot(this));
    listOfPlots.insert("plot3", new Plot(this));

    currentPlotSelected = new Plot(this); // init with empty plot


    // Hide / show different parts of the editor
    connect(ui->actionFileBrowser, &QAction::triggered, this, &LogfileEditor::showFileBrowser);
    connect(ui->actionSelectedLogfile, &QAction::triggered, this, &LogfileEditor::showSelectedLogfile);
    connect(ui->btnRemoveFile, &QPushButton::pressed, this, &LogfileEditor::switchPlot);

    ui->layoutPlotSelected->addWidget(currentPlotSelected);
    ui->layoutPlotMerged->addWidget(plotMerged);

    ui->layoutPlotMerged->setStretchFactor(plotMerged, 2);
    ui->layoutPlotSelected->setStretchFactor(currentPlotSelected, 2);

    emulateData();
}

LogfileEditor::~LogfileEditor() {
    delete ui;
}

void LogfileEditor::emulateData(void) {
    for(int i = 0; i<50; ++i){
        listOfPlots["plot0"]->addData(i, 10*sin(i));
        listOfPlots["plot1"]->addData(i, 20*sin(i));
        listOfPlots["plot2"]->addData(i, 30*sin(i));
        listOfPlots["plot3"]->addData(i, 40*sin(i));
        plotMerged->addData(i, 100*cos(i));
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

void LogfileEditor::switchPlot() {
    static int index = 0;
    ++index;
    QString newPlot = QString("plot%1").arg(index%4);

    ui->layoutPlotSelected->replaceWidget(currentPlotSelected, listOfPlots[newPlot]);
    currentPlotSelected = listOfPlots[newPlot];
}
