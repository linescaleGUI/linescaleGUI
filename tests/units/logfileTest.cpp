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
 * @file logfileTest.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Test class for logfile implementation
 *
 */

#include <gtest/gtest.h>
#include <QDir>
#include <QTextStream>
#include "../../src/logfile/logfile.h"
#include "../../src/parser/parser.h"

namespace {

/**
 * @brief Compare two metadata's with each other
 *
 * @param lhs First `Metadata`
 * @param rhs Second `Metadata`
 */
void checkMetaData(Metadata& lhs, Metadata& rhs) {
    EXPECT_EQ(lhs.deviceID, rhs.deviceID);
    EXPECT_EQ(lhs.date, rhs.date);
    EXPECT_EQ(lhs.time, rhs.time);
    EXPECT_EQ(lhs.logNr, rhs.logNr);
    EXPECT_EQ(lhs.unit, rhs.unit);
    EXPECT_EQ(lhs.mode, rhs.mode);
    EXPECT_EQ(lhs.relZero, rhs.relZero);
    EXPECT_EQ(lhs.speed, rhs.speed);
    EXPECT_EQ(lhs.triggerForce, rhs.triggerForce);
    EXPECT_EQ(lhs.stopForce, rhs.stopForce);
    EXPECT_EQ(lhs.preCatch, rhs.preCatch);
    EXPECT_EQ(lhs.catchTime, rhs.catchTime);
    EXPECT_EQ(lhs.totalTime, rhs.totalTime);
}

void compareFiles(QFile& lhs, QFile& rhs) {
    if (!lhs.open(QIODevice::ReadOnly)) {
        FAIL() << "Unable to open lhs file";
    }

    if (!rhs.open(QIODevice::ReadOnly)) {
        FAIL() << "Unable to open rhs file";
    }

    QTextStream streamLHS(&lhs);
    QTextStream streamRHS(&rhs);

    int i = 0;
    while (!lhs.atEnd() && !rhs.atEnd()) {
        EXPECT_EQ(lhs.readLine(), rhs.readLine()) << "Error on line " << i;
        ++i;
    }
}

/**
 * @brief Test fixture for the read test
 *
 * Init logfile and expected data on each test
 * On teardown, compare the return values with the expected data
 *
 */
class LogfileReadTest : public ::testing::Test {
   protected:
    Logfile logfile;
    Metadata expectedMeta;
    QVector<float> expectedForceVector;
    float expectedMinForce;
    float expectedMaxForce;
    int expectedMinForceIndex;
    int expectedMaxForceIndex;

    void TearDown() override {
        checkMetaData(logfile.getMetadata(), expectedMeta);
        EXPECT_EQ(logfile.getForce(), expectedForceVector);
        EXPECT_EQ(logfile.getMinForce(), expectedMinForce);
        EXPECT_EQ(logfile.getMaxForce(), expectedMaxForce);
        EXPECT_EQ(logfile.getMinForceIndex(), expectedMinForceIndex);
        EXPECT_EQ(logfile.getMaxForceIndex(), expectedMaxForceIndex);
    }
};

/**
 * @brief Test fixture for the read test of incorrect logfiles
 *
 * Init logfile and success bool on each test
 * On teardown, check that the return value is false
 *
 */
class LogfileErrorTest : public ::testing::Test {
   protected:
    Logfile logfile;
    bool success;

