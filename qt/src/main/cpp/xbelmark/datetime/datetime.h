#ifndef XBELMARK_DATETIME_DATETIME_H
#define XBELMARK_DATETIME_DATETIME_H

#include <cstddef>
#include <ctime>
#include <regex>
#include <stdexcept>
#include <string>

namespace xbelmark {
namespace datetime {

/**
 *  Epoch expressed in UTC.
 *
 *  @return
 *    Epoch expressed in UTC.
 */
inline std::tm EpochTm() {
  std::tm retval;
  retval.tm_year = 1970 - 1900;
  retval.tm_mon = 1 - 1;
  retval.tm_mday = 1;
  retval.tm_hour = 0;
  retval.tm_min = 0;
  retval.tm_sec = 0;
  retval.tm_yday = 0;
  retval.tm_wday = 4;
  retval.tm_isdst = -1;
  return retval;
}

/**
 *  Offset from UTC of the standard time in the local time zone.
 *
 *  @return
 *    Length of time in seconds to add to UTC to get standard time in the local
 *    time zone.
 */
inline int RawOffset() {
  std::tm local_tm(EpochTm());
  const std::time_t local_time(std::mktime(&local_tm));
  std::tm utc_tm(*std::gmtime(&local_time));
  return static_cast<int>(std::difftime(local_time, std::mktime(&utc_tm)));
}

/**
 *  Offset from UTC of the given time in the local time zone.
 *
 *  @param local_date_time
 *    Time in the local time zone.
 *
 *  @return
 *    Length of time in seconds to add to UTC to get `local_date_time` in the
 *    local time zone.
 */
inline int LocalZoneOffset(const std::tm &local_date_time) {
  std::tm std_time(local_date_time);
  std_time.tm_isdst = 0;
  std::tm local_time(local_date_time);
  local_time.tm_isdst = -1;
  const int dst_offset = static_cast<int>(
      std::difftime(std::mktime(&std_time), std::mktime(&local_time)));
  return RawOffset() + dst_offset;
}

/**
 *  Epoch.
 *
 *  @return
 *    Epoch.
 */
inline std::time_t EpochTime() {
  std::tm instant_tm(EpochTm());
  instant_tm.tm_sec += RawOffset();
  return std::mktime(&instant_tm);
}

/**
 *  Offset from UTC of the given time-zone designator.
 *
 *  @param input
 *    Time-zone designator in XML Schema format.
 *
 *  @return
 *    Length of time in seconds to add to UTC to get the time zone specified by
 *    `input`.
 */
inline int TzdToZoneOffset(const std::string &input) {
  const std::regex re("^((\\+|-)(\\d{2}):(\\d{2})|Z)$");
  std::smatch match;
  std::regex_search(input.begin(), input.end(), match, re);
  if (match.empty()) {
    throw std::invalid_argument(
        "Not a valid time-zone designator format: " + input);
  }
  if (match.str() == "Z") {
    return 0;
  } else {
    const int sign = match.str(2) == "+" ? 1 : -1;
    const int hour_offset = sign * std::stoi(match.str(3));
    const int min_offset = sign * std::stoi(match.str(4));
    return hour_offset * 3600 + min_offset * 60;
  }
}

/**
 *  Seconds since epoch of a date/time.
 *
 *  Year before 1900 is currently disallowed.
 *
 *  @param input
 *    `xs:dateTime` in XML Schema format.
 *
 *  @return
 *    Seconds since epoch of `input`.
 */
inline double DateTime(const std::string &input) {
  std::string re_str;
  re_str += "^([1-9]\\d{4,}|\\d{4})-(\\d{2})-(\\d{2})";
  re_str += "T(\\d{2}):(\\d{2}):(\\d{2})(\\.\\d+)?";
  re_str += "(.*)$";
  const std::regex re(re_str);
  std::smatch match;
  std::regex_search(input.begin(), input.end(), match, re);
  if (match.empty()) {
    throw std::invalid_argument("Not a valid `xs:dateTime` format: " + input);
  }
  if (match.str(1) == "0000") {
    throw std::invalid_argument(
        "Year 0 is disallowed for its ambiguous interpretation: " + input);
  }
  std::tm input_tm(EpochTm());
  input_tm.tm_year = std::stoi(match.str(1)) - 1900;
  if (input_tm.tm_year < 0) {
    throw std::invalid_argument(
        "Year before 1900 is currently disallowed: " + input);
  }
  input_tm.tm_mon = std::stoi(match.str(2)) - 1;
  input_tm.tm_mday = std::stoi(match.str(3));
  input_tm.tm_hour = std::stoi(match.str(4));
  input_tm.tm_min = std::stoi(match.str(5));
  input_tm.tm_sec = std::stoi(match.str(6));
  const double fractional_sec =
      match.str(7).empty() ? 0 : std::stod(match.str(7));

  const int local_offset(LocalZoneOffset(input_tm));
  const int input_offset(
      match.str(8).empty() ? local_offset : TzdToZoneOffset(match.str(8)));
  input_tm.tm_sec += -input_offset + local_offset;

  return std::difftime(std::mktime(&input_tm), EpochTime()) + fractional_sec;
}

/**
 *  Seconds since epoch of a date.
 *
 *  Year before 1900 is currently disallowed.
 *
 *  @param input
 *    `xs:date` in XML Schema format.
 *
 *  @return
 *    Seconds since epoch of `input`.
 */
inline double Date(const std::string &input) {
  std::string re_str;
  re_str += "^([1-9]\\d{4,}|\\d{4})-(\\d{2})-(\\d{2})";
  re_str += "(.*)$";
  const std::regex re(re_str);
  std::smatch match;
  std::regex_search(input.begin(), input.end(), match, re);
  if (match.empty()) {
    throw std::invalid_argument("Not a valid `xs:date` format: " + input);
  }
  if (match.str(1) == "0000") {
    throw std::invalid_argument(
        "Year 0 is disallowed for its ambiguous interpretation: " + input);
  }
  std::tm input_tm(EpochTm());
  input_tm.tm_year = std::stoi(match.str(1)) - 1900;
  if (input_tm.tm_year < 0) {
    throw std::invalid_argument(
        "Year before 1900 is currently disallowed: " + input);
  }
  input_tm.tm_mon = std::stoi(match.str(2)) - 1;
  input_tm.tm_mday = std::stoi(match.str(3));

  const int local_offset(LocalZoneOffset(input_tm));
  const int input_offset(
      match.str(4).empty() ? local_offset : TzdToZoneOffset(match.str(4)));
  input_tm.tm_sec += -input_offset + local_offset;

  return std::difftime(std::mktime(&input_tm), EpochTime());
}

/**
 *  Seconds since epoch of a month in a year.
 *
 *  Year before 1900 is currently disallowed.
 *
 *  @param input
 *    `xs:gYearMonth` in XML Schema format.
 *
 *  @return
 *    Seconds since epoch of `input`.
 */
inline double GYearMonth(const std::string &input) {
  std::string re_str;
  re_str += "^([1-9]\\d{4,}|\\d{4})-(\\d{2})";
  re_str += "(.*)$";
  const std::regex re(re_str);
  std::smatch match;
  std::regex_search(input.begin(), input.end(), match, re);
  if (match.empty()) {
    throw std::invalid_argument(
        "Not a valid `xs:gYearMonth` format: " + input);
  }
  if (match.str(1) == "0000") {
    throw std::invalid_argument(
        "Year 0 is disallowed for its ambiguous interpretation: " + input);
  }
  std::tm input_tm(EpochTm());
  input_tm.tm_year = std::stoi(match.str(1)) - 1900;
  if (input_tm.tm_year < 0) {
    throw std::invalid_argument(
        "Year before 1900 is currently disallowed: " + input);
  }
  input_tm.tm_mon = std::stoi(match.str(2)) - 1;

  const int local_offset(LocalZoneOffset(input_tm));
  const int input_offset(
      match.str(3).empty() ? local_offset : TzdToZoneOffset(match.str(3)));
  input_tm.tm_sec += -input_offset + local_offset;

  return std::difftime(std::mktime(&input_tm), EpochTime());
}

/**
 *  Seconds since epoch of a year.
 *
 *  Year before 1900 is currently disallowed.
 *
 *  @param input
 *    `xs:gYear` in XML Schema format.
 *
 *  @return
 *    Seconds since epoch of `input`.
 */
inline double GYear(const std::string &input) {
  std::string re_str;
  re_str += "^([1-9]\\d{4,}|\\d{4})";
  re_str += "(.*)$";
  const std::regex re(re_str);
  std::smatch match;
  std::regex_search(input.begin(), input.end(), match, re);
  if (match.empty()) {
    throw std::invalid_argument("Not a valid `xs:gYear` format: " + input);
  }
  if (match.str(1) == "0000") {
    throw std::invalid_argument(
        "Year 0 is disallowed for its ambiguous interpretation: " + input);
  }
  std::tm input_tm(EpochTm());
  input_tm.tm_year = std::stoi(match.str(1)) - 1900;
  if (input_tm.tm_year < 0) {
    throw std::invalid_argument(
        "Year before 1900 is currently disallowed: " + input);
  }

  const int local_offset(LocalZoneOffset(input_tm));
  const int input_offset(
      match.str(2).empty() ? local_offset : TzdToZoneOffset(match.str(2)));
  input_tm.tm_sec += -input_offset + local_offset;

  return std::difftime(std::mktime(&input_tm), EpochTime());
}

} // namespace datetime
} // namespace xbelmark

#endif
