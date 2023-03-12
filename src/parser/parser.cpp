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
 * along with LinescaleGUI. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/
/**
 * @file parser.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `Parser` implementation
 *
 */

#include "parser.h"

bool Parser::parsePackage(QByteArray& package, Sample& data) {
    return checkPackage(package) && parseWorkingMode(package, data) &&
           parseMeasuredValue(package, data) && parseMeasureMode(package, data) &&
           parseReferenceZero(package, data) && parseBatteryPercent(package, data) &&
           parseUnitValue(package, data) && parseFrequency(package, data);
}

bool Parser::checkPackage(QByteArray& package) {
    QByteArray value;

    if (package.length() != PACKET_EXPECTED_LEN) {
        return false;
    }

    int checkVal = 0;
    for (int i = 0; i < PACKET_CHECKED_LEN; i++) {
        checkVal += int(package.at(i));
    }

    auto upperDigit = package.at(PACKET_CHECKSUM_UPPER_DIGIT_INDEX);
    auto lowerDigit = package.at(PACKET_CHECKSUM_LOWER_DIGIT_INDEX);
    if (!std::isdigit(upperDigit) || !std::isdigit(lowerDigit)) {
        return false;
    }
    size_t checksum = (upperDigit - '0') * 10 + (lowerDigit - '0');

    if ((checkVal % 100) == checksum) {
        return true;
    } else {
        return false;
    }
}

bool Parser::parseWorkingMode(QByteArray& package, Sample& data) {
    switch (package.at(PACKET_WORKING_MODE_INDEX)) {
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

bool Parser::parseMeasuredValue(QByteArray& package, Sample& data) {
    bool isOk;
    data.measuredValue =
        package.mid(PACKET_MEASURE_VALUE_START_INDEX, PACKET_MEASURE_VALUE_LEN).toDouble(&isOk);
    return isOk;
}

bool Parser::parseMeasureMode(QByteArray& package, Sample& data) {
    switch (package.at(PACKET_MEASURE_MODE_INDEX)) {
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

bool Parser::parseReferenceZero(QByteArray& package, Sample& data) {
    bool isOk;
    data.referenceZero =
        package.mid(PACKET_REFERENCE_ZERO_START_INDEX, PACKET_REFERENCE_ZERO_LEN).toDouble(&isOk);
    return isOk;
}

bool Parser::parseBatteryPercent(QByteArray& package, Sample& data) {
    data.batteryPercent = int(package.at(PACKET_BATTERY_PERCENT_INDEX) - 0x20) * 2;
    return true;
}

bool Parser::parseUnitValue(QByteArray& package, Sample& data) {
    switch (package.at(PACKET_UNIT_VALUE_INDEX)) {
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

bool Parser::parseFrequency(QByteArray& package, Sample& data) {
    switch (package.at(PACKET_FREQUENCY_INDEX)) {
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
