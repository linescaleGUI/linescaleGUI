#include <gtest/gtest.h>
#include "../../src/parser/parser.h"

namespace {
class ParserTest : public ::testing::Test {
   protected:
    Parser parser;
    DataStruct result;
};

void checkDataStruct(DataStruct& lhs, DataStruct& rhs) {
    EXPECT_EQ(lhs.workingMode, rhs.workingMode);
    EXPECT_EQ(lhs.measuredValue, rhs.measuredValue);
    EXPECT_EQ(lhs.measureMode, rhs.measureMode);
    EXPECT_EQ(lhs.referenceZero, rhs.referenceZero);
    EXPECT_EQ(lhs.batteryPercent, rhs.batteryPercent);
    EXPECT_EQ(lhs.unitValue, rhs.unitValue);
    EXPECT_EQ(lhs.frequency, rhs.frequency);
}

/**
 * @brief parser tests.
 */

// Unit vaule Tests
TEST_F(ParserTest, ParseUnitValueKn) {
    QByteArray KnPackage("R-00.01N000.00?NF41\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(KnPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseUnitValueLbf) {
    QByteArray lbfPackage("R999999N000.00?BF87\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, 999999, MeasureMode::ABS_ZERO, 0, 62, UnitValue::LBF, 40};
    bool dataOk = parser.parsePackage(lbfPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseUnitValueKgf) {
    QByteArray kgfPackage("R000019Z000001RGS95\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, 19,MeasureMode::REL_ZERO, 1, 100, UnitValue::KGF, 10};
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}

// Checksum Tests
TEST_F(ParserTest, ParseIncorrectChecksum) {
    QByteArray kgfPackage("R000.63Z-32.84RNS11\r");  // Package with a incorrect checksum
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_FALSE(dataOk);
}
TEST_F(ParserTest, ParseCorrectChecksum) {
    QByteArray kgfPackage("R000.63Z-32.84RNS10\r");  // Package with a correct checksum
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_TRUE(dataOk);
}

// Package lenght Tests
TEST_F(ParserTest, ParseToShortPackage) {
    QByteArray toShortPackage("-00.01N000.00?NF41\r"); //missing 'R' at the beginning -> lenght = 19
    bool dataOk = parser.parsePackage(toShortPackage, result);
    ASSERT_FALSE(dataOk);
}

TEST_F(ParserTest, ParseToLongPackage) {
    QByteArray toLongPackage("RR-00.01N000.00?NF41\r"); //one 'R' to many at the beginning -> lenght = 21
    bool dataOk = parser.parsePackage(toLongPackage, result);
    ASSERT_FALSE(dataOk);
}

// WorkingMode Tests
TEST_F(ParserTest, ParseWorkingModeRealtime) {
    QByteArray realtimePackage("R-00.01N000.00?NF41\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(realtimePackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseWorkingModeOverloaded) {
    QByteArray overloadedPackage("O-00.01N000.00?NF38\r");
    DataStruct expectedResult = {WorkingMode::OVERLOADED, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(overloadedPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseWorkingModeMaxCapacity) {
    QByteArray maxCapacityPackage("C-00.01N000.00?NF26\r");
    DataStruct expectedResult = {WorkingMode::MAX_CAPACITY, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(maxCapacityPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}

// MeassureMode Tests
TEST_F(ParserTest, ParseMeassureModeAbsZero) {
    QByteArray absZeroPackage("R-00.01N000.00?NF41\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(absZeroPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseMeassureModeRelZero) {
    QByteArray absZeroPackage("R-00.01Z000.00?NF53\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::REL_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(absZeroPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}

// SpeedValue Tests
TEST_F(ParserTest, ParseSpeedValue10) {
    QByteArray speedValue10Package("R-00.01N000.00?NS54\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 10};
    bool dataOk = parser.parsePackage(speedValue10Package, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseSpeedValue40) {
    QByteArray speedValue40Package("R-00.01N000.00?NF41\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(speedValue40Package, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseSpeedValue640) {
    QByteArray speedValue640Package("R-00.01N000.00?NM48\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 640};
    bool dataOk = parser.parsePackage(speedValue640Package, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseSpeedValue1280) {
    QByteArray speedValue1280Package("R-00.01N000.00?NQ52\r");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 1280};
    bool dataOk = parser.parsePackage(speedValue1280Package, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}

}  // namespace