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

// #include "BT_INTERFACE.h"


enum class WorkingMode { REALTIME, OVERLOADED, MAX_CAPACITY };
enum class MeasureMode { ABS_ZERO, REL_ZERO };
enum class UnitValue { KN, KGF, LBF };
const int expPackageLenght = 20;

struct DataStruct {
    WorkingMode workingMode;
    double measuredValue;
    MeasureMode measureMode;
    double referenceZero;
    int batteryPercent;
    UnitValue unitValue;
    int frequency;
    /* data */
};

/**
 * @brief Parses received messages from Line Scale to usable data for GUI
 *
 */
class Parser : public QObject {
    Q_OBJECT
   public:
   /**
    * @brief todo
    * 
    * @param package 
    * @param data 
    * @return true 
    * @return false 
    */
    bool parsePackage(QByteArray& package, DataStruct& data);

   private:
   
    void pushRawMsg(QByteArray& rawMsg);
    bool checkPackage(QByteArray& package);
    bool parseWorkingMode(QByteArray& package, DataStruct& data);
    void parseMeasuredValue(QByteArray& package, DataStruct& data);
    bool parseMeasureMode(QByteArray& package, DataStruct& data);
    void parseReferenceZero(QByteArray& package, DataStruct& data);
    void parseBatteryPercent(QByteArray& package, DataStruct& data);    
    bool parseUnitValue(QByteArray& package, DataStruct& data);
    bool parseFrequency(QByteArray& package, DataStruct& data);

   signals:
    /**
     * @brief sends data in form of a signal to GUI
     *
     * @param data parsed data
     * @test connect
     */
    void newDataSet(DataStruct& data);
};

#endif  // PARSER_H
