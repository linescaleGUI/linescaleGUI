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

#include <QHash>
#include <QMainWindow>
#include "../gui/dialogabout.h"
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

   private slots:
    /**
     * @brief Toggle the visibility of the file browser
     *
     */
    void showFileBrowser();

    /**
     * @brief Toggle the visibility of the selectedLogfile group
     *
     */
    void showSelectedLogfile();

    /**
     * @brief Select a logfile and open this from the filesystem
     *
     */
    void openFile();

    /**
     * @brief Insert a new logfile into the treeWidget
     *
     * @param logfile New logfile
     */
    void insertNewLogfile(Logfile* logfile);

    /**
     * @brief Update the output panel with the metadata from the selected logfile
     *
     * @param logfile Ptr to the logfile
     */
    void updateMetadata(Logfile* logfile);

    /**
     * @brief Slot to be called from the treeWidget
     *
     * Updates the selectedLogfile group with the current data
     *
     * @param current Ptr to the selected Item
     */
    void displayNewLogfile(QTreeWidgetItem* current);

    /**
     * @brief Remove the currently selected logfile from the treeWidget
     *
     */
    void removeCurrentSelectedLogfile();

    /**
     * @brief Remove all logfiles from the class and the treeWidget
     *
     */
    void removeAllLogfiles();

   private:
    Ui::LogfileEditor* ui;                 ///< Qt UI ptr
    Plot* currentPlotSelected;             ///< Big plot on the bottom
    Plot* plotMerged;                      ///< Small plot in the selectedLogfile group
    QString startingFolder;                ///< Tracks the current folder
    DialogAbout* dAbout;                   ///< About dialog
    QHash<QString, Logfile*> allLogfiles;  ///< Track all logfiles
};

#endif  // LOGFILEEDITOR_H_
