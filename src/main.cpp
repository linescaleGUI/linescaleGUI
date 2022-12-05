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
 * along with LinescaleGUI.  If not, see <http://www.gnu.org/licenses/>.      *
 ******************************************************************************/
/**
 * @file main.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 * @copyright (c) 2022 LineGrip Corp Ltd.
 *
 * @brief Main file of project linescaleGUI
 *
 */

#include <QApplication>
#include "gui/mainwindow.h"

/** @brief Entry point into project linescaleGUI */
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
