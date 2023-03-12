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
 * @file parserTest.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Test class for parser implementation
 *
 * Tests parser functionality with various inputs an compares it to their
 * respective expected outcomes.
 *
 */

#include <gtest/gtest.h>
#include "../../src/parser/parser.h"

namespace {

/**
 * @brief Test fixture; init parser and sample before each test
 * 
 */
class ParserTest : public ::testing::Test {
   protected:
    Parser parser;
    Sample result;
};

/**
 * @brief Compare two values with each other 
 * 
 * @param lhs First `Sample`
 * @param rhs Second `Sample`
 */
void checkDataStruct(Sample& lhs, Sample& rhs) {
    EXPECT_EQ(lhs.workingMode, rhs.workingMode);
    EXPECT_EQ(lhs.measuredValue, rhs.measuredValue);
    EXPECT_EQ(lhs.measureMode, rhs.measureMode);
    EXPECT_EQ(lhs.referenceZero, rhs.referenceZero);
    EXPECT_EQ(lhs.batteryPercent, rhs.batteryPercent);
    EXPECT_EQ(lhs.unitValue, rhs.unitValue);
    EXPECT_EQ(lhs.frequency, rhs.frequency);
}

// *****************************************************************************
// Unit Value Tests
// *****************************************************************************

// kN
TEST_F(ParserTest, ParseUnitValueKn) {
    QByteArray KnPackage("R-00.01N000.00?NF41\r");
    Sample expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(KnPackage, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}

// Lbf
TEST_F(ParserTest, ParseUnitValueLbf) {
    QByteArray lbfPackage("R999999N000.00?BF87\r");
    Sample expectedResult = {WorkingMode::REALTIME, 999999, MeasureMode::ABS_ZERO, 0, 62, UnitValue::LBF, 40};
    bool dataOk = parser.parsePackage(lbfPackage, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}

// kgf
TEST_F(ParserTest, ParseUnitValueKgf) {
    QByteArray kgfPackage("R000019Z000001RGS95\r");
    Sample expectedResult = {WorkingMode::REALTIME, 19,MeasureMode::REL_ZERO, 1, 100, UnitValue::KGF, 10};
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}

// *****************************************************************************
// Checksum Tests
// *****************************************************************************

// Package with a incorrect checksum
TEST_F(ParserTest, ParseIncorrectChecksum) {
    QByteArray kgfPackage("R000.63Z-32.84RNS11\r");
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_FALSE(dataOk) << "Unable to detect incorrect checksum";
}

// Package with a correct checksum
TEST_F(ParserTest, ParseCorrectChecksum) {
    QByteArray kgfPackage("R000.63Z-32.84RNS10\r");
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_TRUE(dataOk) << "Unable to detect correct checksum";
}

// *****************************************************************************
// Package length Tests
// *****************************************************************************

// missing 'R' at the beginning -> length = 19
TEST_F(ParserTest, ParseToShortPackage) {
    QByteArray toShortPackage("-00.01N000.00?NF41\r");
    bool dataOk = parser.parsePackage(toShortPackage, result);
    ASSERT_FALSE(dataOk) << "Unable to detect package with length 19";
}

// one 'R' to many at the beginning -> length = 21
TEST_F(ParserTest, ParseToLongPackage) {
    QByteArray toLongPackage("RR-00.01N000.00?NF41\r");
    bool dataOk = parser.parsePackage(toLongPackage, result);
    ASSERT_FALSE(dataOk) << "Unable to detect package with length 21";
}

// *****************************************************************************
// WorkingMode Tests
// *****************************************************************************
TEST_F(ParserTest, ParseWorkingModeRealtime) {
    QByteArray realtimePackage("R-00.01N000.00?NF41\r");
    Sample expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(realtimePackage, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseWorkingModeOverloaded) {
    QByteArray overloadedPackage("O-00.01N000.00?NF38\r");
    Sample expectedResult = {WorkingMode::OVERLOADED, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(overloadedPackage, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseWorkingModeMaxCapacity) {
    QByteArray maxCapacityPackage("C-00.01N000.00?NF26\r");
    Sample expectedResult = {WorkingMode::MAX_CAPACITY, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(maxCapacityPackage, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}

// *****************************************************************************
// MeasureMode Tests
// *****************************************************************************
TEST_F(ParserTest, ParseMeasureModeAbsZero) {
    QByteArray absZeroPackage("R-00.01N000.00?NF41\r");
    Sample expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(absZeroPackage, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseMeasureModeRelZero) {
    QByteArray absZeroPackage("R-00.01Z000.00?NF53\r");
    Sample expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::REL_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(absZeroPackage, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}

// *****************************************************************************
// SpeedValue Tests
// *****************************************************************************
TEST_F(ParserTest, ParseSpeedValue10) {
    QByteArray speedValue10Package("R-00.01N000.00?NS54\r");
    Sample expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 10};
    bool dataOk = parser.parsePackage(speedValue10Package, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseSpeedValue40) {
    QByteArray speedValue40Package("R-00.01N000.00?NF41\r");
    Sample expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(speedValue40Package, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseSpeedValue640) {
    QByteArray speedValue640Package("R-00.01N000.00?NM48\r");
    Sample expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 640};
    bool dataOk = parser.parsePackage(speedValue640Package, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseSpeedValue1280) {
    QByteArray speedValue1280Package("R-00.01N000.00?NQ52\r");
    Sample expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 1280};
    bool dataOk = parser.parsePackage(speedValue1280Package, result);
    ASSERT_TRUE(dataOk) << "Unable to parse package";
    checkDataStruct(result, expectedResult);
}

}  // namespace