    void TearDown() override { ASSERT_FALSE(logfile.load()); }
};

/**
 * @brief Helper method to create the `Metadata`
 *
 * @param deviceID          Id from the device
 * @param date              Logging date
 * @param time              Logging time
 * @param logNr             Number of the log
 * @param unit              Unit of the device
 * @param mode              Measure mode (either ABS or REL)
 * @param relZero           Delta from relative zero to absolute zero
 * @param speed             Frequency of the device
 * @param triggerForce      Force to trigger the recording
 * @param stopForce         Force to stop the recording
 * @param preCatch          Time before the triggerForce
 * @param catchTime         Time after the triggerForce
 * @param totalTime         Total time = preCatch + catchTime
 * @return Metadata
 */
Metadata createMetadata(QString deviceID,
                        QString date,
                        QString time,
                        int logNr,
                        UnitValue unit,
                        MeasureMode mode,
                        float relZero,
                        int speed,
                        float triggerForce,
                        float stopForce,
                        int preCatch,
                        int catchTime,
                        int totalTime) {
    Metadata metadata;
    metadata.deviceID = deviceID;
    metadata.date = date;
    metadata.time = time;
    metadata.logNr = logNr;
    metadata.unit = unit;
    metadata.mode = mode, metadata.relZero = relZero;
    metadata.speed = speed;
    metadata.triggerForce = triggerForce;
    metadata.stopForce = stopForce;
    metadata.preCatch = preCatch;
    metadata.catchTime = catchTime;
    metadata.totalTime = totalTime;
    return metadata;
}


// *****************************************************************************
// Read correct files
// *****************************************************************************

TEST_F(LogfileReadTest, readCorrectFile0) {
    logfile.setPath("../../../tests/inputFiles/logfile0.csv");
    ASSERT_TRUE(logfile.load());
    expectedMeta = createMetadata("6B:6C:05", "15.05.22", "16:14:25", 2, UnitValue::KN, MeasureMode::REL_ZERO, 0.02, 40,
                                  0.7, 0, 3, 15, 18);
    expectedForceVector << 1.41 << 4.56 << 314.15 << -271.82 << 345.45;
    expectedMinForce = -271.82;
    expectedMaxForce = 345.45;
    expectedMinForceIndex = 3;
    expectedMaxForceIndex = 4;
}

TEST_F(LogfileReadTest, readCorrectFile1) {
    logfile.setPath("../../../tests/inputFiles/logfile1.csv");
    ASSERT_TRUE(logfile.load());
    expectedMeta = createMetadata("FF:6C:05", "15.05.22", "16:14:25", 2, UnitValue::KGF, MeasureMode::ABS_ZERO, 0.02,
                                  40, 0.7, 0, 3, 15, 18);
    expectedForceVector << 1.41 << 4.56 << 314.15 << -271.82 << 345.45 << 2;
    expectedMinForce = -271.82;
    expectedMaxForce = 345.45;
    expectedMinForceIndex = 3;
    expectedMaxForceIndex = 4;
}


// *****************************************************************************
// Read incorrect files
// *****************************************************************************

TEST_F(LogfileErrorTest, splitError) {
    logfile.setPath("../../../tests/inputFiles/logfileErrorSplit.csv");
}

TEST_F(LogfileErrorTest, charInsteadFloat) {
    logfile.setPath("../../../tests/inputFiles/logfileErrorFloat.csv");
}

TEST_F(LogfileErrorTest, charInsteadInt) {
    logfile.setPath("../../../tests/inputFiles/logfileErrorInt.csv");
}

TEST_F(LogfileErrorTest, wrongUnit) {
    logfile.setPath("../../../tests/inputFiles/logfileErrorUnit.csv");
}

TEST(LogfileLoadTest, loadWithoutPath) {
    Logfile logfile;
    ASSERT_FALSE(logfile.load());
}


// *****************************************************************************
// Write data to logfile
// *****************************************************************************

TEST(LogfileWriteTest, writeCorrectData) {
    Logfile logfile;
    QString outputPath = "logfile_out.csv";
    logfile.setPath(outputPath);
    Metadata metadataToSave = createMetadata("FF:6C:05", "15.05.22", "16:14:25", 2, UnitValue::KGF,
                                             MeasureMode::ABS_ZERO, 0.02, 40, 0.7, 0, 3, 15, 18);
    QVector<float> force;
    force << 1.41 << 4.56 << 314.15 << -271.82 << 345.45 << 2.00;
    logfile.setMetadata(metadataToSave);
    logfile.setForce(force);
    logfile.write();

    QFile compareTo("../../../tests/inputFiles/logfile1.csv");
    QFile exportFile(outputPath);
    compareFiles(compareTo, exportFile);
    exportFile.remove();
}


// *****************************************************************************
// Test the tests
// *****************************************************************************

TEST(LogfileWriteTest, compareSameFile) {
    QFile compareTo0("../../../tests/inputFiles/logfile0.csv");
    QFile compareTo1("../../../tests/inputFiles/logfile0.csv");
    compareFiles(compareTo0, compareTo1);
}

}  // namespace
