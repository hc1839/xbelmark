#include "xbelmark/datetime/datetime.h"

#include <ctime>
#include <utility>

#include <gtest/gtest.h>

namespace xbelmark {
namespace datetime {

/**
 *  @brief Test various valid time-zone offsets.
 */
TEST(TzdToZoneOffset, Valid) {
  ASSERT_EQ(TzdToZoneOffset("Z"), 0);
  ASSERT_EQ(TzdToZoneOffset("+00:00"), 0);
  ASSERT_EQ(TzdToZoneOffset("-00:00"), 0);
  ASSERT_EQ(TzdToZoneOffset("+02:00"), 2 * 3600);
  ASSERT_EQ(TzdToZoneOffset("-05:00"), -5 * 3600);
  ASSERT_EQ(TzdToZoneOffset("-05:30"), -5 * 3600 + -30 * 60);
}

/**
 *  @brief Test various invalid time-zone offsets.
 */
TEST(TzdToZoneOffset, Invalid) {
  ASSERT_ANY_THROW(TzdToZoneOffset("00:00"));
  ASSERT_ANY_THROW(TzdToZoneOffset("+0:0"));
  ASSERT_ANY_THROW(TzdToZoneOffset("+00:00:00"));
  ASSERT_ANY_THROW(TzdToZoneOffset("+00:00:00.0"));
}

/**
 *  @brief Test various valid `xs:dateTime` values.
 */
TEST(DateTime, Valid) {
  ASSERT_EQ(static_cast<long>(DateTime("1970-01-01T00:00:00Z")), 0);
  ASSERT_EQ(static_cast<long>(DateTime("1970-01-01T00:00:00+00:00")), 0);
  ASSERT_EQ(static_cast<long>(DateTime("1970-01-01T00:00:00-00:00")), 0);
  ASSERT_EQ(static_cast<long>(DateTime("1900-01-01T00:00:00Z")), -2208988800);
  ASSERT_EQ(
      static_cast<long>(DateTime("2000-12-10T17:30:15-06:00")), 976491015);
  ASSERT_EQ(
      static_cast<long>(DateTime("2016-12-10T18:30:15-05:00")), 1481412615);
  ASSERT_EQ(
      static_cast<long>(DateTime("2016-12-11T00:30:15+01:00")), 1481412615);
  ASSERT_EQ(static_cast<long>(DateTime("2016-12-10T23:30:15Z")), 1481412615);
  ASSERT_DOUBLE_EQ(DateTime("2016-12-10T18:30:15.5-05:00"), 1481412615.5);
  ASSERT_DOUBLE_EQ(DateTime("2016-12-10T23:30:15.0123Z"), 1481412615.0123);
  ASSERT_DOUBLE_EQ(DateTime("1900-01-01T00:00:00.001Z"), -2208988800 + 0.001);
}

/**
 *  @brief Test various invalid `xs:dateTime` values.
 */
TEST(DateTime, Invalid) {
  ASSERT_ANY_THROW(DateTime("0000-01-01T00:00:00Z"));
  ASSERT_ANY_THROW(DateTime("1899-01-01T00:00:00Z"));
  ASSERT_ANY_THROW(DateTime("2016-12-10T18:30-05:00"));
  ASSERT_ANY_THROW(DateTime("2016-12T18:30:15-05:00"));
  ASSERT_ANY_THROW(DateTime("2016-12-10T18:30:15-05:00:00"));
  ASSERT_ANY_THROW(DateTime("1970-01-01"));
  ASSERT_ANY_THROW(DateTime("00:00:00Z"));
}

/**
 *  @brief Test various valid `xs:date` values.
 */
TEST(Date, Valid) {
  ASSERT_EQ(
      static_cast<long>(Date("1970-01-01Z")),
      static_cast<long>(DateTime("1970-01-01T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(Date("1970-01-01+00:00")),
      static_cast<long>(DateTime("1970-01-01T00:00:00+00:00")));
  ASSERT_EQ(
      static_cast<long>(Date("1970-01-01-00:00")),
      static_cast<long>(DateTime("1970-01-01T00:00:00-00:00")));
  ASSERT_EQ(
      static_cast<long>(Date("1970-01-01")),
      static_cast<long>(DateTime("1970-01-01T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(Date("1900-01-01Z")),
      static_cast<long>(DateTime("1900-01-01T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(Date("1900-01-01")),
      static_cast<long>(DateTime("1900-01-01T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(Date("2000-12-10-06:00")),
      static_cast<long>(DateTime("2000-12-10T00:00:00-06:00")));
  ASSERT_EQ(
      static_cast<long>(Date("2000-12-10")),
      static_cast<long>(DateTime("2000-12-10T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(Date("2016-12-10-05:00")),
      static_cast<long>(DateTime("2016-12-10T00:00:00-05:00")));
  ASSERT_EQ(
      static_cast<long>(Date("2016-12-11+01:00")),
      static_cast<long>(DateTime("2016-12-11T00:00:00+01:00")));
  ASSERT_EQ(
      static_cast<long>(Date("2016-12-10Z")),
      static_cast<long>(DateTime("2016-12-10T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(Date("2016-12-10")),
      static_cast<long>(DateTime("2016-12-10T00:00:00")));
}

/**
 *  @brief Test various invalid `xs:date` values.
 */
TEST(Date, Invalid) {
  ASSERT_ANY_THROW(Date("0000-01-01Z"));
  ASSERT_ANY_THROW(Date("1899-01-01Z"));
  ASSERT_ANY_THROW(Date("2016-12-10T18:30:15-05:00"));
  ASSERT_ANY_THROW(Date("2016-12"));
  ASSERT_ANY_THROW(Date("2016"));
}

/**
 *  @brief Test various valid `xs:gYearMonth` values.
 */
TEST(GYearMonth, Valid) {
  ASSERT_EQ(
      static_cast<long>(GYearMonth("1970-01Z")),
      static_cast<long>(DateTime("1970-01-01T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("1970-01+00:00")),
      static_cast<long>(DateTime("1970-01-01T00:00:00+00:00")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("1970-01-00:00")),
      static_cast<long>(DateTime("1970-01-01T00:00:00-00:00")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("1970-01")),
      static_cast<long>(DateTime("1970-01-01T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("1900-01Z")),
      static_cast<long>(DateTime("1900-01-01T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("1900-01")),
      static_cast<long>(DateTime("1900-01-01T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("2000-12-06:00")),
      static_cast<long>(DateTime("2000-12-01T00:00:00-06:00")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("2000-12")),
      static_cast<long>(DateTime("2000-12-01T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("2016-12-05:00")),
      static_cast<long>(DateTime("2016-12-01T00:00:00-05:00")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("2016-12+01:00")),
      static_cast<long>(DateTime("2016-12-01T00:00:00+01:00")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("2016-12Z")),
      static_cast<long>(DateTime("2016-12-01T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(GYearMonth("2016-12")),
      static_cast<long>(DateTime("2016-12-01T00:00:00")));
}

/**
 *  @brief Test various invalid `xs:gYearMonth` values.
 */
TEST(GYearMonth, Invalid) {
  ASSERT_ANY_THROW(GYearMonth("0000-01Z"));
  ASSERT_ANY_THROW(GYearMonth("1899-01Z"));
  ASSERT_ANY_THROW(GYearMonth("2016-12-10T18:30:15-05:00"));
  ASSERT_ANY_THROW(GYearMonth("2016-12-10"));
  ASSERT_ANY_THROW(GYearMonth("2016"));
}

/**
 *  @brief Test various valid `xs:gYear` values.
 */
TEST(GYear, Valid) {
  ASSERT_EQ(
      static_cast<long>(GYear("1970Z")),
      static_cast<long>(DateTime("1970-01-01T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(GYear("1970+00:00")),
      static_cast<long>(DateTime("1970-01-01T00:00:00+00:00")));
  ASSERT_EQ(
      static_cast<long>(GYear("1970-00:00")),
      static_cast<long>(DateTime("1970-01-01T00:00:00-00:00")));
  ASSERT_EQ(
      static_cast<long>(GYear("1970")),
      static_cast<long>(DateTime("1970-01-01T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(GYear("1900Z")),
      static_cast<long>(DateTime("1900-01-01T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(GYear("1900")),
      static_cast<long>(DateTime("1900-01-01T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(GYear("2000-06:00")),
      static_cast<long>(DateTime("2000-01-01T00:00:00-06:00")));
  ASSERT_EQ(
      static_cast<long>(GYear("2000")),
      static_cast<long>(DateTime("2000-01-01T00:00:00")));
  ASSERT_EQ(
      static_cast<long>(GYear("2016-05:00")),
      static_cast<long>(DateTime("2016-01-01T00:00:00-05:00")));
  ASSERT_EQ(
      static_cast<long>(GYear("2016+01:00")),
      static_cast<long>(DateTime("2016-01-01T00:00:00+01:00")));
  ASSERT_EQ(
      static_cast<long>(GYear("2016Z")),
      static_cast<long>(DateTime("2016-01-01T00:00:00Z")));
  ASSERT_EQ(
      static_cast<long>(GYear("2016")),
      static_cast<long>(DateTime("2016-01-01T00:00:00")));
}

/**
 *  @brief Test various invalid `xs:gYear` values.
 */
TEST(GYear, Invalid) {
  ASSERT_ANY_THROW(GYear("0000Z"));
  ASSERT_ANY_THROW(GYear("1899Z"));
  ASSERT_ANY_THROW(GYear("2016-12-10T18:30:15-05:00"));
  ASSERT_ANY_THROW(GYear("2016-12-10"));
  ASSERT_ANY_THROW(GYear("2016-12"));
}

} // namespace datetime
} // namespace xbelmark
