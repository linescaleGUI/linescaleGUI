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
 * @file logfileeditor.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `LogfileEditor` declaration
 *
 */

#pragma once
#ifndef LOGFILEEDITOR_H_
#define LOGFILEEDITOR_H_

#include <QMainWindow>
#include <QHash>
#include "../gui/plotWidget.h"

QT_BEGIN_NAMESPACE
/**
 * @brief Namespace for all GUI elements
 *
 */
namespace Ui {
class LogfileEditor;
}
QT_END_NAMESPACE

/**
 * @brief Main window for the sub application LogfileEditor
 *
 */
class LogfileEditor : public QMainWindow {
    Q_OBJECT

   public:
    LogfileEditor(QWidget* parent = nullptr);
    ~LogfileEditor();

    private:

    private slots:
    void showFileBrowser();
    void showSelectedLogfile();
    void openFile();
    void insertNewLogfile(Logfile* logfile);
    /**
     * @brief Update the output panel with the metadata from the selected logfile
     *
     * @param logfile Ptr to the logfile
     */
    void updateMetadata(Logfile* logfile);

private:

    Ui::LogfileEditor* ui;
    Plot* currentPlotSelected;
    Plot* plotMerged;

    QVector<Logfile*> listOfFiles;
};

#endif  // LOGFILEEDITOR_H_
