/******************************************************************************
 * Copyright (C) 2022 by Gschwind, Weber, Schoch, Niederberger.               *
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
 * @file parser.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 * @copyright (c) 2022 LineGrip Corp Ltd.
 *
 * @brief Parses the line scale communication protocol
 *
 */

#include "parser.h"

bool Parser::parsePackage(QByteArray& package, DataStruct& data) {
    bool dataOk = checkPackage(package);

    if (dataOk) {
        dataOk = parseWorkingMode(package, data); 
    }
    if (dataOk) {
        parseMeasuredValue(package, data);  
        dataOk = parseMeasureMode(package, data);
    }
    if (dataOk) {
        parseReferenceZero(package, data);
        parseBatteryPercent(package, data);
        dataOk = parseUnitValue(package, data);
    }
    if (dataOk) {
        dataOk = parseFrequency(package, data);
    }
    return dataOk;
}

bool Parser::checkPackage(QByteArray& package) {
    QByteArray value;
    int checkVal = 0;

    int valueStart = 0;
    int valueLength = 17;

    if (package.length() != expPackageLenght) {
        return false;
    }

    for (int i = valueStart; i < valueLength; i++) {
        checkVal += int(package.at(i));
    }

    value.append(package.at(17));
    value.append(package.at(18));

    if ((checkVal % 100) == value.toInt()) {
        return true;
    } else {
        return false;
    }
}

bool Parser::parseWorkingMode(QByteArray& package, DataStruct& data) {
    switch (package.at(0)) {
        case 'R':
            data.workingMode = WorkingMode::REALTIME;
            break;
        case 'O':
            data.workingMode = WorkingMode::OVERLOADED;
            break;
        case 'C':
            data.workingMode = WorkingMode::MAX_CAPACITY;
            break;
        default:
            return false;
            break;
    }
    return true;
}

void Parser::parseMeasuredValue(QByteArray& package, DataStruct& data) {
    QByteArray value;
    int valueStart = 1;
    int valueLength = 6;
    for (int i = valueStart; i < valueLength + valueStart; i++) {
        value.append(package.at(i));
    }
    data.measuredValue = value.toDouble();
}

bool Parser::parseMeasureMode(QByteArray& package, DataStruct& data) {
    switch (package.at(7)) {
        case 'N':
            data.measureMode = MeasureMode::ABS_ZERO;
            break;
        case 'Z':
            data.measureMode = MeasureMode::REL_ZERO;
            break;
        default:
            return false;
            break;
    }
    return true;
}

void Parser::parseReferenceZero(QByteArray& package, DataStruct& data) {
    QByteArray value;
    int valueStart = 8;
    int valueLength = 6;
    for (int i = valueStart; i < valueLength + valueStart; i++) {
        value.append(package.at(i));
    }
    data.referenceZero = value.toDouble();
}

void Parser::parseBatteryPercent(QByteArray& package, DataStruct& data) {
    data.batteryPercent = int(package.at(14) - ' ') * 2;
}

bool Parser::parseUnitValue(QByteArray& package, DataStruct& data) {
    switch (package.at(15)) {
        case 'N':
            data.unitValue = UnitValue::KN;
            break;
        case 'G':
            data.unitValue = UnitValue::KGF;
            break;
        case 'B':
            data.unitValue = UnitValue::LBF;
            break;
        default:
            return false;
            break;
    }
    return true;
}

bool Parser::parseFrequency(QByteArray& package, DataStruct& data) {
    switch (package.at(16)) {
        case 'S':
            data.frequency = 10;
            break;
        case 'F':
            data.frequency = 40;
            break;
        case 'M':
            data.frequency = 640;
            break;
        case 'Q':
            data.frequency = 1280;
            break;
        default:
            return false;
            break;
    }
    return true;
}
