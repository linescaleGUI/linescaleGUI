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
 * @file dialogdebug.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Dialog to show all available commands
 *
 */

#pragma once
#ifndef DIALOGDEBUG_H_
#define DIALOGDEBUG_H_

#include <QDialog>
#include "../deviceCommunication/commMaster.h"

namespace Ui {
class DialogDebug;
}

/**
 * @brief Send predefined snippets to the linescale
 *
 */
class DialogDebug : public QDialog {
    Q_OBJECT

   public:
    DialogDebug(CommMaster* comm, QWidget* parent = nullptr);
    ~DialogDebug();

   private slots:

    /**
     * @brief Insert the current selected item from treeCmd into inputPayload
     *
     */
    void insertTreeCmd();

    /**
     * @brief Insert the double clocked item from treeCmd into inputPayload
     *
     */
    void clickedTreeCmd();
    /** SLOT: append text into loggingText, with a defined color*/

    /**
     * @brief Append text into the logWindow with a predefined color
     *
     * @param text as a QString
     * @param color as QColor
     */
    void appendText(const QString& text, const QColor& color);

    /**
     * @brief Send msg to connected device
     *
     */
    void sendMsg();

    /**
     * @brief Clear the logWindow
     *
     */
    void clearLog();

   private:
    Ui::DialogDebug* ui;
    CommMaster* comm;
};

#endif  // DIALOGDEBUG_H_
