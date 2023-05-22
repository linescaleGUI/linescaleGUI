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
    bool fileOpen = file.open(QIODevice::ReadOnly);
    if (!fileOpen) {
        return -1;  // Unable to open file
    }

    QTextStream in(&file);

    int invalidLineNumber = parseMetadata(in);
    if (invalidLineNumber != 0) {
        file.close();
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
    file.close();
    return invalidLineNumber;
}


bool Logfile::write() {
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        file.resize(0);
        QTextStream stream(&file);
        stream << metadata.deviceID << Qt::endl;
        stream << metadata.date << Qt::endl;
        stream << metadata.time << Qt::endl;
        stream << "LogNo=" << qSetFieldWidth(3) << qSetPadChar('0') << metadata.logNr << qSetFieldWidth(0) << Qt::endl;

        stream << "Unit=";
        switch (metadata.unit) {
            case UnitValue::KGF:
                stream << "kgf" << Qt::endl;
                break;
            case UnitValue::KN:
                stream << "kN" << Qt::endl;
                break;
            case UnitValue::LBF:
                stream << "lbf" << Qt::endl;
            default:
                stream << Qt::endl;
        }
        stream << "Mode=";
        switch (metadata.mode) {
            case MeasureMode::ABS_ZERO:
                stream << "ABS" << Qt::endl;
                break;
            case MeasureMode::REL_ZERO:
                stream << "REL" << Qt::endl;
                break;
            default:
                stream << Qt::endl;
        }

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
        file.close();
        return true;
    }
    return false;
}

int Logfile::parseMetadata(QTextStream& in) {
    bool success;
    metadata.deviceID = in.readLine();
    if(metadata.deviceID == "" || metadata.deviceID.length() != 8){
        return LINE_NUMBER_DEVICEID;
    }

    metadata.date = in.readLine();
    if(metadata.date == "" || metadata.date.length() != 8){
        return LINE_NUMBER_DATE;
    }

    metadata.time = in.readLine();
    if(metadata.time == "" || metadata.time.length() != 8){
        return LINE_NUMBER_TIME;
    }

    metadata.logNr = splitToFloat(in.readLine(), &success);
    if(!success){return LINE_NUMBER_LOGNR;}

    QString tempUnit = splitToQString(in.readLine()).toUpper();
    if (tempUnit == "KGF") {
        metadata.unit = UnitValue::KGF;
    } else if (tempUnit == "KN") {
        metadata.unit = UnitValue::KN;
    } else if (tempUnit == "LBF") {
        metadata.unit = UnitValue::LBF;
    } else {
        return LINE_NUMBER_UNIT;
    }

    QString tempMode = splitToQString(in.readLine()).toUpper();
    if (tempMode == "ABS") {
        metadata.mode = MeasureMode::ABS_ZERO;
    } else if (tempMode == "REL") {
        metadata.mode = MeasureMode::REL_ZERO;
    } else{
        return LINE_NUMBER_MODE;
    }

    metadata.relZero = splitToFloat(in.readLine(), &success);
    if(!success){return LINE_NUMBER_RELZERO;}

    metadata.speed = splitToInt(in.readLine(), &success);
    if(!success){return LINE_NUMBER_SPEED;}

    metadata.triggerForce = splitToFloat(in.readLine(), &success);
    if(!success){return LINE_NUMBER_TRIGGERFORCE;}

    metadata.stopForce = splitToFloat(in.readLine(), &success);
    if(!success){return LINE_NUMBER_STOPFORCE;}

    metadata.preCatch = splitToInt(in.readLine(), &success);
    if(!success){return LINE_NUMBER_PRECATCH;}

    metadata.catchTime = splitToInt(in.readLine(), &success);
    if(!success){return LINE_NUMBER_CATCHTIME;}

    metadata.totalTime = splitToInt(in.readLine(), &success);
    if(!success){return LINE_NUMBER_TOTALTIME;}

    return 0; // success
}

void Logfile::setPath(QString path) {
    file.setFileName(path);
}

QString Logfile::getPath() {
    return QFileInfo(file).filePath();
}

QString Logfile::getFileName() {
    return QFileInfo(file).fileName();
}

float Logfile::splitToFloat(QString input, bool* success) {
    QStringList splitList = input.split("=");
    if (splitList.count() == 2) {
        return splitList[1].toFloat(success);
    } else {
        *success = false;
        return NAN;
    }
}

int Logfile::splitToInt(QString input, bool* success) {
    QStringList splitList = input.split("=");
    if (splitList.count() == 2) {
        return splitList[1].toInt(success);
    } else {
        *success = false;
        return -1;
    }
}

QString Logfile::splitToQString(QString input) {
    QStringList splitList = input.split("=");
    if (splitList.count() == 2) {
        return splitList[1];
    } else {
        return "";
    }
}

void Logfile::setMetadata(Metadata& newData) {
    this->metadata = newData;
}

Metadata& Logfile::getMetadata() {
    return metadata;
}

QVector<float>& Logfile::getForce() {
    return forceVector;
}

QVector<float>& Logfile::getTime() {
    return timeVector;
}

void Logfile::setForce(QVector<float>& force) {
    forceVector = force;
}

void Logfile::setTime(QVector<float>& time) {
    timeVector = time;
}
