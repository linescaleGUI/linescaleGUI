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
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
// include always receive bytes for BT_Handler
Parser::Parser() {}
Parser::~Parser() {}

bool& Parser::parsePackage(QByteArray& package, DataStruct& data) {
    static bool dataOk = true;
    parsePackage(package, data, dataOk);
    return dataOk;
}

DataStruct& Parser::parsePackage(QByteArray& package, DataStruct& data, bool& dataOK) {
    dataOK = checkPackage(package);

    if (dataOK) {
        parseWorkingMode(package, data, dataOK);  // parses Working mode
        parseMeasuredValue(package, data);        // parses measured value
        parseMeasureMode(package, data, dataOK);
        parseReferenceZero(package, data);
        parseBatteryPercent(package, data);
        parseUnitValue(package, data, dataOK);
        parseFrequency(package, data, dataOK);
    }
    return data;
}

bool Parser::checkPackage(QByteArray& package) {
    QByteArray value;
    int checkVal = 0;

    int valueStart = 0;
    int valueLength = 17;

    for (int i = valueStart; i < valueLength; i++) {
        checkVal += int(package.at(i));
    }

    valueStart = 17;
    valueLength = 2;
    for (int i = valueStart; i < valueLength + valueStart; i++) {
        value.append(package.at(i));
    }
    if ((checkVal % 100) == value.toInt()) {
        return true;
    } else {
        return false;
    }
}

void Parser::parseWorkingMode(QByteArray& package, DataStruct& data, bool& dataOK) {
    switch (package.at(0)) {
        case 'R':
            data.workingMode = realTime;
            break;
        case 'O':
            data.workingMode = overloaded;
            break;
        case 'C':
            data.workingMode = maxCapacity;
            break;
        default:
            dataOK = false;
            break;
    }
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

void Parser::parseMeasureMode(QByteArray& package, DataStruct& data, bool& dataOK) {
    switch (package.at(7)) {
        case 'N':
            data.measureMode = absZero;
            break;
        case 'Z':
            data.measureMode = relZero;
            break;
        default:
            dataOK = false;
            break;
    }
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

void Parser::parseUnitValue(QByteArray& package, DataStruct& data, bool& dataOK) {
    switch (package.at(15)) {
        case 'N':
            data.unitValue = kN;
            break;
        case 'G':
            data.unitValue = kgf;
            break;
        case 'B':
            data.unitValue = lbf;
            break;
        default:
            dataOK = false;
            break;
    }
}

void Parser::parseFrequency(QByteArray& package, DataStruct& data, bool& dataOK) {
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
            dataOK = false;
            break;
    }
}
