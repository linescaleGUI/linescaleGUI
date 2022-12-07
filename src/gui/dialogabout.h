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
 * @file dialogabout.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Dialog to display version info
 *
 */

#pragma once
#ifndef DIALOGABOUT_H_
#define DIALOGABOUT_H_

#include <QDialog>

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog {
    Q_OBJECT

   public:
    DialogAbout(QWidget* parent = nullptr);
    ~DialogAbout();

   private:
    /** @brief Replace version placeholder in ui file with actual data
     * generated on compile time.
     * This contains github url, commit hash and compile time.
     */
    void replaceVersionInfo(void);
    Ui::DialogAbout* ui;
};

#endif  // DIALOGABOUT_H_
