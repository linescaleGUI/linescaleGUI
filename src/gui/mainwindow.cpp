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
 */

#include "mainwindow.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTimer>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    dAbout = new DialogAbout(this);
    dDebug = new DialogDebug(this);

    // menu actions
    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, dAbout, &DialogAbout::show);
    connect(ui->actionGitHub, &QAction::triggered, this, &MainWindow::openGitHubLink);
    connect(ui->actionDebug, &QAction::triggered, dDebug, &DialogDebug::show);

    // disable wait for close, automatic close after main window close
    dAbout->setAttribute(Qt::WA_QuitOnClose, false);
}

MainWindow::~MainWindow() {
    delete dDebug;
    delete dAbout;
    delete ui;
}

void MainWindow::openGitHubLink(void) {
    QDesktopServices::openUrl(QUrl("https://github.com/linescaleGUI/linescaleGUI"));
}
