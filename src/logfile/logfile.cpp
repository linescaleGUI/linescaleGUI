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
 * @file logfile.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `Logfile` implementation
 *
 */

#include "logfile.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <algorithm>

int Logfile::load() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return -1;  // Unable to open file
    }

    QTextStream in(&file);

    int invalidLineNumber = parseMetadata(in);
    if (invalidLineNumber != 0) {
        return invalidLineNumber;  // Unable to parse metadata
    }

    int index = 0;
    float period = 1.0 / metadata.speed;

    while (!in.atEnd()) {
        bool success;
        float newForce = in.readLine().toFloat(&success);
        if (success) {
            if (newForce <= minForce) {
                minForce = newForce;
                minForceIndex = index;
            }
            if (newForce >= maxForce) {
                maxForce = newForce;
                maxForceIndex = index;
            }
            forceVector.append(newForce);
            timeVector.append(period * index);
            ++index;
        } else {
            invalidLineNumber = LINE_NUMBER_FORCE + index;
            break;
        }
    }
    return invalidLineNumber;
}

bool Logfile::write() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return false;
    }

    file.resize(0);
    QTextStream stream(&file);
    stream << metadata.deviceID << Qt::endl;
    stream << metadata.date << Qt::endl;
    stream << metadata.time << Qt::endl;
    stream << "LogNo=" << qSetFieldWidth(3) << qSetPadChar('0') << metadata.logNr << qSetFieldWidth(0) << Qt::endl;
    stream << "Unit=" << metadata.unit << Qt::endl;
    stream << "Mode=" << metadata.mode << Qt::endl;
    stream << "RelZero=" << QString::number(metadata.relZero, 'f', 2) << Qt::endl;
    stream << "Speed=" << metadata.speed << Qt::endl;
    stream << "Trig=" << QString::number(metadata.triggerForce, 'f', 2) << Qt::endl;
    stream << "Stop=" << QString::number(metadata.stopForce, 'f', 2) << Qt::endl;
    stream << "Pre=" << metadata.preCatch << Qt::endl;
    stream << "Catch=" << metadata.catchTime << Qt::endl;
    stream << "Total=" << metadata.totalTime << Qt::endl;

    QVectorIterator<float> forceDataIterate(forceVector);
    while (forceDataIterate.hasNext()) {
        stream << QString::number(forceDataIterate.next(), 'f', 2) << Qt::endl;
    }
    return true;
}

int Logfile::parseMetadata(QTextStream& in) {
    bool success;
    int lineNumber = 1;
    metadata.deviceID = in.readLine();
    if (metadata.deviceID == "" || metadata.deviceID.length() != 8) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.date = in.readLine();
    if (metadata.date == "" || metadata.date.length() != 8) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.time = in.readLine();
    if (metadata.time == "" || metadata.time.length() != 8) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.logNr = parseNumericField<int>(in.readLine(), "LogNo", success);
    if (!success) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.unit = parseUnit(splitToQString(in.readLine(), "Unit"));
    if (metadata.unit == UnitValue::NONE) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.mode = parseMeasureMode(splitToQString(in.readLine(), "Mode"));
    if (metadata.mode == MeasureMode::NONE) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.relZero = parseNumericField<float>(in.readLine(), "RelZero", success);
    if (!success) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.speed = parseNumericField<int>(in.readLine(), "Speed", success);
    if (!success) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.triggerForce = parseNumericField<float>(in.readLine(), "Trig", success);
    if (!success) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.stopForce = parseNumericField<float>(in.readLine(), "Stop", success);
    if (!success) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.preCatch = parseNumericField<int>(in.readLine(), "Pre", success);
    if (!success) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.catchTime = parseNumericField<int>(in.readLine(), "Catch", success);
    if (!success) {
        return lineNumber;
    }
    ++lineNumber;

    metadata.totalTime = parseNumericField<int>(in.readLine(), "Total", success);
    if (!success) {
        return lineNumber;
    }

    return 0;  // success
}

void Logfile::setPath(QString path) {
    filePath = path;
}

QString Logfile::getPath() {
    return QFileInfo(filePath).filePath();
}

QString Logfile::getFileName() {
    return QFileInfo(filePath).fileName();
}

QString Logfile::splitToQString(const QString& input, const QString& fieldName) {
    QStringList splitList = input.split("=");
    if (splitList.count() != 2) {
        return "";
    }

    if (splitList[0] != fieldName) {
        return "";
    }
    return splitList[1];
}

void Logfile::setMetadata(const Metadata& newData) {
    this->metadata = newData;
}

Metadata& Logfile::getMetadata() {
    return metadata;
}

const QVector<float>& Logfile::getForce() {
    return forceVector;
}

const QVector<float>& Logfile::getTime() {
    return timeVector;
}

void Logfile::setForce(const QVector<float>& force) {
    forceVector = force;
}

void Logfile::setTime(const QVector<float>& time) {
    timeVector = time;
}
