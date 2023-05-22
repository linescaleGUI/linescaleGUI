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
 * @file logfile.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief `Logfile` declaration
 *
 */

#pragma once
#ifndef LOGFILE_H_
#define LOGFILE_H_

#include <QFile>
#include <QVector>
#include "../parser/parser.h"

/**
 * @brief Metadata as saved in the current logfile
 *
 */
struct Metadata {
    QString deviceID;    ///< Id from the device
    QString date;        ///< Logging date
    QString time;        ///< Logging time
    int logNr;           ///< Number of the log
    UnitValue unit;      ///< Unit of the device
    MeasureMode mode;    ///< Measure mode (either ABS or REL)
    float relZero;       ///< Delta from relative zero to absolute zero
    int speed;           ///< Frequency of the device
    float triggerForce;  ///< Force to trigger the recording
    float stopForce;     ///< Force to stop the recording
    int preCatch;        ///< Time before the triggerForce
    int catchTime;       ///< Time after the triggerForce
    int totalTime;       ///< Total time = preCatch + catchTime
};

class Logfile {
   public:
    /**
     * @brief Open the file and parse the data
     *
     * @return true on success
     * @return false
     */
    bool load();

    float getMinForce() { return minForce; }
    float getMaxForce() { return maxForce; }
    float getMinForceIndex() { return minForceIndex; }
    float getMaxForceIndex() { return maxForceIndex; }

    /**
     * @brief Write the current metadata and force vector into a file
     *
     * @return true if successfully written to the harddisk
     */
    bool write();

    /**
     * @brief Set the path to the file
     *
     * @param path Path to the file
     */
    void setPath(QString path);

    /**
     * @brief Get the path of the logfile
     * 
     * @return QString Absolute path
     */
    QString getPath();

    /**
     * @brief Set a new metadata
     *
     * @param newData Reference to the new data
     */
    void setMetadata(Metadata& newData);

    /**
     * @brief Get a reference to the metadata
     *
     * @return Metadata&
     */
    Metadata& getMetadata();

    /**
     * @brief Get the forceVector
     * 
     * @return QVector<float>& 
     */
    QVector<float>& getForce();

    /**
     * @brief Get the timeVector
     * 
     * @return QVector<float>& 
     */
    QVector<float>& getTime();

    /**
     * @brief Set the forceVector
     *
     * @param force
     */
    void setForce(QVector<float>& force);

   private:
    /**
     * @brief Validate the metadata
     *
     * @return true
     * @return false
     */
    bool validateMetadata();

    /**
     * @brief Split the input line and return a float
     *
     * @param input
     * @return float
     */
    float splitToFloat(QString input, bool* success = nullptr);

    /**
     * @brief Split the input line and return a int
     *
     * @param input
     * @return int
     */
    int splitToInt(QString input, bool* success = nullptr);

    /**
     * @brief Split the input line and return a QString
     *
     * @param input
     * @return QString
     */
    QString splitToQString(QString input);

    QFile file;
    Metadata metadata;
    QVector<float> forceVector;
    QVector<float> timeVector;
    float minForce = FLT_MAX;   ///< Minimum force present
    float maxForce = -FLT_MAX;  ///< Maximum force present
    int minForceIndex = 0;      ///< Index of minForce
    int maxForceIndex = 0;      ///< Index of maxForce
};

#endif  // LOGFILE_H_
