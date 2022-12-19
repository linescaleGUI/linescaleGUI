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
 * @brief `Parser` declaration
 *
 */

#ifndef PARSER_H
#define PARSER_H

#include <QByteArray>
#include <QObject>

/**
 * @brief Indicates the working mode sent by the Line Scale
 *
 */
enum class WorkingMode {
    REALTIME,      ///< Indicates that the work is in real-time mode
    OVERLOADED,    ///< Indicates that the test is overloaded
    MAX_CAPACITY,  ///< Indicates the maximum capacity (default 3000 not received)
};
/**
 * @brief   Indicates if the measured force is relative to a previous set value or if it is absolute
 *
 */
enum class MeasureMode {
    ABS_ZERO,  ///< Represents the absolute zero measurement mode
    REL_ZERO,  ///< Represents the relative zero measurement mode
};
/**
 * @brief Unit of the measured force
 *
 */
enum class UnitValue {
    KN,   ///< Indicates the unit of measurement is kN
    KGF,  ///< The unit of measurement is kgf
    LBF,  ///< Indicates that the unit of measurement is lbf
};
/**
 * @brief A sample received from a LineScale.
 *
 * This struct holds all data that the LineScale sends in a single packet.
 * A single packet corresponds to a force sample. 
 * The device also sends auxiliary information such as the sampling frequency, 
 * percent of battery left, and so on.
 */
struct Sample {
    WorkingMode workingMode;  ///< Indicates the working mode sent by the Line Scale
    double measuredValue;     ///< Stores value of measured force
    MeasureMode measureMode;  ///< Indicates if the measured force is relative to a previous set
                              ///< value or if it is absolute
    double referenceZero;     ///< Stores reference force
    int batteryPercent;       ///< Stores battery voltage of the Line Scale in percent
    UnitValue unitValue;      ///< Stores the unit of the measured force
    int frequency;  ///< Stores the connection frequency between host device and Line Scale
};

/**
 * @brief Class to parse the received messages
 *
 */
class Parser : public QObject {
    Q_OBJECT
   public:
    /**
     * @brief Parses received messages from Line Scale to usable data for GUI.
     *
     * @todo Return error code instead of bool.
     *
     * @param package Raw package as QByteArray
     * @param data Parsed sample
     * @return true: If parsing was successful
     * @return false: If parsing resulted in an error
     */
    bool parsePackage(QByteArray& package, Sample& data);
    static constexpr size_t PACKET_EXPECTED_LEN =
        20;  ///< Length the package should have so it would be parsed correctly

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
