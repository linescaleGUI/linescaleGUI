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
#include <QTextStream>
#include <QVector>
#include <limits>
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

/**
 * @brief Class to represent a logfile from the device
 *
 */
class Logfile {
   public:
    /**
     * @brief Open the file and parse the data
     *
     * @return int 0 on success; -1 if unable to open, first invalid line number on failure
     */
    int load();

    float getMinForce() { return minForce; }            ///< Return min force of the logfile
    float getMaxForce() { return maxForce; }            ///< Return max force of the logfile
    float getMinForceIndex() { return minForceIndex; }  ///< Return timestamp of min force
    float getMaxForceIndex() { return maxForceIndex; }  ///< Return timestamp of max force

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
     * @brief Get the name of the logfile without the path
     *
     * @return QString Name of the logfile
     */
    QString getFileName();

    /**
     * @brief Set a new metadata
     *
     * @param newData Reference to the new data
     */
    void setMetadata(const Metadata& newData);

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
    const QVector<float>& getForce();

    /**
     * @brief Get the timeVector
     *
     * @return QVector<float>&
     */
    const QVector<float>& getTime();

    /**
     * @brief Set the forceVector
     *
     * @param force
     */
    void setForce(const QVector<float>& force);

    /**
     * @brief Set the timeVector
     *
     * @param time
     */
    void setTime(const QVector<float>& time);

   private:
    /**
     * @brief Parse the metadata from a given text stream
     *
     * @param in QTextStream of the logfile
     * @return int 0 on success; first invalid line number on failure
     */
    int parseMetadata(QTextStream& in);

    /**
     * @brief Split the input line and return a float
     *
     * @tparam T Either int or float
     * @param input QString to parse to float
     * @param fieldName QString with the field name (in front of the = sign)
     * @param success Ptr to success bool
     * @return T Parsed value
     */
    template <typename T>
    T parseNumericField(const QString& input, const QString& fieldName, bool& success) {
        QStringList splitList = input.split("=");
        success = false;
        if (splitList.count() != 2) {
            return -1;
        }

        if (splitList[0] != fieldName) {
            return -1;
        }

        // Type depending code for int and float
        if constexpr (std::is_same_v<T, int>) {
            return splitList[1].toInt(&success);
        } else if constexpr (std::is_same_v<T, float>) {
            return splitList[1].toFloat(&success);
        } else {
            static_assert(!sizeof(T*), "T must be int or float");
        }
    }

    /**
     * @brief Split the input line and return a QString
     *
     * Return the value for a given fieldName. If the fieldName is invalid, it
     * returns an empty string
     *
     * @param input QString to split
     * @param fieldName QString with the field name (in front of the = sign)
     * @return QString
     */
    QString splitToQString(const QString& input, const QString& fieldName);

    QString filePath;
    Metadata metadata;
    QVector<float> forceVector;
    QVector<float> timeVector;
    float minForce = std::numeric_limits<float>::max();     ///< Minimum force present
    float maxForce = std::numeric_limits<float>::lowest();  ///< Maximum force present
    int minForceIndex = 0;                                  ///< Index of minForce
    int maxForceIndex = 0;                                  ///< Index of maxForce
    static constexpr int LINE_NUMBER_FORCE = 14;            ///< Start of the force vector
};

#endif  // LOGFILE_H_
