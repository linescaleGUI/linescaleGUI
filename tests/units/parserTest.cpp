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
    DataStruct expectedResult = {realTime, -00.01, absZero, 000.00, 62, kN, 40};
    bool dataOk = parser.parsePackage(KnPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseLbf) {
    QByteArray lbfPackage("R999999N000.00?BF87");
    DataStruct expectedResult = {realTime, 999999, absZero, 0, 62, lbf, 40};
    bool dataOk = parser.parsePackage(lbfPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseKgf) {
    QByteArray kgfPackage("R000019Z000001RGS95");
    DataStruct expectedResult = {realTime, 19, relZero, 1, 100, kgf, 10};
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_TRUE(dataOk);
    checkDataStruct(result, expectedResult);
}
TEST_F(ParserTest, ParseWrongChecksum) {
    QByteArray kgfPackage("R000.63Z-32.84RNS11");  // Package with a wrong checksum
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_FALSE(dataOk);
}
TEST_F(ParserTest, ParseCorrectChecksum) {
    QByteArray kgfPackage("R000.63Z-32.84RNS10");  // Package with a correct checksum
    bool dataOk = parser.parsePackage(kgfPackage, result);
    ASSERT_TRUE(dataOk);
}
}  // namespace