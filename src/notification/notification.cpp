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
 * @file notfications.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 */

#include "notification.h"
#include <QMessageBox>
#include <QTime>

const QString Notification::stringColorStart[] = {
    "",
    "",
    "<font color=\"Orange\">",
    "<font color=\"DeepPink\">"
};

const QString Notification::stringColorEnd[] = {
    "",
    "",
    "</font>",
    "</font>"
};

const QString Notification::stringSeverity[] = {
    "",
    " Info",
    " Warning",
    " Error"
};

Notification::Notification(QTextBrowser* textBrowser) : textBrowser(textBrowser) {}

bool Notification::push(const QString& message, Severity severity, bool showDialog) {
    if ((textBrowser == nullptr) || (severity < SEVERITY_NONE) || (severity > SEVERITY_ERROR)) {
        return false;
    }

    QTime time = QTime::currentTime();
    QString string = stringColorStart[severity] + time.toString() + stringSeverity[severity] + ": " + stringColorEnd[severity] + message;
    textBrowser->append(string);

    if ((severity == SEVERITY_ERROR) && (showDialog == true)) {
        QMessageBox::critical(textBrowser->parentWidget(), stringSeverity[severity], message, QMessageBox::Cancel, QMessageBox::Cancel);
    }

    return true;
}
