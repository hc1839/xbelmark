package io.github.hc1839.xbelmark.xslt.ext;

import java.time.DateTimeException;
import java.time.LocalDate;
import java.time.LocalTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.time.temporal.ChronoField;
import java.time.temporal.TemporalAccessor;

/**
 *  Conversions of date/time formats.
 */
public class DateTime {
  /**
   *  Converts `xs:dateTime` or `xs:date` to seconds since epoch.
   *
   *  @param input
   *    Date/time string in `xs:dateTime` or `xs:date` format.
   *
   *  @return
   *    Seconds since epoch.
   */
  public static long dateTimeToUnix(String input) throws DateTimeException {
    TemporalAccessor temporal;
    LocalTime time;
    try {
      temporal = DateTimeFormatter.ISO_DATE_TIME.parse(input);
      time = LocalTime.from(temporal);
    } catch (Exception e) {
      temporal = DateTimeFormatter.ISO_DATE.parse(input);
      time = LocalTime.MIDNIGHT;
    }
    LocalDate date = LocalDate.from(temporal);
    ZoneId zone = temporal.isSupported(ChronoField.OFFSET_SECONDS)
                  ? ZoneId.from(temporal)
                  : ZoneId.systemDefault();
    return ZonedDateTime.of(date, time, zone).toEpochSecond();
  }
}
