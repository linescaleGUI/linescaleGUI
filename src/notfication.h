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
 * @file notfications.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Class to handle all notifications
 *
 */

#pragma once
#ifndef NOTFICATIONS_H_
#define NOTFICATIONS_H_

#include <QString>
#include <QTextBrowser>

class Notification {
public:
    /**
     * Severity level to which notifications can be set
     */
    enum Severity {
        SEVERITY_NONE,
        SEVERITY_INFO,
        SEVERITY_WARNING,
        SEVERITY_ERROR
    };

    /**
     * @brief   Constructor of the class
     * @param   textBrowser Pointer to a textBrowser to which the notifications
     *          be written
     */
    Notification(QTextBrowser* textBrowser);

    /**
     * @brief   Method to write a notification to the already specified text
     *          browser
     * @param   message Message which shall be written
     * @param   severity Severity level of the notification
     * @param   showDialog Only applies to SEVERITY_ERROR levels. By setting
     *          this parameter to false, the error dialog will not be shown.
     * @return  Returns false if an error occurred. True otherwise.
     */
    bool push(const QString& message, Severity severity = SEVERITY_INFO, bool showDialog = true);

private:
    static const QString stringColorStart[];
    static const QString stringColorEnd[];
    static const QString stringSeverity[];
    QTextBrowser* textBrowser;
};

#endif // NOTFICATIONS_H_
