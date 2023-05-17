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
#include <QTextStream>
#include <algorithm>

bool Logfile::load() {
    bool forceSuccess = true;  // to enable AND connection with all other success flags
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        metadata.deviceID = in.readLine();
        metadata.date = in.readLine();
        metadata.time = in.readLine();
        metadata.logNr = splitToFloat(in.readLine());
        QString tempUnit = splitToQString(in.readLine()).toUpper();
        QString tempMode = splitToQString(in.readLine()).toUpper();
        metadata.relZero = splitToFloat(in.readLine());
        metadata.speed = splitToInt(in.readLine());
        metadata.triggerForce = splitToFloat(in.readLine());
        metadata.stopForce = splitToFloat(in.readLine());
        metadata.preCatch = splitToInt(in.readLine());
        metadata.catchTime = splitToInt(in.readLine());
        metadata.totalTime = splitToInt(in.readLine());

        if (tempUnit == "KGF") {
            metadata.unit = UnitValue::KGF;
        } else if (tempUnit == "KN") {
            metadata.unit = UnitValue::KN;
        } else if (tempUnit == "LBF") {
            metadata.unit = UnitValue::LBF;
        }

        if (tempMode == "ABS") {
            metadata.mode = MeasureMode::ABS_ZERO;
        } else if (tempMode == "REL") {
            metadata.mode = MeasureMode::REL_ZERO;
        }

        int index = 0;
        while (!in.atEnd()) {
            bool singleSuccess;
            float newForce = in.readLine().toFloat(&singleSuccess);
            if(newForce <= minForce){
                minForce = newForce;
                minForceIndex = index;
            }
            if(newForce >= maxForce){
                maxForce = newForce;
                maxForceIndex = index;
            }
            forceData.append(newForce);
            forceSuccess = singleSuccess && forceSuccess;
            ++index;
        }
        file.close();
    }
    return forceSuccess && validateMetadata();
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

        QVectorIterator<float> forceDataIterate(forceData);
        while (forceDataIterate.hasNext()) {
            stream << QString::number(forceDataIterate.next(), 'f', 2) << Qt::endl;
        }
        file.close();
        return true;
    }
    return false;
}

void Logfile::setPath(QString path) {
    file.setFileName(path);
}

bool Logfile::validateMetadata() {
    return metadata.deviceID != ""                                                                                    //
           && metadata.deviceID.length() == 8                                                                         //
           && metadata.date != ""                                                                                     //
           && metadata.date.length() == 8                                                                             //
           && metadata.time != ""                                                                                     //
           && metadata.time.length() == 8                                                                             //
           && metadata.logNr > 0                                                                                      //
           && (metadata.unit == UnitValue::KGF || metadata.unit == UnitValue::KN || metadata.unit == UnitValue::LBF)  //
           && (metadata.mode == MeasureMode::ABS_ZERO || metadata.mode == MeasureMode::REL_ZERO)                      //
           && !isnan(metadata.relZero)                                                                                //
           && (metadata.speed == 10 || metadata.speed == 40 || metadata.speed == 640 || metadata.speed == 1280)       //
           && !isnan(metadata.triggerForce)                                                                           //
           && !isnan(metadata.stopForce)                                                                              //
           && metadata.preCatch >= 0                                                                                  //
           && metadata.catchTime >= 0                                                                                 //
           && metadata.totalTime >= 0;                                                                                //
}

float Logfile::splitToFloat(QString input, bool* success) {
    QStringList splitList = input.split("=");
    if (splitList.count() == 2) {
        return splitList[1].toFloat(success);
    } else {
        return NAN;
    }
}

int Logfile::splitToInt(QString input, bool* success) {
    QStringList splitList = input.split("=");
    if (splitList.count() == 2) {
        return splitList[1].toInt(success);
    } else {
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
    return forceData;
}

void Logfile::setForce(QVector<float>& force) {
    forceData = force;
}
