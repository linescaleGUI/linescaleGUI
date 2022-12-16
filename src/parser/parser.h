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
 * @file parser.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Parses the line scale communication protocol
 *
 */

#ifndef PARSER_H
#define PARSER_H

#include <QByteArray>
#include <QObject>

enum class WorkingMode { REALTIME, OVERLOADED, MAX_CAPACITY };
enum class MeasureMode { ABS_ZERO, REL_ZERO };
enum class UnitValue { KN, KGF, LBF };



struct Sample {        
    WorkingMode workingMode;
    double measuredValue;
    MeasureMode measureMode;
    double referenceZero;
    int batteryPercent;
    UnitValue unitValue;
    int frequency;
    /* data */
};


class Parser : public QObject {
    Q_OBJECT
   public:
   /**
    * @brief Parses received messages from Line Scale to usable data for GUI.
    * 
    * @param package Raw package as QByteArray 
    * @param data Parsed sample
    * @return true: If parsing was successful
    * @return false: If parsing resulted in an error
    */
    bool parsePackage(QByteArray& package, Sample& data);
    static constexpr size_t PACKET_EXPECTED_LEN = 20;

   private:
    bool checkPackage(QByteArray& package);
    bool parseWorkingMode(QByteArray& package, Sample& data);
    bool parseMeasuredValue(QByteArray& package, Sample& data);
    bool parseMeasureMode(QByteArray& package, Sample& data);
    bool parseReferenceZero(QByteArray& package, Sample& data);
    bool parseBatteryPercent(QByteArray& package, Sample& data);
    bool parseUnitValue(QByteArray& package, Sample& data);
    bool parseFrequency(QByteArray& package, Sample& data);

    static constexpr size_t PACKET_CHECKED_LEN = 17;
    static constexpr size_t PACKET_WORKING_MODE_INDEX = 0;
    static constexpr size_t PACKET_CHECKSUM_LOWER_DIGIT_INDEX = 18;
    static constexpr size_t PACKET_MEASURE_VALUE_START_INDEX = 1;
    static constexpr size_t PACKET_MEASURE_VALUE_LEN = 6;
    static constexpr size_t PACKET_CHECKSUM_UPPER_DIGIT_INDEX = 17;
    static constexpr size_t PACKET_MEASURE_MODE_INDEX = 7;
    static constexpr size_t PACKET_REFERENCE_ZERO_START_INDEX = 8;
    static constexpr size_t PACKET_REFERENCE_ZERO_LEN = 6;
    static constexpr size_t PACKET_BATTERY_PERCENT_INDEX = 14;
    static constexpr size_t PACKET_UNIT_VALUE_INDEX = 15;
    static constexpr size_t PACKET_FREQUENCY_INDEX = 16;
};

#endif  // PARSER_H
