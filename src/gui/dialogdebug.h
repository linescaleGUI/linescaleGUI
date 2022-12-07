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
    DialogDebug(QWidget* parent = nullptr);
    ~DialogDebug();

   private:
    Ui::DialogDebug* ui;
};

#endif  // DIALOGDEBUG_H_
