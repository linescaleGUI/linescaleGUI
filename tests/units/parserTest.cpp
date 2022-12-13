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
 * @brief parser test.
 */
TEST_F(ParserTest, ParseKN) {
    QByteArray KnPackage("R-00.01N000.00?NF41");
    DataStruct expectedResult = {WorkingMode::REALTIME, -00.01,MeasureMode::ABS_ZERO, 000.00, 62, UnitValue::KN, 40};
    bool dataOk = parser.parsePackage(KnPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseLbf) {
    QByteArray lbfPackage("R999999N000.00?BF87");
    DataStruct expectedResult = {WorkingMode::REALTIME, 999999, MeasureMode::ABS_ZERO, 0, 62, UnitValue::LBF, 40};
    bool dataOk = parser.parsePackage(lbfPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseKgf) {
    QByteArray kgfPackage("R000019Z000001RGS95");
    DataStruct expectedResult = {WorkingMode::REALTIME, 19,MeasureMode::REL_ZERO, 1, 100, UnitValue::KGF, 10};
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseIncorrectChecksum) {
    QByteArray kgfPackage("R000.63Z-32.84RNS11");  // Package with a incorrect checksum
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_FALSE(dataOk);
}
TEST_F(ParserTest, ParseCorrectChecksum) {
    QByteArray kgfPackage("R000.63Z-32.84RNS10");  // Package with a correct checksum
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_TRUE(dataOk);
}
}  // namespace